/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2007-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    TraCIConstants.h
/// @author  Axel Wegener
/// @author  Friedemann Wesner
/// @author  Bjoern Hendriks
/// @author  Daniel Krajzewicz
/// @author  Thimor Bohn
/// @author  Tino Morenz
/// @author  Michael Behrisch
/// @author  Christoph Sommer
/// @author  Mario Krumnow
/// @author  Jakob Erdmann
/// @author  Laura Bieker
/// @date    2007/10/24
/// @version $Id$
///
// holds codes used for TraCI
/****************************************************************************/
#ifndef TRACICONSTANTS_H
#define TRACICONSTANTS_H

// ****************************************
// VERSION
// ****************************************
const int TRACI_VERSION = 19;

// ****************************************
// COMMANDS
// ****************************************
// command: get version
const int CMD_GETVERSION = 0x00;

// command: load
const int CMD_LOAD = 0x01;

// command: simulation step
const int CMD_SIMSTEP = 0x02;

// command: set connection priority (execution order)
const int CMD_SETORDER = 0x03;

// command: stop node
const int CMD_STOP = 0x12;

// command: reroute to parking area
const int CMD_REROUTE_TO_PARKING = 0xc2;

// command: Resume from parking
const int CMD_RESUME = 0x19;

// command: set lane
const int CMD_CHANGELANE = 0x13;

// command: slow down
const int CMD_SLOWDOWN = 0x14;

// command: set sublane (vehicle)
const int CMD_CHANGESUBLANE = 0x15;

// command: open gap
const int CMD_OPENGAP = 0x16;

// command: change target
const int CMD_CHANGETARGET = 0x31;

// command: close sumo
const int CMD_CLOSE = 0x7F;

// command: add subscription filter
const int CMD_ADD_SUBSCRIPTION_FILTER = 0x7e;


// command: subscribe induction loop (e1) context
const int CMD_SUBSCRIBE_INDUCTIONLOOP_CONTEXT = 0x80;
// response: subscribe induction loop (e1) context
const int RESPONSE_SUBSCRIBE_INDUCTIONLOOP_CONTEXT = 0x90;
// command: get induction loop (e1) variable
const int CMD_GET_INDUCTIONLOOP_VARIABLE = 0xa0;
// response: get induction loop (e1) variable
const int RESPONSE_GET_INDUCTIONLOOP_VARIABLE = 0xb0;
// command: subscribe induction loop (e1) variable
const int CMD_SUBSCRIBE_INDUCTIONLOOP_VARIABLE = 0xd0;
// response: subscribe induction loop (e1) variable
const int RESPONSE_SUBSCRIBE_INDUCTIONLOOP_VARIABLE = 0xe0;

// command: subscribe multi-entry/multi-exit detector (e3) context
const int CMD_SUBSCRIBE_MULTIENTRYEXIT_CONTEXT = 0x81;
// response: subscribe multi-entry/multi-exit detector (e3) context
const int RESPONSE_SUBSCRIBE_MULTIENTRYEXIT_CONTEXT = 0x91;
// command: get multi-entry/multi-exit detector (e3) variable
const int CMD_GET_MULTIENTRYEXIT_VARIABLE = 0xa1;
// response: get multi-entry/multi-exit detector (e3) variable
const int RESPONSE_GET_MULTIENTRYEXIT_VARIABLE = 0xb1;
// command: subscribe multi-entry/multi-exit detector (e3) variable
const int CMD_SUBSCRIBE_MULTIENTRYEXIT_VARIABLE = 0xd1;
// response: subscribe multi-entry/multi-exit detector (e3) variable
const int RESPONSE_SUBSCRIBE_MULTIENTRYEXIT_VARIABLE = 0xe1;

// command: subscribe traffic lights context
const int CMD_SUBSCRIBE_TL_CONTEXT = 0x82;
// response: subscribe traffic lights context
const int RESPONSE_SUBSCRIBE_TL_CONTEXT = 0x92;
// command: get traffic lights variable
const int CMD_GET_TL_VARIABLE = 0xa2;
// response: get traffic lights variable
const int RESPONSE_GET_TL_VARIABLE = 0xb2;
// command: set traffic lights variable
const int CMD_SET_TL_VARIABLE = 0xc2;
// command: subscribe traffic lights variable
const int CMD_SUBSCRIBE_TL_VARIABLE = 0xd2;
// response: subscribe traffic lights variable
const int RESPONSE_SUBSCRIBE_TL_VARIABLE = 0xe2;

