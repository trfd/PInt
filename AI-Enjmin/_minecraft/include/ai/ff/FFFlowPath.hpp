       ///
///  FFFlowPath.hpp
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

#ifndef AI_FF_FlowPath_hpp
#define AI_FF_FlowPath_hpp

#include "FFUtils.hpp"
#include "FFFlowTile.hpp"

namespace ai
{
    namespace ff
    {
       template<typename _Config>
       class FlowPath
       {
         
       public:
        
           typedef Utils<_Config>        Utils;
           typedef FlowTile<_Config>     FlowTile;
           typedef FlowTile_ptr<_Config> FlowTile_ptr;

           FlowPath()
           {
               std::memset(m_tiles,0,sizeof(m_tiles));
           }

           void addTile(FlowTile_ptr tile)
           {
               m_tiles[tile->chunk()].first = true;
               m_tiles[tile->chunk()].second = tile;
           }

           void setFailure() { m_failure = true; }

           bool isFailure(){ return m_failure; }

           UniDirection direction(const Cell& cell_)
           {
               if(m_failure)
                    return UniDirection::NONE;
               
               std::pair<ChunkID, LocalCell> lcell = Utils::toLocal(cell_);

               if(!m_tiles[lcell.first].first)
                return UniDirection::NONE;

               std::pair<bool, FlowTile_ptr> pair = m_tiles[lcell.first];

               FlowTile_ptr tile = pair.second;

               return tile->flow().get(lcell.second.x,lcell.second.y).direction;
           }

       private:  
           
           bool m_failure = false;

           std::pair<bool, FlowTile_ptr> m_tiles[Utils::chunkCount];
       };

       template<typename _Config>
       using FlowPath_ptr =  std::shared_ptr<FlowPath<_Config>>;
    }
}

#endif //AI_FF_FlowPath_hpp
