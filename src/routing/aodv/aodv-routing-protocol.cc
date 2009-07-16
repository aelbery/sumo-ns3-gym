/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Based on 
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 * 
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Borovkova <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#include "aodv-routing-protocol.h"
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-route.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/nstime.h"
#include "ns3/net-device.h"


/// UDP Port for AODV control traffic
#define AODV_PORT  654

NS_LOG_COMPONENT_DEFINE ("AodvRoutingProtocol");

namespace ns3
{
namespace aodv
{
NS_OBJECT_ENSURE_REGISTERED (RoutingProtocol);

void
RoutingProtocol::InsertBroadcastId (Ipv4Address id, uint32_t bid)
{
  if(LookupBroadcastId(id, bid) )
    return;
  struct BroadcastId broadcastId = {id, bid,BCAST_ID_SAVE + Simulator::Now()};
  m_broadcastIdCache.push_back(broadcastId);
}
bool
RoutingProtocol::LookupBroadcastId (Ipv4Address id, uint32_t bid)
{
  std::vector<BroadcastId>::const_iterator i;
  for(i = m_broadcastIdCache.begin(); i != m_broadcastIdCache.end(); ++i)
    if(i->src == id && i->id == bid)
      return true;
  return false;
}
void
RoutingProtocol::PurgeBroadcastId ()
{
  std::vector<BroadcastId>::iterator i = remove_if(m_broadcastIdCache.begin(), m_broadcastIdCache.end(), IsExpired());
  m_broadcastIdCache.erase(i, m_broadcastIdCache.end());
}

RoutingProtocol::RoutingProtocol() :
  ACTIVE_ROUTE_TIMEOUT(Seconds(3)),
  MY_ROUTE_TIMEOUT(Scalar(2)*ACTIVE_ROUTE_TIMEOUT),
  NET_DIAMETER(35),
  NODE_TRAVERSAL_TIME(MilliSeconds(40)),
  ALLOWED_HELLO_LOSS (2),
  BAD_LINK_LIFETIME (Seconds (3)),
  FREQUENCY (Seconds (0.5)),
  m_broadcastID(1), m_seqNo(2/*??*/),
  btimer(Timer::REMOVE_ON_DESTROY),
  htimer(Timer::REMOVE_ON_DESTROY),
  ntimer(Timer::REMOVE_ON_DESTROY),
  rtimer(Timer::REMOVE_ON_DESTROY),
  lrtimer(Timer::REMOVE_ON_DESTROY)

  {
  NET_TRAVERSAL_TIME = Scalar(2 * NET_DIAMETER) * NODE_TRAVERSAL_TIME;
  MaxHelloInterval = Scalar(1.25) * HELLO_INTERVAL;
  MinHelloInterval = Scalar(0.75) * HELLO_INTERVAL;
  }

TypeId 
RoutingProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::aodv::RoutingProtocol")
          .SetParent<Ipv4RoutingProtocol> ()
          .AddConstructor<RoutingProtocol> ()
          .AddAttribute ("HelloInterval", "HELLO messages emission interval.",
              TimeValue (Seconds (1)),
              MakeTimeAccessor (&RoutingProtocol::HELLO_INTERVAL),
              MakeTimeChecker ())
              .AddAttribute ("Broadcast id save", "Broadcast id save interval.",
                  TimeValue (Seconds (6)),
                  MakeTimeAccessor (&RoutingProtocol::BCAST_ID_SAVE),
                  MakeTimeChecker ())