// command: subscribe lane context
const int CMD_SUBSCRIBE_LANE_CONTEXT = 0x83;
// response: subscribe lane context
const int RESPONSE_SUBSCRIBE_LANE_CONTEXT = 0x93;
// command: get lane variable
const int CMD_GET_LANE_VARIABLE = 0xa3;
// response: get lane variable
const int RESPONSE_GET_LANE_VARIABLE = 0xb3;
// command: set lane variable
const int CMD_SET_LANE_VARIABLE = 0xc3;
// command: subscribe lane variable
const int CMD_SUBSCRIBE_LANE_VARIABLE = 0xd3;
// response: subscribe lane variable
const int RESPONSE_SUBSCRIBE_LANE_VARIABLE = 0xe3;

// command: subscribe vehicle context
const int CMD_SUBSCRIBE_VEHICLE_CONTEXT = 0x84;
// response: subscribe vehicle context
const int RESPONSE_SUBSCRIBE_VEHICLE_CONTEXT = 0x94;
// command: get vehicle variable
const int CMD_GET_VEHICLE_VARIABLE = 0xa4;
// response: get vehicle variable
const int RESPONSE_GET_VEHICLE_VARIABLE = 0xb4;
// command: set vehicle variable
const int CMD_SET_VEHICLE_VARIABLE = 0xc4;
// command: subscribe vehicle variable
const int CMD_SUBSCRIBE_VEHICLE_VARIABLE = 0xd4;
// response: subscribe vehicle variable
const int RESPONSE_SUBSCRIBE_VEHICLE_VARIABLE = 0xe4;

// command: subscribe vehicle type context
const int CMD_SUBSCRIBE_VEHICLETYPE_CONTEXT = 0x85;
// response: subscribe vehicle type context
const int RESPONSE_SUBSCRIBE_VEHICLETYPE_CONTEXT = 0x95;
// command: get vehicle type variable
const int CMD_GET_VEHICLETYPE_VARIABLE = 0xa5;
// response: get vehicle type variable
const int RESPONSE_GET_VEHICLETYPE_VARIABLE = 0xb5;
// command: set vehicle type variable
const int CMD_SET_VEHICLETYPE_VARIABLE = 0xc5;
// command: subscribe vehicle type variable
const int CMD_SUBSCRIBE_VEHICLETYPE_VARIABLE = 0xd5;
// response: subscribe vehicle type variable
const int RESPONSE_SUBSCRIBE_VEHICLETYPE_VARIABLE = 0xe5;

// command: subscribe route context
const int CMD_SUBSCRIBE_ROUTE_CONTEXT = 0x86;
// response: subscribe route context
const int RESPONSE_SUBSCRIBE_ROUTE_CONTEXT = 0x96;
// command: get route variable
const int CMD_GET_ROUTE_VARIABLE = 0xa6;
// response: get route variable
const int RESPONSE_GET_ROUTE_VARIABLE = 0xb6;
// command: set route variable
const int CMD_SET_ROUTE_VARIABLE = 0xc6;
// command: subscribe route variable
const int CMD_SUBSCRIBE_ROUTE_VARIABLE = 0xd6;
// response: subscribe route variable
const int RESPONSE_SUBSCRIBE_ROUTE_VARIABLE = 0xe6;

// command: subscribe poi context
const int CMD_SUBSCRIBE_POI_CONTEXT = 0x87;
// response: subscribe poi context
const int RESPONSE_SUBSCRIBE_POI_CONTEXT = 0x97;
// command: get poi variable
const int CMD_GET_POI_VARIABLE = 0xa7;
// response: get poi variable
const int RESPONSE_GET_POI_VARIABLE = 0xb7;
// command: set poi variable
const int CMD_SET_POI_VARIABLE = 0xc7;
// command: subscribe poi variable
const int CMD_SUBSCRIBE_POI_VARIABLE = 0xd7;
// response: subscribe poi variable
const int RESPONSE_SUBSCRIBE_POI_VARIABLE = 0xe7;

// command: subscribe polygon context
const int CMD_SUBSCRIBE_POLYGON_CONTEXT = 0x88;
// response: subscribe polygon context
const int RESPONSE_SUBSCRIBE_POLYGON_CONTEXT = 0x98;
// command: get polygon variable
const int CMD_GET_POLYGON_VARIABLE = 0xa8;
// response: get polygon variable
const int RESPONSE_GET_POLYGON_VARIABLE = 0xb8;
// command: set polygon variable
const int CMD_SET_POLYGON_VARIABLE = 0xc8;
// command: subscribe polygon variable
const int CMD_SUBSCRIBE_POLYGON_VARIABLE = 0xd8;
// response: subscribe polygon variable
const int RESPONSE_SUBSCRIBE_POLYGON_VARIABLE = 0xe8;

