///
///  FFUtils.hpp
///
///  Created by Baptiste Dupy on 17/08/2014.
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


#ifndef AI_FF_Utils_hpp
#define AI_FF_Utils_hpp

#include "FFTypes.hpp"

namespace ai
{
    namespace ff
    {
        template<typename _Config>
        class Utils
        {
        public:

            #pragma region Typedefs/Constants

            static const int chunkWidth  = _Config::chunkWidth;
            static const int chunkHeight = _Config::chunkHeight;

            static const int gridWidth  = _Config::gridWidth;
            static const int gridHeight = _Config::gridHeight;

            typedef typename _Config::BufferCell BufferCell;
            typedef typename _Config::FlowCell   FlowCell;

            typedef Chunk<Cost      ,chunkWidth,chunkHeight> CostChunk;
            typedef Chunk<BufferCell,chunkWidth,chunkHeight> BufferChunk;
            typedef Chunk<FlowCell  ,chunkWidth,chunkHeight> FlowChunk;

            typedef std::shared_ptr<CostChunk>             CostChunk_ptr;
            typedef CostChunkGraph<chunkWidth,chunkHeight> CostChunkGraph;

            typedef Grid<CostChunk  ,gridWidth,gridHeight> CostGrid;
            typedef Grid<BufferChunk,gridWidth,gridHeight> BufferGrid;
            typedef Grid<FlowChunk  ,gridWidth,gridHeight> FlowGrid;

            typedef std::shared_ptr<Portal> Portal_ptr;

            typedef std::vector<Portal_ptr>                  PortalList;
            typedef typename PortalList::iterator            PortalList_it;
            //typedef ck::gen::data::AdjacencyList<Portal_ptr> PortalGraph;
            typedef std::vector<Portal::Entrance*>           EntranceList;

            static const int chunkRowCount    = CostGrid::chunkRowCount;
            static const int chunkColumnCount = CostGrid::chunkColumnCount;
            static const int chunkCount       = CostGrid::chunkCount;

            static const int frontierCount    = (2 * chunkRowCount - 1) * chunkColumnCount  - chunkRowCount ;

            #pragma endregion 

            /// Returns the coordinate of the chunk in the grid
            inline static ChunkCoord chunkCoordinates(ChunkID chunk_)
            {
                return ChunkCoord(chunk_ % chunkRowCount,
                                  chunk_ / chunkRowCount);
            }

            /// Returns the ChunkID corresponding to the coordinates
            /// of a chunk
            inline static ChunkID chunkID(ChunkCoord const& chCoord)
            {
                return chCoord.x + chCoord.y * chunkRowCount;
            }

             /// Returns the ChunkID corresponding to the coordinates
            /// of a chunk.(Arguments are ChunkCoord)
            inline static ChunkID chunkID(int cx_,int cy_)
            {
                return cx_ + cy_ * chunkRowCount;
            }

            /// Returns the ChunkID of the neightbor chunk in direction dir_
            inline static ChunkID nextChunk(ChunkID chId_,Direction dir_)
            {
                return chunkID(chunkCoordinates(chId_)+direction(dir_));
            }

            /// Returns the Direction that corresponds to
            /// a border of the chunk (usefull for portals)
            inline static Direction directionFromBorder(Border border_)
            {
                switch(border_)
                {
                case TOP:    return Direction::UP;
                case BOTTOM: return Direction::DOWN;
                case LEFT:   return Direction::LEFT;
                case RIGHT:  return Direction::RIGHT;
                default:     return Direction::NONE;
                }
            }

            /// Returns the direction associated to a frontier
            /// Note that frontier's directions always point out
            /// in the positive way. 
            /// That is direction can only be RIGHT (positive X direction)
            /// or BOTTOM (positive Y direction)
            inline static Direction directionOfFrontierInChunk(FrontierID frontier_,ChunkID chID_)
            {
                std::pair<ChunkID, ChunkID> chunks = chunksOfFrontier(frontier_);

                ChunkID other = (chunks.first == chID_) ? chunks.second : chunks.first;
                
                ChunkCoord cThis = chunkCoordinates(chID_);
                ChunkCoord cOther = chunkCoordinates(other);

                if(cOther.x == cThis.x)
                { 
                    if(cOther.y < cThis.y)
                        return Direction::UP;
                    else if(cOther.y > cThis.y)
                        return Direction::DOWN;
                }
                else if(cOther.y == cThis.y)
                { 
                    if(cOther.x < cThis.x)
                        return Direction::LEFT;
                    else if(cOther.x > cThis.x)
                        return Direction::RIGHT;
                }

                return Direction::NONE; 
            }

