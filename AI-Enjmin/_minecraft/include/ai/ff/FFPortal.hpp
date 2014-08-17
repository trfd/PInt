///
///  FFPortal.hpp
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

#ifndef AI_FF_Portal_hpp
#define AI_FF_Portal_hpp
#include "ai/CKAssert.hpp"
#include <map>
#include <memory>


#include "ai/DtAdjacencyList.hpp"

#include "FFTypes.hpp"
#include "FFChunk.hpp"
#include "FFGrid.hpp"

#define __USE_SAFE_PORTAL_INSERT__

namespace ai
{
    namespace ff
    {
        /// Portal connect chunkA to chunkB
        /// with the cells defined by the Cell rectangle 
        /// aCells of chunkA and bCells of chunkB
        class Portal : public std::enable_shared_from_this<Portal>
        {
        public:
           
            enum CompareResult
            {
                DISJOINT    = 0,
                INTERSECT   = 1,
                EQUAL       = 2
            };

            #pragma region Entrance

            /// Half of portal that resides on one chunk
            struct Entrance
            {
                ChunkID chunk;
                CellRect cells;
                Border border;
                Portal& portal;

                Entrance(Portal& portal_,Border border_,ChunkID id_, CellRect cells_)
                : portal(portal_), border(border_),chunk(id_), cells(cells_)
                {}

                bool operator==(Entrance const& entr_)
                {
                    return (chunk == entr_.chunk && cells == entr_.cells);
                }

                inline bool intersects(CellRect const& rect_)
                {
                    return CellRect::intersects(cells,rect_);
                }

                static bool intersects(Entrance const& lhs_, Entrance const& rhs_)
                {
                    if(lhs_.chunk != rhs_.chunk)
                        return false;
                    return CellRect::intersects(lhs_.cells,rhs_.cells);
                }
            };

            #pragma endregion

            Portal(FrontierID frontier_,
                   ChunkID c1_, ChunkID c2_, 
                   Border b1_, Border b2_,
                   CellRect const& cr1_, CellRect const& cr2_)
            : m_frontier(frontier_),
            m_entrance1(*this,b1_,c1_,cr1_),
            m_entrance2(*this,b2_,c2_,cr2_)
            {}

            inline static bool intersects(Portal const& lhs_, Portal const& rhs_)
            {
                return ((Entrance::intersects(lhs_.m_entrance1,rhs_.m_entrance1)) || 
                       (Entrance::intersects(lhs_.m_entrance1,rhs_.m_entrance2)) ||
                       (Entrance::intersects(lhs_.m_entrance2,rhs_.m_entrance1)) ||
                       (Entrance::intersects(lhs_.m_entrance2,rhs_.m_entrance2)) );
            }

            inline bool intersects(CellRect const& rect_)
            {
                return (m_entrance1.intersects(rect_) || m_entrance2.intersects(rect_));
            }
            
            /// Returns the entrance that lies in chunk_
            inline Entrance& entrance(ChunkID chunk_)
            {
                if(m_entrance1.chunk == chunk_)
                    return m_entrance1;
                else if(m_entrance2.chunk == chunk_)
                    return m_entrance2;
                else
                    throw std::exception(); // Portal not in chunk
            }

            /// Returns the origin of entrance that lies in chunk
            inline Cell origin(ChunkID chunk_)
            {
                return entrance(chunk_).cells.origin;
            }

            std::shared_ptr<Portal> ptr(){ return shared_from_this(); }

             /// Returns the size of portal
            inline CellSize size()
            {
                ck_assert(m_entrance1.cells.size == m_entrance2.cells.size);
                return m_entrance1.cells.size;
            }

            /// Returns the rectangle of entrance that lies in chunk
            inline CellRect rectangle(ChunkID chunk_)
            {
                 return entrance(chunk_).cells;
            }

            inline Entrance& entrance1() { return m_entrance1; }
            inline Entrance& entrance2() { return m_entrance2; }

            inline FrontierID frontier() { return m_frontier; }

        private:
            
            Entrance m_entrance1;
            Entrance m_entrance2;

            FrontierID m_frontier;
        };

        
    }
}

#endif //AI_FF_Portal_hpp