// command: subscribe junction context
const int CMD_SUBSCRIBE_JUNCTION_CONTEXT = 0x89;
// response: subscribe junction context
const int RESPONSE_SUBSCRIBE_JUNCTION_CONTEXT = 0x99;
// command: get junction variable
const int CMD_GET_JUNCTION_VARIABLE = 0xa9;
// response: get junction variable
const int RESPONSE_GET_JUNCTION_VARIABLE = 0xb9;
// command: set junction variable
const int CMD_SET_JUNCTION_VARIABLE = 0xc9;
// command: subscribe junction variable
const int CMD_SUBSCRIBE_JUNCTION_VARIABLE = 0xd9;
// response: subscribe junction variable
const int RESPONSE_SUBSCRIBE_JUNCTION_VARIABLE = 0xe9;

// command: subscribe edge context
const int CMD_SUBSCRIBE_EDGE_CONTEXT = 0x8a;
// response: subscribe edge context
const int RESPONSE_SUBSCRIBE_EDGE_CONTEXT = 0x9a;
// command: get edge variable
const int CMD_GET_EDGE_VARIABLE = 0xaa;
// response: get edge variable
const int RESPONSE_GET_EDGE_VARIABLE = 0xba;
// command: set edge variable
const int CMD_SET_EDGE_VARIABLE = 0xca;
// command: subscribe edge variable
const int CMD_SUBSCRIBE_EDGE_VARIABLE = 0xda;
// response: subscribe edge variable
const int RESPONSE_SUBSCRIBE_EDGE_VARIABLE = 0xea;

// command: subscribe simulation context
const int CMD_SUBSCRIBE_SIM_CONTEXT = 0x8b;
// response: subscribe simulation context
const int RESPONSE_SUBSCRIBE_SIM_CONTEXT = 0x9b;
// command: get simulation variable
const int CMD_GET_SIM_VARIABLE = 0xab;
// response: get simulation variable
const int RESPONSE_GET_SIM_VARIABLE = 0xbb;
// command: set simulation variable
const int CMD_SET_SIM_VARIABLE = 0xcb;
// command: subscribe simulation variable
const int CMD_SUBSCRIBE_SIM_VARIABLE = 0xdb;
// response: subscribe simulation variable
const int RESPONSE_SUBSCRIBE_SIM_VARIABLE = 0xeb;

// command: subscribe GUI context
const int CMD_SUBSCRIBE_GUI_CONTEXT = 0x8c;
// response: subscribe GUI context
const int RESPONSE_SUBSCRIBE_GUI_CONTEXT = 0x9c;
// command: get GUI variable
const int CMD_GET_GUI_VARIABLE = 0xac;
// response: get GUI variable
const int RESPONSE_GET_GUI_VARIABLE = 0xbc;
// command: set GUI variable
const int CMD_SET_GUI_VARIABLE = 0xcc;
// command: subscribe GUI variable
const int CMD_SUBSCRIBE_GUI_VARIABLE = 0xdc;
// response: subscribe GUI variable
const int RESPONSE_SUBSCRIBE_GUI_VARIABLE = 0xec;

// command: subscribe areal detector (e2) context
const int CMD_SUBSCRIBE_LANEAREA_CONTEXT = 0x8d;
// response: subscribe areal detector (e2) context
const int RESPONSE_SUBSCRIBE_LANEAREA_CONTEXT = 0x9d;
// command: get areal detector (e2) variable
const int CMD_GET_LANEAREA_VARIABLE = 0xad;
// response: get areal detector (e2) variable
const int RESPONSE_GET_LANEAREA_VARIABLE = 0xbd;
// command: subscribe areal detector (e2) variable
const int CMD_SUBSCRIBE_LANEAREA_VARIABLE = 0xdd;
// response: subscribe areal detector (e2) variable
const int RESPONSE_SUBSCRIBE_LANEAREA_VARIABLE = 0xed;

// command: subscribe person context
const int CMD_SUBSCRIBE_PERSON_CONTEXT = 0x8e;
// response: subscribe person context
const int RESPONSE_SUBSCRIBE_PERSON_CONTEXT = 0x9e;
// command: get person variable
const int CMD_GET_PERSON_VARIABLE = 0xae;
// response: get person variable
const int RESPONSE_GET_PERSON_VARIABLE = 0xbe;
// command: set person variable
const int CMD_SET_PERSON_VARIABLE = 0xce;
// command: subscribe person variable
const int CMD_SUBSCRIBE_PERSON_VARIABLE = 0xde;
// response: subscribe person variable
const int RESPONSE_SUBSCRIBE_PERSON_VARIABLE = 0xee;


