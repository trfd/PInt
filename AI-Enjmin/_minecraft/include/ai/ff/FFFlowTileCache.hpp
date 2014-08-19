///
///  FFFlowTileCache.hpp
///
///  Created by Baptiste Dupy on 18/08/2014.
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

#ifndef AI_FF_FlowTileCache_hpp
#define AI_FF_FlowTileCache_hpp

#include "FFFlowTile.hpp"

namespace ai
{
    namespace ff
    {
        template<typename _Config>
        class FlowTileCache
        {
            #pragma region Typedefs/Constants

            typedef Utils<_Config>      Utils;
            typedef FlowTile<_Config>   FlowTile;

            static const int chunkWidth  = _Config::chunkWidth;
            static const int chunkHeight = _Config::chunkHeight;
            static const int chunkSize   = _Config::chunkWidth * _Config::chunkHeight;

            typedef typename _Config::FlowCell FlowCell;

            typedef Chunk<FlowCell,chunkWidth,chunkHeight> FlowChunk;

            std::map<uint64_t,FlowTile_ptr>           FlowTileMap;
            std::map<uint64_t,FlowTile_ptr>::iterator FlowTileMap_it;

            #pragma endregion 

            inline bool canAccess(FlowTile::ID id) const
            {
                return (m_tiles.find(id.id) != m_tiles.end());
            }

            inline FlowTile_ptr<_Config>& tileForID(FlowTile::ID id)
            {
                FlowTileMap_it it = m_tiles.find(id.id);

                if(it == m_tiles.end())
                    throw std::exception();

                return *it;
            }

            void addTile(const FlowTile_ptr<_Config>& tile)
            {
                m_tiles[tile->id()] = tile;
            }

        private:

            FlowTileMap m_tiles;

        };
    }
}

#endif //AI_FF_FlowTileCache_hpp