                  ;
  return tid;
}

RoutingProtocol::~RoutingProtocol()
{
}

void
RoutingProtocol::DoDispose ()
{
  m_ipv4 = 0;
  for (std::map< Ptr<Socket>, Ipv4InterfaceAddress >::iterator iter = m_socketAddresses.begin ();
      iter != m_socketAddresses.end (); iter++)
  {
    iter->first->Close ();
  }
  m_socketAddresses.clear ();
  Ipv4RoutingProtocol::DoDispose ();
}

void
RoutingProtocol::Start ()
{
  NS_LOG_FUNCTION (this);
  // Open UDP sockets for control traffic on each IP interface
  const Ipv4Address loopback ("127.0.0.1");
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
  {
    Ipv4InterfaceAddress iface = m_ipv4->GetAddress (i, 0);
    if (iface.GetLocal() == loopback) continue;

    // Create a socket to listen only on this interface
    Ptr<Socket> socket = Socket::CreateSocket( GetObject<Node> (), TypeId::LookupByName ("ns3::UdpSocketFactory"));
    NS_ASSERT (socket != 0);
    int status = socket->Bind (InetSocketAddress (iface.GetLocal(), AODV_PORT));
    NS_ASSERT (status != -1);
    socket->SetRecvCallback (MakeCallback (&RoutingProtocol::RecvAodv,  this));
    status = socket->Connect (InetSocketAddress (iface.GetBroadcast(), AODV_PORT));
    NS_ASSERT (status != -1);

    m_socketAddresses.insert(std::make_pair(socket, iface));
    NS_LOG_INFO ("Interface " << iface << " used by AODV");

    // Add local broadcast record to the routing table
    Ptr<NetDevice> dev = m_ipv4->GetNetDevice(m_ipv4->GetInterfaceForAddress (iface.GetLocal()));
    RoutingTableEntry rt(/*device=*/dev, /*dst=*/iface.GetBroadcast (),
        /*know seqno=*/true, /*seqno=*/0,
        /*iface=*/iface.GetLocal (),
        /*hops=*/1,
        /*next hop=*/iface.GetBroadcast (),
        /*lifetime=*/Seconds(1e9)); // TODO use infty
    m_routingTable.AddRoute (rt);
  }
}

Ptr<Ipv4Route> 
RoutingProtocol::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << p->GetUid() << header.GetDestination());
  Ptr<Ipv4Route> rtentry;
  Ipv4Address dst = header.GetDestination();
  Ptr<NetDevice> dev;
  RoutingTableEntry rt(dev);
  if (! m_routingTable.LookupRoute(dst, rt))
  {
    SendRequest(dst, false, false);
    sockerr = Socket::ERROR_NOROUTETOHOST;
    return rtentry;
  }
  else
  {
    rtentry = rt.GetRoute();
    NS_ASSERT (rtentry != 0);
    NS_LOG_LOGIC("exist route to " << rtentry->GetDestination());
    return rtentry;
  }

}

bool
RoutingProtocol::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
    UnicastForwardCallback ucb, MulticastForwardCallback mcb,
    LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << p->GetUid() <<  header.GetDestination() << idev->GetAddress());

  NS_ASSERT (m_ipv4 != 0);
  // Check if input device supports IP
  NS_ASSERT (m_ipv4->GetInterfaceForDevice (idev) >= 0);
  uint32_t iif = m_ipv4->GetInterfaceForDevice (idev);

  Ipv4Address dst = header.GetDestination ();

  // Local delivery to AODV interfaces
  for(std::map<Ptr<Socket>, Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin(); j != m_socketAddresses.end(); ++j)
  {
    Ipv4InterfaceAddress iface = j->second;

    if (dst == iface.GetLocal ())
    {
      NS_LOG_LOGIC ("Unicast local delivery to " << iface.GetLocal ());
      lcb(p, header, iif);
      return true;
    }
    if (dst == iface.GetBroadcast ())
    {
      NS_LOG_LOGIC ("Broadcast local delivery to " << iface.GetLocal ());
      // TODO not duplicate
      lcb(p, header, iif);
      // TODO has TTL, forward
      return true;
    }
  }

  // TODO: local delivery to non-AODV interfaces

  // Locally deliver all AODV control traffic
  if (LooksLikeAodvControl (p, header))
  {
    NS_LOG_LOGIC("Locally deliver AODV control traffic to " << m_ipv4->GetAddress(iif, 0).GetLocal() << " packet " << p->GetUid());
    Ipv4Header h = header;
    h.SetDestination(m_ipv4->GetAddress(iif, 0).GetLocal()); // really want to receive it
    lcb(p, h, iif);
    return true;
  }

  // Forwarding
  Ptr<NetDevice> dev;
  RoutingTableEntry rt(dev);
  if (m_routingTable.LookupRoute(dst, rt))
  {
    Ptr<Ipv4Route> rtentry = rt.GetRoute();
    NS_LOG_LOGIC(rtentry->GetSource()<<" forwarding to " << dst);
    ucb (rtentry, p, header);
    return true;
  }
  NS_LOG_LOGIC("route not found to "<< header.GetDestination());
  m_routingTable.Print(std::cout);
  return false;
}