// ****************************************
// POSITION REPRESENTATIONS
// ****************************************
// Position in geo-coordinates
const int POSITION_LON_LAT = 0x00;
// 2D cartesian coordinates
const int POSITION_2D = 0x01;
// Position in geo-coordinates with altitude
const int POSITION_LON_LAT_ALT = 0x02;
// 3D cartesian coordinates
const int POSITION_3D = 0x03;
// Position on road map
const int POSITION_ROADMAP = 0x04;


// ****************************************
// DATA TYPES
// ****************************************
// Polygon (2*n doubles)
const int TYPE_POLYGON = 0x06;
// unsigned byte
const int TYPE_UBYTE = 0x07;
// signed byte
const int TYPE_BYTE = 0x08;
// 32 bit signed integer
const int TYPE_INTEGER = 0x09;
// double
const int TYPE_DOUBLE = 0x0B;
// 8 bit ASCII string
const int TYPE_STRING = 0x0C;
// list of strings
const int TYPE_STRINGLIST = 0x0E;
// compound object
const int TYPE_COMPOUND = 0x0F;
// color (four ubytes)
const int TYPE_COLOR = 0x11;


// ****************************************
// RESULT TYPES
// ****************************************
// result type: Ok
const int RTYPE_OK = 0x00; 
// result type: not implemented
const int RTYPE_NOTIMPLEMENTED = 0x01;
// result type: error
const int RTYPE_ERR = 0xFF;

// ****************************************
// special return or parameter values
// ****************************************
// return value for invalid queries (especially vehicle is not on the road), see Position::INVALID
const int INVALID_DOUBLE_VALUE = -1073741824;
// return value for invalid queries (especially vehicle is not on the road), see Position::INVALID
const int INVALID_INT_VALUE = -1073741824;
// maximum value for client ordering (2 ^ 30)
const int MAX_ORDER = 1073741824;


// ****************************************
// DIFFERENT DISTANCE REQUESTS
// ****************************************
// air distance
const int REQUEST_AIRDIST = 0x00;
// driving distance
const int REQUEST_DRIVINGDIST = 0x01;


// ****************************************
// VEHICLE REMOVAL REASONS
// ****************************************
// vehicle started teleport
const int REMOVE_TELEPORT = 0x00;
// vehicle removed while parking
const int REMOVE_PARKING = 0x01;
// vehicle arrived
const int REMOVE_ARRIVED = 0x02;
// vehicle was vaporized
const int REMOVE_VAPORIZED = 0x03;
// vehicle finished route during teleport
const int REMOVE_TELEPORT_ARRIVED = 0x04;

// ****************************************
// PERSON/CONTAINER STAGES
// ****************************************
// person / container stopping
const int STAGE_WAITING_FOR_DEPART = 0x00;
// person / container stopping
const int STAGE_WAITING = 0x01;
// person walking / container transhiping
const int STAGE_WALKING = 0x02;
// person riding / container being transported
const int STAGE_DRIVING = 0x03;

// ****************************************
// Stop Flags
// ****************************************
const int STOP_DEFAULT = 0x00;
const int STOP_PARKING = 0x01;
const int STOP_TRIGGERED = 0x02;
const int STOP_CONTAINER_TRIGGERED = 0x04;
const int STOP_BUS_STOP = 0x08;
const int STOP_CONTAINER_STOP = 0x10;
const int STOP_CHARGING_STATION = 0x20;
const int STOP_PARKING_AREA = 0x40;

// ****************************************
// Departure Flags
// ****************************************
const int DEPARTFLAG_TRIGGERED = -0x01;
const int DEPARTFLAG_CONTAINER_TRIGGERED = -0x02;
const int DEPARTFLAG_NOW = -0x03;

const int DEPARTFLAG_SPEED_RANDOM = -0x02;
const int DEPARTFLAG_SPEED_MAX = -0x03;

const int DEPARTFLAG_LANE_RANDOM = -0x02;
const int DEPARTFLAG_LANE_FREE = -0x03;
const int DEPARTFLAG_LANE_ALLOWED_FREE = -0x04;
const int DEPARTFLAG_LANE_BEST_FREE = -0x05;
const int DEPARTFLAG_LANE_FIRST_ALLOWED = -0x06;

const int DEPARTFLAG_POS_RANDOM = -0x02;
const int DEPARTFLAG_POS_FREE = -0x03;
const int DEPARTFLAG_POS_BASE = -0x04;
const int DEPARTFLAG_POS_LAST = -0x05;
const int DEPARTFLAG_POS_RANDOM_FREE = -0x06;

const int ARRIVALFLAG_LANE_CURRENT = -0x02;
const int ARRIVALFLAG_SPEED_CURRENT = -0x02;

const int ARRIVALFLAG_POS_RANDOM = -0x02;
const int ARRIVALFLAG_POS_MAX = -0x03;

