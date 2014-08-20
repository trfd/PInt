///
///  FFPortalPathSearch.hpp
///
///  Created by Baptiste Dupy on 15/08/2014.
///  Contact:
///          Mail:       <baptiste.dupy@gmail.com>
///          GitHub:     trfd <https://github.com/trfd>
///          Website:    <http://www.trf-d.com>
///
///  Copyright (C) 2014 Baptiste (trfd) Dupy
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
///
///

#ifndef AI_FF_PortalPathSearch_hpp
#define AI_FF_PortalPathSearch_hpp

#include <map>

#include "ai/DtAdjacencyList.hpp"

#include "ASPathFinder.hpp"
#include "FFTypes.hpp"
#include "FFChunk.hpp"
#include "FFGrid.hpp"
#include "FFPortal.hpp"
#include "FFCostChunkGraph.hpp"
#include "FFPortalGraph.hpp"

#define __USE_SAFE_PORTAL_INSERT__
#define __ENABLE_DEBUG_DRAW__

namespace ai
{
    namespace ff
    {
        
            class PortalPathSearch
            {   
                PathRequest* pathRequest;
                
                ChunkID fromChunk;
                ChunkID toChunk;
                
                std::vector<PortalPath> candidates;

                ASPathFinder pathfinder;

                template<typename... Args_>
                PortalPathSearch(PortalPathSearch<_Config>* m_world,PathRequest* pathReq_,ChunkID from_,ChunkID to_,Args_&&... args_)
                : pathRequest(pathReq_), 
                fromChunk(from_), toChunk(to_),
                pathfinder(std::forward<Args_>(args_)...)
                {
                    std::vector<FrontierID> fromFrontiers = Utils::allFrontiers(from_);
                    std::vector<FrontierID> toFrontiers = Utils::allFrontiers(from_);

                    std::vector<Portal_ptr> fromPortals;
                    std::vector<Portal_ptr> toPortals;

                }


            };
          }