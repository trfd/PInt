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

#include "ai/CKHash.hpp"
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

             /// Portal ids are built using the index
             /// of the portal's origin cell and its size.
             /// The origin is the min in X and Y of 
             /// entrance's origins.
             /// In this way portal have unique ids 
             /// independant of memory,time and implementation.
             /// The size is either the width or height
             /// depending of portal's orientation.
             /// The size uses only 5 bits thus it can
             /// not be greater than 64.
             /// The size of chunks can then be not 
             /// larger than that.
             /// Thus the index uses 58 bits. That makes 
             /// the square grid can not be larger than 
             /// 2^29x2^29
             //TODO: add an extra to save world's ID
            union ID
            {   
                struct
                {
                    uint64_t index : 58;
                    uint8_t  size  : 6;
                };

                uint64_t id;
            };
           
            enum CompareResult
            {
                DISJOINT    = 0,
                INTERSECT   = 1,
                EQUAL       = 2
            };

            #pragma region Entrance

            class EntranceData
            {
            public:

                /// Entrance ids are built using the local index
                /// of the entrance's origin cell and its size.
                /// The size is either the width or height
                /// depending of portal's orientation.
                /// The local index is the index of origin
                /// in chunk coordinates. It uses 8 bits
                /// Thus chunk should not be larger than 64x64
                /// Note that a lot value can not be used,
                /// since localIndex lies in the chunk's edge 
                /// making every index in the interior unused
                /// and the size can not exceed chunk width or
                /// height.
                union ID
                {
                    struct
                    {
                        uint32_t chunkHash;
                        uint16_t localIndex;
                        uint8_t  size;
                        // 8 bits free
                    };
                    uint64_t id;
                };

                ID uid;
                ChunkID chunk;
                CellRect cells;
                Border border;

                EntranceData(ID uid_,ChunkID chunk_,CellRect cells_,Border border_)
                : uid(uid_), chunk(chunk_), cells(cells_), border(border_)
                {}
            };

            /// Half of portal that resides on one chunk
            class Entrance : public EntranceData
            {
            public:

                Portal& portal;

                Entrance(Portal& portal_,const EntranceData& data_)
                : EntranceData(data_) , portal(portal_)
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

                Entrance& operator= (const Entrance& ref_)
                {
                    return Entrance(portal, *this);
                }
            };

            #pragma endregion

            Portal(ID id_,
                   FrontierID frontier_, 
                   const EntranceData& en1Data_,
                   const EntranceData& en2Data_)
            : m_id(id_),
            m_frontier(frontier_),
            m_entrance1(*this,en1Data_),
            m_entrance2(*this,en2Data_)
            {
                ck_assert(m_entrance1.cells.size == m_entrance2.cells.size);
            }

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

            #pragma region Accessors
            
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

            inline ChunkID otherChunk(ChunkID chunk_)
            {
                 if(m_entrance1.chunk == chunk_)
                    return m_entrance2.chunk;
                else if(m_entrance2.chunk == chunk_)
                    return m_entrance1.chunk;
                else 
                    return g_badChunk;
            }

            inline Entrance& entrance1() { return m_entrance1; }
            inline Entrance& entrance2() { return m_entrance2; }

            inline FrontierID frontier() { return m_frontier; }

            inline ID id(){ return m_id; }

            #pragma endregion

        private:
            
            ID m_id;

            Entrance m_entrance1;
            Entrance m_entrance2;

            FrontierID m_frontier;
        };

        #pragma region Post-Typedefs

        typedef Portal::ID               PortalID;
        typedef Portal::EntranceData::ID EntranceID;
        
        #pragma endregion
    }
}

#endif //AI_FF_Portal_hpp