// ****************************************
// Routing modes
// ****************************************
// use custom weights if available, fall back to loaded weights and then to free-flow speed
const int ROUTING_MODE_DEFAULT = 0x00;
// use aggregated travel times from device.rerouting
const int ROUTING_MODE_AGGREGATED = 0x01;
// use loaded efforts
const int ROUTING_MODE_EFFORT = 0x02;
// use combined costs
const int ROUTING_MODE_COMBINED = 0x03;

// ****************************************
// FILTER TYPES (for context subscription filters)
// ****************************************

// Reset all filters
const int FILTER_TYPE_NONE = 0x00;

// Filter by list of lanes relative to ego vehicle
const int FILTER_TYPE_LANES = 0x01;

// Exclude vehicles on opposite (and other) lanes from context subscription result
const int FILTER_TYPE_NOOPPOSITE = 0x02;

// Specify maximal downstream distance for vehicles in context subscription result
const int FILTER_TYPE_DOWNSTREAM_DIST = 0x03;

// Specify maximal upstream distance for vehicles in context subscription result
const int FILTER_TYPE_UPSTREAM_DIST = 0x04;

// Only return leader and follower on the specified lanes in context subscription result
const int FILTER_TYPE_LEAD_FOLLOW = 0x05;

// Only return foes on upcoming junction in context subscription result
const int FILTER_TYPE_TURN = 0x07;

// Only return vehicles of the given vClass in context subscription result
const int FILTER_TYPE_VCLASS = 0x08;

// Only return vehicles of the given vType in context subscription result
const int FILTER_TYPE_VTYPE = 0x09;





// ****************************************
// VARIABLE TYPES (for CMD_GET_*_VARIABLE)
// ****************************************
// list of instances' ids (get: all)
const int TRACI_ID_LIST = 0x00;

// count of instances (get: all)
const int ID_COUNT = 0x01;

// subscribe object variables (get: all)
const int AUTOMATIC_VARIABLES_SUBSCRIPTION = 0x02;

// subscribe context variables (get: all)
const int AUTOMATIC_CONTEXT_SUBSCRIPTION = 0x03;

// generic attributes (get/set: all)
const int GENERIC_ATTRIBUTE = 0x03;

// last step vehicle number (get: induction loops, multi-entry/multi-exit detector, lanes, edges)
const int LAST_STEP_VEHICLE_NUMBER = 0x10;

// last step vehicle number (get: induction loops, multi-entry/multi-exit detector, lanes, edges)
const int LAST_STEP_MEAN_SPEED = 0x11;

// last step vehicle list (get: induction loops, multi-entry/multi-exit detector, lanes, edges)
const int LAST_STEP_VEHICLE_ID_LIST = 0x12;

// last step occupancy (get: induction loops, lanes, edges)
const int LAST_STEP_OCCUPANCY = 0x13;

// last step vehicle halting number (get: multi-entry/multi-exit detector, lanes, edges)
const int LAST_STEP_VEHICLE_HALTING_NUMBER = 0x14;

// last step mean vehicle length (get: induction loops, lanes, edges)
const int LAST_STEP_LENGTH = 0x15;

// last step time since last detection (get: induction loops)
const int LAST_STEP_TIME_SINCE_DETECTION = 0x16;

// entry times
const int LAST_STEP_VEHICLE_DATA = 0x17;

// last step jam length in vehicles
const int JAM_LENGTH_VEHICLE = 0x18;

// last step jam length in meters
const int JAM_LENGTH_METERS = 0x19;

// last step person list (get: edges, vehicles)
const int LAST_STEP_PERSON_ID_LIST = 0x1a;

// full name (get: edges, simulation)
const int VAR_NAME = 0x1b;

// traffic light states, encoded as rRgGyYoO tuple (get: traffic lights)
const int TL_RED_YELLOW_GREEN_STATE = 0x20;

// index of the phase (set: traffic lights)
const int TL_PHASE_INDEX = 0x22;

// traffic light program (set: traffic lights)
const int TL_PROGRAM = 0x23;

// phase duration (set: traffic lights)
const int TL_PHASE_DURATION = 0x24;

// controlled lanes (get: traffic lights)
const int TL_CONTROLLED_LANES = 0x26;

// controlled links (get: traffic lights)
const int TL_CONTROLLED_LINKS = 0x27;

// index of the current phase (get: traffic lights)
const int TL_CURRENT_PHASE = 0x28;

// name of the current program (get: traffic lights)
const int TL_CURRENT_PROGRAM = 0x29;

// controlled junctions (get: traffic lights)
const int TL_CONTROLLED_JUNCTIONS = 0x2a;

// complete definition (get: traffic lights)
const int TL_COMPLETE_DEFINITION_RYG = 0x2b;