            /// The border of chunk in which the frontier lies
            inline static Border borderOfFrontierInChunk(FrontierID frontier_,ChunkID chID_)
            {
                std::pair<ChunkID, ChunkID> chunks = chunksOfFrontier(frontier_);

                ChunkID other = (chunks.first == chID_) ? chunks.second : chunks.first;
                
                ChunkCoord cThis = chunkCoordinates(chID_);
                ChunkCoord cOther = chunkCoordinates(other);

                if(cOther.x == cThis.x)
                { 
                    if(cOther.y < cThis.y)
                        return Border::TOP;
                    else if(cOther.y > cThis.y)
                        return Border::BOTTOM;
                }
                else if(cOther.y == cThis.y)
                { 
                    if(cOther.x < cThis.x)
                        return Border::LEFT;
                    else if(cOther.x > cThis.x)
                        return Border::RIGHT;
                }

                return Border::NONE;    
            }

            /// Return the origin of the chunk chID_
            /// in cell coordinates
            inline static Cell chunkOrigin(ChunkID chID_)
            {
                return Cell((chID_ % chunkRowCount) * chunkWidth,
                            (chID_ / chunkRowCount) * chunkHeight);
            }

            /// Return the (past-)end of the chunk chID_
            /// in cell coordinates 
            /// This cell is not in the chunk (past end)
            /// and can be also outside of the grid.
            /// Equivalent to (lastCell.x+1,lastCell.y+1),
            /// with lastCell the lastCell in the grid.
            inline static Cell chunkEnd(ChunkID chID_)
            {
                return Cell(((chID_ % chunkRowCount)+1) * chunkWidth,
                            ((chID_ / chunkRowCount)+1) * chunkHeight);
            }

            /// Return the rectangle corresponding to the chunk chID_
            /// in cell coordinates
            inline CellRect chunkRect(ChunkID chID_)
            {
                return CellRect(chunkOrigin(chID_), CellSize(chunkWidth,chunkHeight));
            }

            inline static ck::Vector2i borderOffset(Border border_)
            {
                switch(border_)
                {
                case LEFT:
                case TOP:    return ck::Vector2i(0,0);
                case BOTTOM: return ck::Vector2i(0,chunkHeight-1);
                case RIGHT:  return ck::Vector2i(chunkWidth-1,0);

                default:     return ck::Vector2i(0,0);
                }
            }

            /// Bijection from chunkID couple to frontierID
            /// see chunksOfFrontier()
            inline static FrontierID frontier(ChunkID lhs_, ChunkID rhs_)
            {
                if(lhs_ == rhs_)
                    return g_badFrontier;

                ChunkCoord lCoord = chunkCoordinates(lhs_);
                ChunkCoord rCoord = chunkCoordinates(rhs_);

                if(lCoord.x == rCoord.x)
                {
                    return ((2 * chunkRowCount - 1) * min(lCoord.y,rCoord.y) + chunkRowCount - 1 +
                            lCoord.x,rCoord.x);
                }
                else if(lCoord.y == rCoord.y)
                {
                    return ((2 * chunkRowCount - 1) * lCoord.y + min(lCoord.x,rCoord.x));
                }
                else
                    return g_badFrontier;
            }