void 
RoutingProtocol::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (ipv4 != 0);
  NS_ASSERT (m_ipv4 == 0);

  btimer.SetFunction (& RoutingProtocol::BroadcastTimerExpire, this);
  ntimer.SetFunction (& RoutingProtocol::NeighborTimerExpire, this);
  rtimer.SetFunction (& RoutingProtocol::RouteCacheTimerExpire, this);
  lrtimer.SetFunction (& RoutingProtocol::LocalRepairTimerExpire, this);
  htimer.SetFunction (& RoutingProtocol::LocalRepairTimerExpire, this);

  m_ipv4 = ipv4;
  Simulator::ScheduleNow (&RoutingProtocol::Start, this);
}

void 
RoutingProtocol::NotifyInterfaceUp (uint32_t i)
{
  NS_LOG_FUNCTION (this << m_ipv4->GetAddress (i, 0).GetLocal ());
  // TODO
}

void 
RoutingProtocol::NotifyInterfaceDown (uint32_t i)
{ 
  NS_LOG_FUNCTION (this << m_ipv4->GetAddress (i, 0).GetLocal ());
  // TODO
}

void 
RoutingProtocol::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
}

void 
RoutingProtocol::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
}

bool 
RoutingProtocol::LooksLikeAodvControl (Ptr<const Packet> p, Ipv4Header const & header) const
{
  if (header.GetProtocol() == 17 /*UDP*/)
    {
      UdpHeader uh;
      return (p->PeekHeader(uh) && uh.GetDestinationPort() == AODV_PORT && uh.GetSourcePort() == AODV_PORT);
    }
  return false;
}

// TODO add use an expanding ring search technique
void 
RoutingProtocol::SendRequest (Ipv4Address dst, bool G, bool D)
{
  NS_LOG_FUNCTION (this << dst);

  // Create RREQ header
  TypeHeader tHeader (AODVTYPE_RREQ);
  RreqHeader rreqHeader;
  rreqHeader.SetDst (dst);

  Ptr<NetDevice> dev;
  RoutingTableEntry rt(dev);
  if(m_routingTable.LookupRoute (dst, rt))
  {
    rreqHeader.SetHopCount (rt.GetLastValidHopCount());
    rreqHeader.SetDstSeqno (rt.GetSeqNo());
  }
  else
  {
    rreqHeader.SetUnknownSeqno(true);
  }

  if (G) rreqHeader.SetGratiousRrep(true);
  if (D) rreqHeader.SetDestinationOnly(true);

  m_seqNo++;
  rreqHeader.SetOriginSeqno(m_seqNo);
  m_broadcastID++;
  rreqHeader.SetId(m_broadcastID);
  rreqHeader.SetHopCount(0);

  // Send RREQ as subnet directed broadcast from each (own) interface
  Ptr<Packet> packet = Create<Packet> ();
  for(std::map<Ptr<Socket>, Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin(); j != m_socketAddresses.end(); ++j)
  {
    Ptr<Socket> socket = j->first;
    Ipv4InterfaceAddress iface = j->second;

    rreqHeader.SetOrigin (iface.GetLocal());
    InsertBroadcastId (iface.GetLocal(), m_broadcastID);


    packet->AddHeader (rreqHeader);
    packet->AddHeader (tHeader);
    socket->Send (packet);
  }
  //  htimer.SetDelay(HELLO_INTERVAL);
  //  htimer.Schedule();
}

void 
RoutingProtocol::RecvAodv (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);

  Ptr<Packet> packet;
  Address sourceAddress;
  packet = socket->RecvFrom (sourceAddress);

  InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom (sourceAddress);
  NS_ASSERT (inetSourceAddr.GetPort () == AODV_PORT);
  Ipv4Address senderIfaceAddr = inetSourceAddr.GetIpv4 ();
  Ipv4Address receiverIfaceAddr = m_socketAddresses[socket].GetLocal();

  NS_LOG_DEBUG ("AODV node " << this << " received a AODV packet from " << senderIfaceAddr << " to " << receiverIfaceAddr);

  UpdateNeighbor (senderIfaceAddr, receiverIfaceAddr);

  TypeHeader tHeader(AODVTYPE_RREQ);

  packet->RemoveHeader(tHeader);
  if (!tHeader.IsValid())
  {
    NS_LOG_WARN ("AODV message with unknown type received: " << tHeader.Get());
    return; // drop
  }

  switch (tHeader.Get())
  {
    case AODVTYPE_RREQ:
    {
      RecvRequest (packet, receiverIfaceAddr, senderIfaceAddr, socket);
      break;
    }
    case AODVTYPE_RREP:
    {
      RecvReply (packet, receiverIfaceAddr, senderIfaceAddr);
      break;
    }
    case AODVTYPE_RERR:
    {
      RecvError (packet);
      break;
    }
    case AODVTYPE_RREP_ACK:
      break;
  }
}