// complete program (set: traffic lights)
const int TL_COMPLETE_PROGRAM_RYG = 0x2c;

// assumed time to next switch (get: traffic lights)
const int TL_NEXT_SWITCH = 0x2d;

// current state, using external signal names (get: traffic lights)
const int TL_EXTERNAL_STATE = 0x2e;

// outgoing link number (get: lanes)
const int LANE_LINK_NUMBER = 0x30;

// id of parent edge (get: lanes)
const int LANE_EDGE_ID = 0x31;

// outgoing link definitions (get: lanes)
const int LANE_LINKS = 0x33;

// list of allowed vehicle classes (get&set: lanes)
const int LANE_ALLOWED = 0x34;

// list of not allowed vehicle classes (get&set: lanes)
const int LANE_DISALLOWED = 0x35;

// list of foe lanes (get: lanes)
const int VAR_FOES = 0x37;

// slope (get: edge, lane, vehicle, person)
const int VAR_SLOPE = 0x36;

// speed (get: vehicle)
const int VAR_SPEED = 0x40;

// maximum allowed/possible speed (get: vehicle types, lanes, set: edges, lanes)
const int VAR_MAXSPEED = 0x41;

// position (2D) (get: vehicle, poi, inductionloop, areadetector; set: poi)
const int VAR_POSITION = 0x42;

// position (3D) (get: vehicle, poi, set: poi)
const int VAR_POSITION3D = 0x39;

// angle (get: vehicle)
const int VAR_ANGLE = 0x43;

// angle (get: vehicle types, lanes, arealdetector, set: lanes)
const int VAR_LENGTH = 0x44;

// color (get: vehicles, vehicle types, polygons, pois)
const int VAR_COLOR = 0x45;

// max. acceleration (get: vehicles, vehicle types)
const int VAR_ACCEL = 0x46;

// max. comfortable deceleration (get: vehicles, vehicle types)
const int VAR_DECEL = 0x47;

// max. (physically possible) deceleration (get: vehicles, vehicle types)
const int VAR_EMERGENCY_DECEL = 0x7b;

// apparent deceleration (get: vehicles, vehicle types)
const int VAR_APPARENT_DECEL = 0x7c;

// action step length (get: vehicles, vehicle types)
const int VAR_ACTIONSTEPLENGTH = 0x7d;

// last action time (get: vehicles)
const int VAR_LASTACTIONTIME = 0x7f;

// driver's desired headway (get: vehicle types)
const int VAR_TAU = 0x48;

// vehicle class (get: vehicle types)
const int VAR_VEHICLECLASS = 0x49;

// emission class (get: vehicle types)
const int VAR_EMISSIONCLASS = 0x4a;

// shape class (get: vehicle types)
const int VAR_SHAPECLASS = 0x4b;

// minimum gap (get: vehicle types)
const int VAR_MINGAP = 0x4c;

// width (get: vehicle types, lanes, polygons)
const int VAR_WIDTH = 0x4d;

// shape (get: polygons)
const int VAR_SHAPE = 0x4e;

// type id (get: vehicles, polygons, pois)
const int VAR_TYPE = 0x4f;

// road id (get: vehicles)
const int VAR_ROAD_ID = 0x50;

// lane id (get: vehicles, inductionloop, arealdetector)
const int VAR_LANE_ID = 0x51;

// lane index (get: vehicle, edge)
const int VAR_LANE_INDEX = 0x52;

// route id (get & set: vehicles)
const int VAR_ROUTE_ID = 0x53;

// edges (get: routes, vehicles)
const int VAR_EDGES = 0x54;

// update bestLanes (set: vehicle)
const int VAR_UPDATE_BESTLANES = 0x6a;

// filled? (get: polygons)
const int VAR_FILL = 0x55;

// position (1D along lane) (get: vehicle)
const int VAR_LANEPOSITION = 0x56;

// route (set: vehicles)
const int VAR_ROUTE = 0x57;

// travel time information (get&set: vehicle)
const int VAR_EDGE_TRAVELTIME = 0x58;

// effort information (get&set: vehicle)
const int VAR_EDGE_EFFORT = 0x59;

// last step travel time (get: edge, lane)
const int VAR_CURRENT_TRAVELTIME = 0x5a;

// signals state (get/set: vehicle)
const int VAR_SIGNALS = 0x5b;

// new lane/position along (set: vehicle)
const int VAR_MOVE_TO = 0x5c;

// driver imperfection (set: vehicle)
const int VAR_IMPERFECTION = 0x5d;

// speed factor (set: vehicle)
const int VAR_SPEED_FACTOR = 0x5e;

// speed deviation (set: vehicle)
const int VAR_SPEED_DEVIATION = 0x5f;