            /// Returns the pair of chunks on either side of the frontier
            inline static std::pair<ChunkID, ChunkID> chunksOfFrontier(FrontierID frontier_)
            {
                ck_assert(frontier_>=0 && frontier_<frontierCount);

                int x1;
                int y1 = frontier_ / (2*chunkRowCount - 1);

                int x2,y2;
                if(frontier_ % (2 * chunkRowCount - 1) >= chunkRowCount - 1)
                { 
                    x1 = (frontier_ % (2 * chunkRowCount - 1)) - (chunkRowCount - 1);
                    x2 = x1;
                    y2 = y1+1;
                } 
                else 
                { 
                    x1 = frontier_ % (2 * chunkRowCount - 1);
                    x2 = x1+1;
                    y2 = y1;
                }

                return std::make_pair(chunkID(x1,y1),chunkID(x2,y2));
            }

            /// Converts a local cell of Chunk ch_ to a global grid cell.
            inline static Cell localToGrid(ChunkID ch_,const LocalCell& lCell_)
            {
                return chunkOrigin(ch_) + lCell_;
            }

            /// Converts a grid cell to a cell relative to the chunk ch_
            /// Even if the cell doesn't belong to this chunk
            inline static LocalCell gridToLocal(ChunkID ch_,const Cell& gCell_)
            {
                return gCell_ - chunkOrigin(ch_);
            }

            /// Converts a grid cell into a local cell and returns 
            /// the local cell and the chunk is which the local cell 
            /// (and therefore the grid cell) lies.
            inline static std::pair<ChunkID,LocalCell> toLocal(const Cell& gCell_)
            {
                std::pair<ChunkID,LocalCell> pair;
                
                pair.first = chunkID(gCell_ / chunkWidth , 
                                     gCell_ / chunkHeight);
                
                pair.second.x = gCell_.x % chunkWidth;
                pair.second.y = gCell_.y % chunkHeight;

                return pair;
            }

            /// Return the index of a cell local to a chunk
            inline static Index indexOfLocalCell(const LocalCell& lcell_)
            {
                return lcell_.x + lcell_.y * chunkWidth;
            }

            /// Returns the index of a grid cell
            inline static Index indexOfCell(const Cell& gcell_)
            {
                return gcell_.x + gcell_.y * gridWidth;
            }
        

            /// Returns whether or not a local cell is not out of its chunk's bounds
            inline static bool isValidLocalCell(const LocalCell& lcell_)
            {
                return (lcell_.x >= 0 && lcell_.y >= 0 && lcell_.x < chunkWidth && lcell_.y < chunkHeight);
            }

            /// Returns whether or not a cell is not out of its grid's bounds
            inline static bool isValidGridCell(const Cell& gcell_)
            {
                return (gcell_.x >= 0 && gcell_.y >= 0 && gcell_.x < gridWidth && gcell_.y < gridHeight);
            }

            /// Create a portal ID using entrance origins and size
            inline static Portal::ID makePortalID(Cell entr1Origin, Cell entr2Origin, const CellSize& size)
            {  
              Portal::ID id;

              id.index = Utils::indexOfCell(Cell(min(entr1Origin.x,entr2Origin.x) , min(entr1Origin.y,entr2Origin.y)));
              id.size  = max(size.width , size.height);

              return id;
            }

            /// Create a portal ID using min origin (that is min X and min Y of entrances' origin)
            /// and size
            inline static Portal::ID makePortalID(Cell origin, const CellSize& size)
            {  
              Portal::ID id;

              id.index = Utils::indexOfCell(origin);
              id.size  = max(size.width , size.height);

              return id;
            }

            inline static Portal::EntranceData::ID makeEntranceID(ChunkID chID_,const Cell& origin_, const CellSize& size_)
            {
                Portal::EntranceData::ID id;

                id.chunkHash = ck::utils::hash<32>::get(chID_);
                id.localIndex = Utils::indexOfLocalCell(Utils::gridToLocal(chID_,origin_));
                id.size = max(size_.width , size_.height);

                return id;
            };

            inline static Portal::EntranceData::ID makeEntranceID(ChunkID chID_,const CellRect& rect_)
            {
                Portal::EntranceData::ID id;

                id.chunkHash = ck::utils::hash<32>::get(chID_);
                id.localIndex = Utils::indexOfLocalCell(Utils::gridToLocal(chID_,rect_.origin));
                id.size = max(rect_.size.width , rect_.size.height);

                return id;
            };
        };
    }
}

#endif // AI_FF_Utils_hpp
