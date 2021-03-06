#include "ns3/log.h"
#include "rsu-environment.h"
#include <cmath>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RsuRnvironment");
NS_OBJECT_ENSURE_REGISTERED (RsuEnv);

RsuEnv::RsuEnv ()
{
  NS_LOG_FUNCTION (this);
  // Opening interface with simulation script
  this->SetOpenGymInterface (OpenGymInterface::Get ());
  // Setting default values fot params
  m_vehicles = 0;
  m_max_vehicles = 25;
  m_alpha = 0.9;
  m_beta = 0.99;
  max_headway_time = 2.0;
  max_velocity_value = 50; // was 50
  desired_velocity_value = 45; // was 47
  old_reward = 0.0;
  current_reward = 0.0;
  current_step = 1;
  horizon = 128;
  epsilon_threshold = 1e-4;
  max_delta = 6.0;

  NS_LOG_INFO ("Set Up Interface : " << OpenGymInterface::Get () << "\n");
}

RsuEnv::~RsuEnv ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
RsuEnv::GetTypeId (void)
{
  static TypeId tid = TypeId ("RsuEnv")
                          .SetParent<OpenGymEnv> ()
                          .SetGroupName ("Applications")
                          .AddConstructor<RsuEnv> ();
  return tid;
}

void
RsuEnv::DoDispose ()
{
  NS_LOG_FUNCTION (this);
}


Ptr<OpenGymSpace>
RsuEnv::GetObservationSpace ()
{
  NS_LOG_FUNCTION (this);

  // set low and high values: low is for lowest speed/headway while high is for highest
  float low = 0.0;
  float high = max_velocity_value;

  // setting observation space shape which has a size of 2*numOfVehicles since it has headways and velocities for each vehicle
  std::vector<uint32_t> shape = {
      2 * m_max_vehicles,
  };
  std::string dtype = TypeNameGet<float> ();

  // initializing observation space
  Ptr<OpenGymBoxSpace> space = CreateObject<OpenGymBoxSpace> (low, high, shape, dtype);
  NS_LOG_UNCOND ("GetObservationSpace: " << space);
  return space;
}

Ptr<OpenGymSpace>
RsuEnv::GetActionSpace ()
{
  NS_LOG_FUNCTION (this);

  // set low and high values
  float low = -max_delta;
  float high = max_delta;

  // setting action space shape which has a size of numOfVehicles since actions are respective speeds for each vehicles
  std::vector<uint32_t> shape = {
      m_max_vehicles,
  };
  std::string dtype = TypeNameGet<float> ();

  // initializing action space
  Ptr<OpenGymBoxSpace> box = CreateObject<OpenGymBoxSpace> (low, high, shape, dtype);
  NS_LOG_INFO ("GetActionSpace: " << box);
  return box;
}

Ptr<OpenGymDataContainer>
RsuEnv::GetObservation ()
{
  NS_LOG_FUNCTION (this);

  // setting observation shape which has a size of 2*numOfVehicles since it has headways and velocities for each vehicle
  std::vector<uint32_t> shape = {
      2 * m_max_vehicles,
  };
  Ptr<OpenGymBoxContainer<float>> box = CreateObject<OpenGymBoxContainer<float>> (shape);

  // send zeros first time

  // Add Current headways of vehicles reachable by RSU to the observation
  for (uint32_t i = 0; i < actual_headways.size (); ++i)
    {
      float value = static_cast<float> (actual_headways[i]);
      box->AddValue (value);
    }

  // Add Current velocities of vehicles reachable by RSU to the observation
  for (uint32_t i = 0; i < actual_speeds.size (); ++i)
    {
      float value = static_cast<float> (actual_speeds[i]);
      box->AddValue (value);
    }

  NS_LOG_UNCOND ("MyGetObservation: " << box);
  return box;
}