// routing mode (get/set: vehicle)
const int VAR_ROUTING_MODE = 0x89;

// speed without TraCI influence (get: vehicle)
const int VAR_SPEED_WITHOUT_TRACI = 0xb1;

// best lanes (get: vehicle)
const int VAR_BEST_LANES = 0xb2;

// how speed is set (set: vehicle)
const int VAR_SPEEDSETMODE = 0xb3;

// move vehicle to explicit (remote controlled) position (set: vehicle)
const int MOVE_TO_XY = 0xb4;

// is the vehicle stopped, and if so parked and/or triggered?
// value = stopped + 2 * parking + 4 * triggered
const int VAR_STOPSTATE = 0xb5;

// how lane changing is performed (get/set: vehicle)
const int VAR_LANECHANGE_MODE = 0xb6;

// maximum speed regarding max speed on the current lane and speed factor (get: vehicle)
const int VAR_ALLOWED_SPEED = 0xb7;

// position (1D lateral position relative to center of the current lane) (get: vehicle)
const int VAR_LANEPOSITION_LAT = 0xb8;

// get/set prefered lateral alignment within the lane (vehicle)
const int VAR_LATALIGNMENT = 0xb9;

// get/set maximum lateral speed (vehicle, vtypes)
const int VAR_MAXSPEED_LAT = 0xba;

// get/set minimum lateral gap (vehicle, vtypes)
const int VAR_MINGAP_LAT = 0xbb;

// get/set vehicle height (vehicle, vtypes)
const int VAR_HEIGHT = 0xbc;

// get/set vehicle line
const int VAR_LINE = 0xbd;

// get/set vehicle via
const int VAR_VIA = 0xbe;

// current CO2 emission of a node (get: vehicle, lane, edge)
const int VAR_CO2EMISSION = 0x60;

// current CO emission of a node (get: vehicle, lane, edge)
const int VAR_COEMISSION = 0x61;

// current HC emission of a node (get: vehicle, lane, edge)
const int VAR_HCEMISSION = 0x62;

// current PMx emission of a node (get: vehicle, lane, edge)
const int VAR_PMXEMISSION = 0x63;

// current NOx emission of a node (get: vehicle, lane, edge)
const int VAR_NOXEMISSION = 0x64;

// current fuel consumption of a node (get: vehicle, lane, edge)
const int VAR_FUELCONSUMPTION = 0x65;

// current noise emission of a node (get: vehicle, lane, edge)
const int VAR_NOISEEMISSION = 0x66;

// current person number (get: vehicle)
const int VAR_PERSON_NUMBER = 0x67;

// number of persons waiting at a defined bus stop (get: simulation)
const int VAR_BUS_STOP_WAITING = 0x67;

// current leader together with gap (get: vehicle)
const int VAR_LEADER = 0x68;

// edge index in current route (get: vehicle)
const int VAR_ROUTE_INDEX = 0x69;

// current waiting time (get: vehicle, lane)
const int VAR_WAITING_TIME = 0x7a;

// current waiting time (get: vehicle)
const int VAR_ACCUMULATED_WAITING_TIME = 0x87;

// upcoming traffic lights (get: vehicle)
const int VAR_NEXT_TLS = 0x70;

// upcoming stops (get: vehicle)
const int VAR_NEXT_STOPS = 0x73;

// current acceleration (get: vehicle)
const int VAR_ACCELERATION = 0x72;

// current time in seconds (get: simulation)
const int VAR_TIME = 0x66;

// current time step (get: simulation)
const int VAR_TIME_STEP = 0x70;

// current electricity consumption of a node (get: vehicle, lane, edge)
const int VAR_ELECTRICITYCONSUMPTION = 0x71;

// number of loaded vehicles (get: simulation)
const int VAR_LOADED_VEHICLES_NUMBER = 0x71;

// loaded vehicle ids (get: simulation)
const int VAR_LOADED_VEHICLES_IDS = 0x72;

// number of departed vehicle (get: simulation)
const int VAR_DEPARTED_VEHICLES_NUMBER = 0x73;

// departed vehicle ids (get: simulation)
const int VAR_DEPARTED_VEHICLES_IDS = 0x74;

// number of vehicles starting to teleport (get: simulation)
const int VAR_TELEPORT_STARTING_VEHICLES_NUMBER = 0x75;

// ids of vehicles starting to teleport (get: simulation)
const int VAR_TELEPORT_STARTING_VEHICLES_IDS = 0x76;

// number of vehicles ending to teleport (get: simulation)
const int VAR_TELEPORT_ENDING_VEHICLES_NUMBER = 0x77;

// ids of vehicles ending to teleport (get: simulation)
const int VAR_TELEPORT_ENDING_VEHICLES_IDS = 0x78;