void
RoutingProtocol::UpdateNeighbor(Ipv4Address sender, Ipv4Address receiver)
{
  NS_LOG_FUNCTION (this << "sender " << sender << " receiver " << receiver );
  Ptr<NetDevice> dev;
  RoutingTableEntry toNeighbor(dev);
  if(!m_routingTable.LookupRoute(sender, toNeighbor))
  {
    dev = m_ipv4->GetNetDevice(m_ipv4->GetInterfaceForAddress (receiver));
    RoutingTableEntry newEntry(/*device=*/dev, /*dst=*/sender,
        /*know seqno=*/false, /*seqno=*/0,
        /*iface=*/receiver,
        /*hops=*/1, /*next hop=*/sender,
        /*lifetime=*/Simulator::Now() + ACTIVE_ROUTE_TIMEOUT);
    m_routingTable.AddRoute(newEntry);
  }
  else
  {
    toNeighbor.SetFlag (RTF_UP);
    Time t = toNeighbor.GetLifeTime();
    if (t < Simulator::Now() + ACTIVE_ROUTE_TIMEOUT)
      toNeighbor.SetLifeTime(Simulator::Now() + ACTIVE_ROUTE_TIMEOUT);
    m_routingTable.Update(sender, toNeighbor);
  }
}

void 
RoutingProtocol::RecvRequest (Ptr<Packet> p, Ipv4Address receiver, Ipv4Address src, Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << receiver << src);

  RreqHeader rreqHeader;
  p->RemoveHeader(rreqHeader);  // TODO check that header correctly found
  uint32_t id = rreqHeader.GetId();
  Ipv4Address origin = rreqHeader.GetOrigin();

  // Node checks to determine whether it has received a RREQ
  // with the same Originator IP Address and RREQ ID.
  // If such a RREQ has been received, the node
  // silently discards the newly received RREQ.
  if (LookupBroadcastId (origin, id))
  {
    NS_LOG_DEBUG ("My interface " << receiver <<" RREQ duplicate from " << origin << " dropped by id " << id);
    return;
  }
  InsertBroadcastId (origin, id);
 
  // Increment RREQ hop count
  uint8_t hop = rreqHeader.GetHopCount() + 1;
  rreqHeader.SetHopCount (hop);

  // Reverse route to the Originator IP Address is created, or updating
  Ptr<NetDevice> dev;
  RoutingTableEntry toOrigin(dev);
  if(! m_routingTable.LookupRoute(origin, toOrigin))
  {
    dev = m_ipv4->GetNetDevice(m_ipv4->GetInterfaceForAddress (receiver));
    RoutingTableEntry newEntry(/*device=*/dev, /*dst=*/origin, /*validSeno=*/true, /*seqNo=*/rreqHeader.GetOriginSeqno(),
        /*iface=*/receiver, /*hops=*/hop, /*nextHop*/src,
        /*timeLife=*/Simulator::Now() + Scalar(2)*NET_TRAVERSAL_TIME - Scalar(2*hop)*NODE_TRAVERSAL_TIME  );
    m_routingTable.AddRoute (newEntry);
  }
  else  // TODO check logic
  {
    if (toOrigin.GetSeqNo() < rreqHeader.GetOriginSeqno())  toOrigin.SetSeqNo (rreqHeader.GetOriginSeqno());
    toOrigin.SetValidSeqNo(true);
    toOrigin.SetNextHop(src);
    toOrigin.SetOutputDevice(m_ipv4->GetNetDevice(m_ipv4->GetInterfaceForAddress (receiver)));
    toOrigin.SetHop(hop);
    Time minimalLifetime = Simulator::Now() + Scalar(2)*NET_TRAVERSAL_TIME - Scalar(2*hop)*NODE_TRAVERSAL_TIME;
    if (toOrigin.GetLifeTime() < minimalLifetime)
      toOrigin.SetLifeTime(minimalLifetime);
    m_routingTable.Update(origin, toOrigin);
  }

  //  A node generates a RREP if either:
  //
  //  (i)  it is itself the destination, or
  for (uint32_t k = 0; k < m_ipv4->GetNInterfaces (); k++)
  {
    Ipv4Address addr = m_ipv4->GetAddress (k, 0).GetLocal ();
    if (addr == rreqHeader.GetDst()) 
    {
      m_routingTable.LookupRoute(origin, toOrigin);
      SendReply (rreqHeader, toOrigin, socket);
      return;
    }
  }

  //  (ii) it has an active route to the destination, the destination
  //       sequence number in the node's existing route table entry
  //       for the destination is valid and greater than or equal to
  //       the Destination Sequence Number of the RREQ ,
  //       and the "destination only" ('D') flag is NOT set.

  RoutingTableEntry toDst(dev);
  Ipv4Address dst = rreqHeader.GetDst();
  if (m_routingTable.LookupRoute(dst, toDst))
  {
    // The Destination Sequence number for the requested destination is set
    // to the maximum of the corresponding value received in the RREQ message,
    // and the destination sequence value currently maintained by the node for
    // the requested destination. However, the forwarding node MUST NOT modify
    // its maintained value for the destination sequence number, even if the value
    // received in the incoming RREQ is larger than the value currently maintained
    // by the forwarding node.
    uint32_t dstSeqNo =  toDst.GetSeqNo();
    if (rreqHeader.GetUnknownSeqno() || (rreqHeader.GetDstSeqno() < dstSeqNo))
    {
      if(!rreqHeader.GetDestinationOnly() && toDst.GetValidSeqNo() && (toDst.GetFlag() == RTF_UP))
      {
        SendReplyByIntermediateNode(toDst, toOrigin, rreqHeader.GetGratiousRrep());
        return;
      }
      rreqHeader.SetOriginSeqno(dstSeqNo);
      rreqHeader.SetUnknownSeqno(false);
    }
  }

  // If a node does not generate a RREP the incoming IP header has
  // TTL larger than 1, the node updates and broadcasts the RREQ
  // to address 255.255.255.255 on each of its configured interfaces.
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader(rreqHeader);
  TypeHeader tHeader(AODVTYPE_RREQ);
  packet->AddHeader(tHeader);

  for(std::map<Ptr<Socket>, Ipv4InterfaceAddress >::const_iterator j = m_socketAddresses.begin(); j != m_socketAddresses.end(); ++j)
  {
    Ptr<Socket> socket = j->first;
    Ipv4InterfaceAddress iface = j->second;
    socket->Send(packet);
  }

  // Shift hello timer