float
RsuEnv::GetReward ()
{
  NS_LOG_FUNCTION (this);

  // The following formula is used to calculate the reward for the agent:
  // reward = beta * ( max_v - sum(|desired_v - v[i]|)/N - alpha * sum(max(max_h - h[i])))

  float reward = 0.0;
  double max_headway_summation = 0.0;
  for (uint32_t i = 0; i < actual_headways.size (); i++)
    {
      max_headway_summation += fmax (max_headway_time - actual_headways[i], 0.0);
    }
  double abs_speed_diff_summation = 0.0;
  for (uint32_t i = 0; i < actual_speeds.size (); i++)
    {
      abs_speed_diff_summation += abs (desired_velocity_value - actual_speeds[i]);
    }
  reward = max_velocity_value - (abs_speed_diff_summation / m_vehicles) -
           (max_headway_summation * m_alpha);

  current_reward = reward;
  if (current_step % horizon == 0)
    {
      old_reward = current_reward;
    }

  reward *= m_beta;

  NS_LOG_UNCOND ("MyGetReward: " << reward);
  return reward;
}

bool
RsuEnv::GetGameOver ()
{
  NS_LOG_FUNCTION (this);
  bool isGameOver = false;
  //	isGameOver = pow(abs(old_reward - current_reward), 2) < epsilon_threshold;
  NS_LOG_UNCOND ("MyGetGameOver: " << isGameOver);
  return isGameOver;
}

std::string
RsuEnv::GetExtraInfo ()
{
  NS_LOG_FUNCTION (this);
  std::string myInfo = "info";
  NS_LOG_UNCOND ("MyGetExtraInfo: " << myInfo);
  return myInfo;
}

bool
RsuEnv::ExecuteActions (Ptr<OpenGymDataContainer> action)
{
  NS_LOG_FUNCTION (this);

  // get the latest actions performed by the agent
  Ptr<OpenGymBoxContainer<float>> box = DynamicCast<OpenGymBoxContainer<float>> (action);

  // get new actions data (velocities)
  new_speeds = box->GetData ();

  // make sure all values are in the range [+]
  //	for (uint32_t i = 0; i < new_speeds.size(); i++) {
  //		new_speeds[i] = new_speeds[i] < 0 ? - fmod(abs(new_speeds[i]), max_delta) : fmod(abs(new_speeds[i]), max_delta);
  //	}

  current_step++;
  NS_LOG_UNCOND ("MyExecuteActions: " << action);
  return true;
}

std::vector<float>
RsuEnv::ExportNewSpeeds ()
{
  NS_LOG_FUNCTION (this);
  std::vector<float> new_speeds_no_paddings;
  // Remove unecessary paddings from new speeds
  for (uint32_t i = 0; i < m_vehicles; i++)
    {
      new_speeds_no_paddings.push_back (new_speeds[i]);
    }
  NS_LOG_INFO ("###################################################################################"
               "########################\n");
  return new_speeds_no_paddings;
}

void
RsuEnv::ImportSpeedsAndHeadWays (std::vector<double> RSU_headways, std::vector<double> RSU_speeds)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_INFO ("###################################################################################"
               "########################\n");

  // remove old headway and speed values
  actual_headways.clear ();
  actual_speeds.clear ();

  // get new speed and headway values from RSU
  actual_headways = RSU_headways;
  // pad zeros to eliminate missmatch with headways
  for (uint32_t i = actual_headways.size (); i < m_max_vehicles; i++)
    {
      actual_headways.push_back (0.0);
    }
  actual_speeds = RSU_speeds;
  // pad zeros to eliminate missmatch with speeds
  for (uint32_t i = actual_speeds.size (); i < m_max_vehicles; i++)
    {
      actual_speeds.push_back (0.0);
    }
  m_vehicles = actual_speeds.size ();
  Notify ();
}

uint32_t
RsuEnv::GetActionSpaceSize ()
{
  return m_max_vehicles;
}

} // namespace ns3