// number of arrived vehicles (get: simulation)
const int VAR_ARRIVED_VEHICLES_NUMBER = 0x79;

// ids of arrived vehicles (get: simulation)
const int VAR_ARRIVED_VEHICLES_IDS = 0x7a;

// delta t (get: simulation)
const int VAR_DELTA_T = 0x7b;

// bounding box (get: simulation)
const int VAR_NET_BOUNDING_BOX = 0x7c;

// minimum number of expected vehicles (get: simulation)
const int VAR_MIN_EXPECTED_VEHICLES = 0x7d;

// number of vehicles starting to park (get: simulation)
const int VAR_STOP_STARTING_VEHICLES_NUMBER = 0x68;

// ids of vehicles starting to park (get: simulation)
const int VAR_STOP_STARTING_VEHICLES_IDS = 0x69;

// number of vehicles ending to park (get: simulation)
const int VAR_STOP_ENDING_VEHICLES_NUMBER = 0x6a;

// ids of vehicles ending to park (get: simulation)
const int VAR_STOP_ENDING_VEHICLES_IDS = 0x6b;

// number of vehicles starting to park (get: simulation)
const int VAR_PARKING_STARTING_VEHICLES_NUMBER = 0x6c;

// ids of vehicles starting to park (get: simulation)
const int VAR_PARKING_STARTING_VEHICLES_IDS = 0x6d;

// number of vehicles ending to park (get: simulation)
const int VAR_PARKING_ENDING_VEHICLES_NUMBER = 0x6e;

// ids of vehicles ending to park (get: simulation)
const int VAR_PARKING_ENDING_VEHICLES_IDS = 0x6f;

// number of vehicles involved in a collision (get: simulation)
const int VAR_COLLIDING_VEHICLES_NUMBER = 0x80;

// ids of vehicles involved in a collision (get: simulation)
const int VAR_COLLIDING_VEHICLES_IDS = 0x81;

// number of vehicles involved in a collision (get: simulation)
const int VAR_EMERGENCYSTOPPING_VEHICLES_NUMBER = 0x89;

// ids of vehicles involved in a collision (get: simulation)
const int VAR_EMERGENCYSTOPPING_VEHICLES_IDS = 0x8a;

// clears the simulation of all not inserted vehicles (set: simulation)
const int CMD_CLEAR_PENDING_VEHICLES = 0x94;

// triggers saving simulation state (set: simulation)
const int CMD_SAVE_SIMSTATE = 0x95;

// sets/retrieves abstract parameter
const int VAR_PARAMETER = 0x7e;


// add an instance (poi, polygon, vehicle, person, route)
const int ADD = 0x80;

// remove an instance (poi, polygon, vehicle, person)
const int REMOVE = 0x81;

// copy an instance (vehicle type, other TBD.)
const int COPY = 0x88;

// convert coordinates
const int POSITION_CONVERSION = 0x82;

// distance between points or vehicles
const int DISTANCE_REQUEST = 0x83;

// the current driving distance
const int VAR_DISTANCE = 0x84;

// add a fully specified instance (vehicle)
const int ADD_FULL = 0x85;

// find a car based route
const int FIND_ROUTE = 0x86;

// find an intermodal route
const int FIND_INTERMODAL_ROUTE = 0x87;

// force rerouting based on travel time (vehicles)
const int CMD_REROUTE_TRAVELTIME = 0x90;

// force rerouting based on effort (vehicles)
const int CMD_REROUTE_EFFORT = 0x91;

// validates current route (vehicles)
const int VAR_ROUTE_VALID = 0x92;

// retrieve information regarding the current person/container stage
const int VAR_STAGE = 0xc0;

// retrieve information regarding the next edge including crossings and walkingAreas (pedestrians only)
const int VAR_NEXT_EDGE = 0xc1;

// retrieve information regarding the number of remaining stages
const int VAR_STAGES_REMAINING = 0xc2;

// retrieve the current vehicle id for the driving stage (person, container)
const int VAR_VEHICLE = 0xc3;

// append a person stage (person)
const int APPEND_STAGE = 0xc4;

// append a person stage (person)
const int REMOVE_STAGE = 0xc5;

// zoom
const int VAR_VIEW_ZOOM = 0xa0;

// view position
const int VAR_VIEW_OFFSET = 0xa1;

// view schema
const int VAR_VIEW_SCHEMA = 0xa2;

// view by boundary
const int VAR_VIEW_BOUNDARY = 0xa3;

// screenshot
const int VAR_SCREENSHOT = 0xa5;

// track vehicle
const int VAR_TRACK_VEHICLE = 0xa6;

// presence of view
const int VAR_HAS_VIEW = 0xa7;


#endif