//  htimer.Schedule();
}

void 
RoutingProtocol::SendReply (RreqHeader const & rreqHeader, RoutingTableEntry const & toOrigin, Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << toOrigin.GetDestination ());
  // Create RREP
  RrepHeader rrepHeader;
  rrepHeader.SetHopCount(toOrigin.GetHop());
  rrepHeader.SetOrigin(toOrigin.GetDestination());
  //  Destination node MUST increment its own sequence number by one
  //  if the sequence number in the RREQ packet is equal to that incremented value.
  //  Otherwise, the destination does not change its sequence number before
  //  generating the  RREP message.
  if(!rreqHeader.GetUnknownSeqno() && (rreqHeader.GetDstSeqno() == m_seqNo + 1))
    m_seqNo++;
  rrepHeader.SetDstSeqno(m_seqNo);
  rrepHeader.SetDst(rreqHeader.GetDst());
  rrepHeader.SetLifeTime(MY_ROUTE_TIMEOUT);


  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader(rrepHeader);
  TypeHeader tHeader(AODVTYPE_RREP);
  packet->AddHeader(tHeader);

  socket->SendTo (packet, 0, InetSocketAddress(toOrigin.GetDestination (), AODV_PORT));
}

void 
RoutingProtocol::RecvReply (Ptr<Packet> p, Ipv4Address receiverIfaceAddr ,Ipv4Address senderIfaceAddr)
{
  NS_LOG_FUNCTION(this << senderIfaceAddr);
  RrepHeader rrepHeader;
  p->RemoveHeader(rrepHeader);

  uint8_t hop = rrepHeader.GetHopCount() + 1;
  rrepHeader.SetHopCount(hop);
  Ptr<NetDevice> dev;
  RoutingTableEntry toDst (dev);

  //  If the route table entry to the destination is created or updated,
  //    then the following actions occur:
  //    -  the route is marked as active,
  //    -  the destination sequence number is marked as valid,
  //    -  the next hop in the route entry is assigned to be the node from
  //       which the RREP is received, which is indicated by the source IP
  //       address field in the IP header,
  //    -  the hop count is set to the value of the New Hop Count,
  //    -  the expiry time is set to the current time plus the value of the
  //       Lifetime in the RREP message,
  //    -  and the destination sequence number is the Destination Sequence
  //       Number in the RREP message.

  // The forward route for this destination is created if it does not already exist.
  if(!m_routingTable.LookupRoute(rrepHeader.GetDst(), toDst))
  {
    dev = m_ipv4->GetNetDevice(m_ipv4->GetInterfaceForAddress (receiverIfaceAddr));
    RoutingTableEntry newEntry(/*device=*/dev, /*dst=*/rrepHeader.GetDst(), /*validSeqNo=*/true, /*seqno=*/rrepHeader.GetDstSeqno(), /*iface=*/receiverIfaceAddr,
        /*hop=*/hop, /*nextHop=*/senderIfaceAddr, /*lifeTime=*/Simulator::Now() + rrepHeader.GetLifeTime());
    m_routingTable.AddRoute(newEntry);
  }
  //  The existing entry is updated only in the following circumstances:
  else
  {
    //  (i)   the sequence number in the routing table is marked as
    //        invalid in route table entry.
    toDst.SetValidSeqNo(true);
    toDst.SetFlag(RTF_UP);
    toDst.SetNextHop(senderIfaceAddr);
    toDst.SetLifeTime(Simulator::Now() + rrepHeader.GetLifeTime());
    toDst.SetHop(hop);
    if(!toDst.GetValidSeqNo())
    {
      m_routingTable.Update(rrepHeader.GetDst(), toDst);
    }
    //  (ii)  the Destination Sequence Number in the RREP is greater than
    //        the node's copy of the destination sequence number and the
    //        known value is valid,
    else if(rrepHeader.GetDstSeqno() > toDst.GetSeqNo())
    {
      toDst.SetSeqNo(rrepHeader.GetDstSeqno());
      m_routingTable.Update(rrepHeader.GetDst(), toDst);
    }
    else
    {
      //  (iii) the sequence numbers are the same, but the route is marked as inactive.
      if( (rrepHeader.GetDstSeqno() == toDst.GetSeqNo()) && (toDst.GetFlag() != RTF_UP) )
      {
        m_routingTable.Update(rrepHeader.GetDst(), toDst);
      }
      //  (iv)   the sequence numbers are the same, and the New Hop Count is
      //         smaller than the hop count in route table entry.
      else if( (rrepHeader.GetDstSeqno() == toDst.GetSeqNo()) && (hop < toDst.GetHop()) )
      {
        m_routingTable.Update(rrepHeader.GetDst(), toDst);
      }
    }
  }

  if(receiverIfaceAddr == rrepHeader.GetOrigin())
  {
    //TODO  may be send messeges from queue
    return;
  }


  RoutingTableEntry toOrigin(dev);
  if(!m_routingTable.LookupRoute(rrepHeader.GetOrigin(), toOrigin))
  {
    //        imposible!
    //        drop();
    return;
  }

  m_routingTable.LookupRoute(rrepHeader.GetDst(), toDst);
  toDst.InsertPrecursor(toOrigin.GetNextHop());
  m_routingTable.Update(rrepHeader.GetDst(), toDst);

  if ( toOrigin.GetLifeTime() < (Simulator::Now() + ACTIVE_ROUTE_TIMEOUT) )
  {
    toOrigin.SetLifeTime(Simulator::Now() + ACTIVE_ROUTE_TIMEOUT);
    m_routingTable.Update(toOrigin.GetDestination (), toOrigin);
  }
  RoutingTableEntry toNextHopToDst(dev);
  m_routingTable.LookupRoute(toDst.GetNextHop(), toNextHopToDst);
  toNextHopToDst.InsertPrecursor(toOrigin.GetNextHop());
  m_routingTable.Update(toDst.GetNextHop(), toNextHopToDst);

  // TODO add operation over unidirctinal links
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader(rrepHeader);
  TypeHeader tHeader(AODVTYPE_RREP);
  packet->AddHeader(tHeader);
  for (std::map<Ptr<Socket>, Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin(); j != m_socketAddresses.end(); ++j)
  {
    dev = m_ipv4->GetNetDevice(m_ipv4->GetInterfaceForAddress (j->second.GetLocal()));
    if(dev->GetAddress() == toOrigin.GetOutputDevice()->GetAddress())
      j->first->SendTo(packet,0, InetSocketAddress(toOrigin.GetNextHop(), AODV_PORT));
  }

}

