#ifndef __Pathfinding_hpp__
#define __Pathfinding_hpp__

#include "ff/FlowField.hpp"

#include "../world.h"

using namespace ai::ff;

const int c_worldSize = MAT_SIZE_CUBES;

typedef DefaultWorldConfig<c_worldSize,c_worldSize> Config;

typedef World<Config>               FFWorld;
typedef FlowFieldPathFinder<Config> FFPathFinder;
typedef FlowPath<Config>            FFPath;
typedef FlowPath_ptr<Config>        FFPath_ptr;

#endif //__Pathfinding_hpp__