void 
RoutingProtocol::RecvError (Ptr<Packet> p)
{
  // TODO
}

void 
RoutingProtocol::BroadcastTimerExpire ()
{
  // id_purge();
  btimer.Schedule (BCAST_ID_SAVE);
}

void 
RoutingProtocol::HelloTimerExpire ()
{
  SendHello ();
  // TODO select random time for the next hello
  htimer.Schedule (HELLO_INTERVAL);
}

void 
RoutingProtocol::NeighborTimerExpire ()
{
  // nb_purge();
  ntimer.Schedule (HELLO_INTERVAL);
}

void 
RoutingProtocol::RouteCacheTimerExpire ()
{
  // rt_purge();
  rtimer.Schedule (FREQUENCY);
}

void 
RoutingProtocol::LocalRepairTimerExpire ()
{
  // TODO start local repair procedure
}

void
RoutingProtocol::SendHello ()
{
  // TODO send hello packet from interfaces
}


void
RoutingProtocol::SendReplyByIntermediateNode(RoutingTableEntry & toDst, RoutingTableEntry & toOrigin, bool gratRep)
{
#if 0
  RrepHeader rrepHeader;
  TypeHeader tHeader(AODVTYPE_RREP);

  rrepHeader.SetDst(toDst.GetDst());
  rrepHeader.SetOrigin(toOrigin.GetDst());
  rrepHeader.SetDstSeqno(toDst.GetSeqNo());
  rrepHeader.SetHopCount(toDst.GetHop());
  rrepHeader.SetLifeTime(toDst.GetLifeTime() - Simulator::Now());

  toDst.InsertPrecursor(toOrigin.GetNextHop());
  toOrigin.InsertPrecursor(toDst.GetNextHop());


  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader(rrepHeader);
  packet->AddHeader(tHeader);
  std::map< Ipv4Address, Ptr<Socket> >::const_iterator j = m_addressSocket.find(toOrigin.GetInterface());
  j->second->Send(packet);

  if(gratRep)
  {
    rrepHeader.SetHopCount(toOrigin.GetHop());
    rrepHeader.SetDst(toOrigin.GetDst());
    rrepHeader.SetDstSeqno(toOrigin.GetSeqNo());
    rrepHeader.SetOrigin(toDst.GetDst());
    rrepHeader.SetLifeTime(toOrigin.GetLifeTime() - Simulator::Now());

    Ptr<Packet> packetToDst = Create<Packet> ();
    packetToDst->AddHeader(rrepHeader);
    packetToDst->AddHeader(tHeader);
    j = m_addressSocket.find(toDst.GetInterface());
    j->second->Send(packet);
  }
#endif
}


void 
RoutingProtocol::SendError (Ipv4Address failed)
{
  // TODO
}

void 
RoutingProtocol::LocalRouteRepair (Ipv4Address dst, Ptr<Packet> p)
{
  // TODO local_rt_repair
}

void 
RoutingProtocol::HandleLinkFailure (Ipv4Address id)
{
  // TODO
}

void
RoutingProtocol::RtPurge ()
{
  // TODO AODV::rt_purge()
}
//-----------------------------------------------------------------------------
// TODO: NeighborList, BroadcastIdCache
//-----------------------------------------------------------------------------
}}
