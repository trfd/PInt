///
///  FFWorld.hpp
///
///  Created by Baptiste Dupy on 13/08/2014.
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

#ifndef AI_FF_World_hpp
#define AI_FF_World_hpp

#include <cassert>
#include <map>

#include "ai/DtAdjacencyList.hpp"

#include "FFTypes.hpp"
#include "FFChunk.hpp"
#include "FFGrid.hpp"
#include "FFPortal.hpp"

#include "ASPathFinder.hpp"
#include "FFCostChunkGraph.hpp"

#include "Tests.hpp"

#define __USE_SAFE_PORTAL_INSERT__
#define __ENABLE_DEBUG_DRAW__

namespace ai
{
    namespace ff
    {
        template<int __width, int __height>
        struct DefaultWorldConfig
        {
            typedef BufferCell  BufferCell;
            typedef FlowCell    FlowCell;

            static const int chunkWidth  = 16;
            static const int chunkHeight = 16;
            
            static const int gridWidth   = __width;
            static const int gridHeight  = __height; 
        };


        //TODO: Change the way portal are created. In supporting actions
        // on portals such as Merge/Split/Expand/Reduce/Add/Remove, dynamic
        // mutation of costGrid can be achieved and alter only paths
        // linked to these portals
        template<typename _Config>
        class World
        {
        public:

            class ChunkPortals;

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

            typedef std::vector<Portal_ptr>                     PortalList;
            typedef typename PortalList::iterator               PortalList_it;
            typedef ck::gen::data::AdjacencyList<Portal_ptr>    PortalGraph;
            typedef std::map<ChunkID,ChunkPortals>              ChunkPortalsMap;

            static const int chunkRowCount    = CostGrid::chunkRowCount;
            static const int chunkColumnCount = CostGrid::chunkColumnCount;
            static const int chunkCount       = CostGrid::chunkCount;

            static const int frontierCount    = (2 * chunkRowCount - 1) * (chunkColumnCount - 1) - chunkRowCount ;

            #pragma endregion 
       
            World()
            {
                for(int idx = 0 ; idx< chunkCount ; idx++)
                    m_costGrid.setChunk(CostChunk_ptr(new CostChunk()),idx);

                for(int i= 0 ; i<gridWidth*gridHeight ; i++)
                { 
                    int c = 0;
                    if(test::map[i])
                        c = 255;
                    m_costGrid.set(c, i % gridWidth, i / gridHeight);

                }
                        
#ifdef __ENABLE_DEBUG_DRAW__
                Debug::addToRender(ck::makeFunctor(this,&World::drawGrid));
#endif
            
                for(int cIdx = 0 ; cIdx< chunkCount ; cIdx++)
                {
                    m_costChunkGraph[cIdx] = CostChunkGraph(m_costGrid.chunk(cIdx));
                    m_costChunkGraph[cIdx].computeAllNeighbors();
                }
            }

            #pragma region ChunkPortals

            class ChunkPortals
            {
            public:

                ChunkPortals()
                {
                    m_portalDirty[0] = true;
                    m_portalDirty[1] = true;
                    m_portalDirty[2] = true;
                    m_portalDirty[3] = true;
                }
                
                /// World callback that allow ChunkPortals to 
                /// track deprecated portals and graphs
                void cellHasChanged(Cost from_, Cost to_, int x_, int y_)
                {
                    int lx = x_ % chunkWidth;
                    int ly = y_ % chunkHeight;

                    if(lx == 0 || ly == 0 || lx == chunkWidth-1 || ly == chunkHeight-1)
                        borderCellHasChanged(from_,to_,x_,y_);

                    m_portalGraphDirty = true; 
                }

                /// Returns the corresponding Border if the cell (x_,y_)
                /// belongs to any. Otherwise returns Border::NONE 
                inline static Border borderForCell(int x_, int y_)
                {
                    return borderForLocalCell(x_%chunkWidth, y_%chunkHeight);
                }

                /// Same as borderForCell but cell (localx_,localy_) has
                /// local coordinate relative to a chunk (no matter with chunk,
                /// the border are the sames)
                inline static Border borderForLocalCell(int localx_, int localy_)
                {
                    if(localy_ == 0)                
                        return Border::TOP;
                    else if(localy_ == chunkHeight-1)    
                        return Border::BOTTOM;
                    else if(localx_ == 0)                
                        return Border::LEFT;
                    else if(localx_ == chunkWidth-1)     
                        return Border::RIGHT;

                    return Border::NONE;
                }

                inline std::vector<Portal_ptr>& portals(Border border_)
                {
                    if(border_ >= Border::NONE)
                        throw std::exception();

                    return m_portals[border_];
                }

                inline ChunkID chunkID(){ return m_chunk; }

                inline bool isGraphDirty(){ return m_portalGraphDirty; }

                inline bool isBorderDirty(Border border_)
                {
                     if(border_ >= Border::NONE)
                        throw std::exception();

                    return m_portalDirty[border_];
                }

            private:

                /// Track changes on border of the chunk.
                /// Removes deprecated portals and set dirty flag
                void borderCellHasChanged(Cost from_, Cost to_, int x_, int y_)
                {
                    Border border = borderForCell(x_,y_);

                    if(border >= Border::NONE)
                        return;

                    CellRect cell(x_,y_,1,1);

                    PortalList& list = m_portals[border];
                    
                    // Check if the cell that has changed intersects any of the existing
                    // portals of the corresponding border (top,bottom,...)
                    for(PortalList_it it = list.begin() ; it != list.end() ; ++it)
                    {
                        if((*it)->intersects(cell))
                        {
                            // Remove the portal if cell is in the portal
                            // Trying to fix the portal is worthless
                            it = list.erase(it);
                        }
                    }

                    m_portalDirty[border] = true;
                }

                ChunkID m_chunk;
                
                bool m_portalDirty[4];
                bool m_portalGraphDirty;

                PortalList m_portals[4];
            };

            #pragma endregion

            #pragma region Debug

            void drawGrid()
            {
                float csize = 10.f;
                float height = 200.f;

                //// Draw cells' cost

                glBegin(GL_QUADS);

                glColor3f(1 , 1 , 1);

                 glVertex3f(  0    * csize,  0    * csize, height+10);
                 glVertex3f( (0+1) * csize,  0    * csize, height+10);
                 glVertex3f( (0+1) * csize, (0+1) * csize, height+10);
                 glVertex3f(  0    * csize, (0+1) * csize, height+10);

                 glColor3f(1 , 0, 0);
                 glVertex3f(  1    * csize,  0    * csize, height+10);
                 glVertex3f( (1+1) * csize,  0    * csize, height+10);
                 glVertex3f( (1+1) * csize, (0+1) * csize, height+10);
                 glVertex3f(  1    * csize, (0+1) * csize, height+10);

                 glColor3f(0 , 1, 0);
                 glVertex3f(  0    * csize,  1    * csize, height+10);
                 glVertex3f( (0+1) * csize,  1    * csize, height+10);
                 glVertex3f( (0+1) * csize, (1+1) * csize, height+10);
                 glVertex3f(  0    * csize, (1+1) * csize, height+10);

                for(int x = 0 ; x < gridWidth ; x++)
                {
                    for(int y = 0 ; y < gridHeight ; y++)
                    {      
                        //glColor(m_costGrid.get(x,y) , 0 , 0);
                        glColor3ub(m_costGrid.get(x,y) , 0 , 0);

                        glVertex3f(  x    * csize,  y    * csize, height);
                        glVertex3f( (x+1) * csize,  y    * csize, height);
                        glVertex3f( (x+1) * csize, (y+1) * csize, height);
                        glVertex3f(  x    * csize, (y+1) * csize, height);
                    }
                }

                glEnd();

                //// Draw Portals

                glBegin(GL_LINES);
                glColor3f(0,1,1);
                for(int i = 0 ; i < chunkCount ; ++i)
                {
                    ChunkPortals& cp = m_chunkPortalsMap[i];

                    for(int j = 0; j < 4 ; j++)
                    {
                        PortalList& list = cp.portals((Border)j);
                        for(auto p : list)
                        {
                            CellRect rect1 = p->entrance1().cells; 
                            CellRect rect2 = p->entrance2().cells;

                            glVertex3f( rect1.origin.x * csize,
                                        rect1.origin.y * csize, height+1);
                            glVertex3f((rect1.origin.x + rect1.size.width) * csize,
                                        rect1.origin.y * csize, height+1);

                            glVertex3f((rect1.origin.x + rect1.size.width) * csize,
                                        rect1.origin.y * csize, height+1);
                            glVertex3f((rect1.origin.x + rect1.size.width) * csize,
                                       (rect1.origin.y + rect1.size.height) * csize, height+1);

                            glVertex3f((rect1.origin.x + rect1.size.width) * csize,
                                       (rect1.origin.y + rect1.size.height) * csize, height+1);
                            glVertex3f( rect1.origin.x * csize,
                                       (rect1.origin.y + rect1.size.height) * csize, height+1);
                             
                            glVertex3f( rect1.origin.x * csize,
                                       (rect1.origin.y + rect1.size.height) * csize, height+1);
                            glVertex3f( rect1.origin.x * csize,
                                        rect1.origin.y * csize, height+1);
                            
                            glVertex3f( rect2.origin.x * csize,
                                        rect2.origin.y * csize, height+1);
                            glVertex3f((rect2.origin.x + rect2.size.width) * csize,
                                        rect2.origin.y * csize, height+1);
                                            
                            glVertex3f((rect2.origin.x + rect1.size.width) * csize,
                                        rect2.origin.y * csize, height+1);
                            glVertex3f((rect2.origin.x + rect2.size.width) * csize,
                                       (rect2.origin.y + rect2.size.height) * csize, height+1);
                                            
                            glVertex3f((rect2.origin.x + rect2.size.width) * csize,
                                       (rect2.origin.y + rect2.size.height) * csize, height+1);
                            glVertex3f( rect2.origin.x * csize,
                                       (rect2.origin.y + rect2.size.height) * csize, height+1);

                            glVertex3f( rect2.origin.x * csize,
                                       (rect2.origin.y + rect2.size.height) * csize, height+1);               
                            glVertex3f( rect2.origin.x * csize,
                                        rect2.origin.y * csize, height+1);
                        }
                    }

                }

                glEnd();


                //// Draw neighbors

                glBegin(GL_LINES);
                glColor3f(1,0,1);
                for(int i = 0 ; i < chunkCount ; ++i)
                {
                    Cell corg = chunkOrigin(i);

                    for(int x = 0 ; x < chunkWidth ; x++)
                    {
                        for(int y = 0 ; y < chunkHeight ; y++)
                        {
                            IndexArray& neighbors = m_costChunkGraph[i].neighbors(x+y*chunkWidth);

                            for(Index idx : neighbors)
                            {
                              //  glVertex3f((corg.x+x)*csize+csize/2,(corg.y+y)*csize+csize/2,height + 3);
                               // glVertex3f((corg.x+ (idx%chunkWidth))*csize+csize/2, (corg.y+(idx/chunkWidth))*csize+csize/2,height + 3);
                            }
                        }
                    }

                }

                glEnd();

                /// Draw path

                int prevX=0;
                int prevY=0;

                int x = 0;
                int y = 0;

                glBegin(GL_LINES);
                

                for(int i = 0 ; i < chunkCount ; ++i)
                {
                    srand(i);


                    Cell corg = chunkOrigin(i);

                    for(std::vector<ASPath>::iterator p = m_portalPaths[i].begin();
                        p != m_portalPaths[i].end() ; ++p)
                    {
                      
                        int r = rand() %128;
                        int v = rand() %128;
                        int b = rand() %128;

                        glColor3ub(r+128,v+128,b+128);

                        prevX=p->begin().index()%chunkWidth;
                        prevY=p->begin().index()/chunkWidth;

                        for(ASPathIterator it = p->begin() ; it != p->end() ; ++it)
                        {   
                            x = it.index()%chunkWidth ; y = it.index()/chunkWidth;

                            glVertex3f((corg.x+prevX)*csize+csize/2,(corg.y+prevY)*csize+csize/2,height + 4);
                            glVertex3f((corg.x+x)*csize+csize/2,(corg.y+y)*csize+csize/2,height + 4);

                            prevX = x; prevY = y;
                        }
                    }
                }

                glEnd();
            }

            #pragma endregion 

            #pragma region Mutation

            inline Cost costAt(int x_, int y_) { return m_costGrid.get(x_,y_); }

            inline void setCostAt(Cost const& cost_, int x_, int y_)
            {
                Cost prev = m_costGrid.get(x_, y_);

                if(prev == cost_)
                    return;

                // Mark corresponding chunk as dirty
                ChunkID chID = m_costGrid.indexOfChunkAt(x_,y_);
                m_chunkPortalsMap[chID]->cellHasChanged(prev,cost_,x_,y_);

                m_costGrid.set(cost_, x_, y_);
            }
           
            #pragma endregion

            inline void beginChanges(){}
            inline void endChanges(){ computePortals(); }
        
        private:

            #pragma region Portals

            /// Use the current state of the cost grid to
            /// find and create portals
            /// This method is far from being optimal.
            /// This uses a brute-force by removing
            /// every portal linked to the chunk
            /// and recreating them.
            void computePortals()
            {
                for(int frIdx = 0 ; frIdx < frontierCount ; frIdx++)
                {
                    //computePortalsForChunk(chIdx);
                    createPortals(frIdx);
                }


                //// Debug

                
                for(int frIdx = 0 ; frIdx < frontierCount ; frIdx++)
                {
                    std::pair<ChunkID, ChunkID> chunks = chunksOfFrontier(frIdx);

                    ChunkID chunkPair[2] = { chunks.first, chunks.second };


                    //// Compute path between portals

                    std::vector<Portal_ptr> allPortals = m_portals[frIdx];

                   
                    for(PortalList_it it = allPortals.begin() ; it != allPortals.end() ; ++it)
                    {
                        for(int c = 0 ; c<2 ; c++)
                        { 
                            Cell chunkOrig = chunkOrigin(chunkPair[c]);
                            Cell itOrig = (*it)->origin(chunkPair[c]) - chunkOrig ;
                            int itCellIdx = itOrig.x + itOrig.y * chunkWidth;

                            for(PortalList_it other = it+1 ; other != allPortals.end() ; ++other)
                            {
                                Cell otherOrig = (*other)->origin(chunkPair[c]) - chunkOrig;
                                int otherCellIdx = otherOrig.x + otherOrig.y * chunkWidth;

                                ASPathFinder pf(&m_costChunkGraph[chunkPair[c]],itCellIdx,otherCellIdx);

                                pf.burnSteps();

                                if(pf.state() == ASPathFinder::TERMINATED)
                                {
                                    m_portalPaths[chunkPair[c]].push_back(pf.path());
                                }
                                else
                                    std::cout << "ASPathFinder fails...\n";
                            }
                        }
                    }

                }
                ///
            }

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

            inline static ChunkID chunkID(int x_,int y_)
            {
                return x_ + y_ * chunkRowCount;
            }

            /// Returns the ChunkID of the neightbor chunk in direction dir_
            inline static ChunkID nextChunk(ChunkID chId_,Direction dir_)
            {
                return chunkID(chunkCoordinates(chId_)+direction(dir_));
            }

            /// Returns the Direction that corresponds to
            /// a border of the chunk (usefull for portals)
            inline Direction directionFromBorder(Border border_)
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
            inline Direction directionFromFrontier(FrontierID frontier_)
            {
                std::pair<ChunkID, ChunkID> chunks = chunksOfFrontier(frontier_);

                ChunkCoord c1 = chunkCoordinates(chunks.first);
                ChunkCoord c2 = chunkCoordinates(chunks.second);

                if(c1.x == c2.x)
                    return Direction::RIGHT;
                else if(c1.y == c2.y)
                    return Direction::DOWN;
                return Direction::NONE;
            }

            /// The border of chunk in which the frontier lies
            inline Border borderOfFrontierInChunk(FrontierID frontier_,ChunkID chID_)
            {
                std::pair<ChunkID, ChunkID> chunks = chunksOfFrontier(frontier_);

                ChunkID other = (chunks.first == chID_) ? chunks.second : chunks.first;
                
                ChunkCoord cThis = chunkCoordinates(chID_);
                ChunkCoord cOther = chunkCoordinates(other);

                if(cThis.x == cOther.x)
                    if(cOther.y < cThis.y)
                        return Border::TOP;
                    else if(cOther.y > cThis.y)
                        return Border::BOTTOM;
                    else // Are equal (AND they shouldn't)
                        return Border::NONE;
                else if(cThis.y == cOther.y)
                    if(cOther.x < cThis.x)
                        return Border::LEFT;
                    else if(cOther.y > cThis.y)
                        return Border::RIGHT;
                    else // Are equal (AND they shouldn't)
                        return Border::NONE;
                return Border::NONE;    
            }

            /// Return the origin of the chunk chID_
            /// in cell coordinates
            inline Cell chunkOrigin(ChunkID chID_)
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
            inline Cell chunkEnd(ChunkID chID_)
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

            inline ck::Vector2i borderOffset(Border border_)
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
            inline FrontierID frontier(ChunkID lhs_, ChunkID rhs_)
            {
                if(lhs_ == rhs_)
                    return g_badFrontier;

                ChunkCoord lCoord = chunkCoordinates(lhs_);
                ChunkCoord rCoord = chunkCoordinates(rhs_);

                if(lCoord.x == rCoord.x)
                {
                    return ((2 * chunkRowCount - 1) * min(lCoord.y,rCoord.y) + chunkRowCount - 1+ lCoord.x,rCoord.x);
                }
                else if(lCoord.y == rCoord.y)
                {
                    return ((2 * chunkRowCount - 1) * lCoord.y + min(lCoord.x,rCoord.x));
                }
                else
                    return g_badFrontier;
            }

            inline std::pair<ChunkID, ChunkID> chunksOfFrontier(FrontierID frontier_)
            {
                int x1 = (frontier_ % (2*chunkRowCount - 1)) % (chunkRowCount - 1);
                int y1 = frontier_ / (2*chunkRowCount - 1);

                int x2,y2;
                if(frontier_ % (2 * chunkRowCount - 1) >= chunkRowCount - 1)
                    x2 = x1 , y2 = y1+1; 
                else 
                    x2 = x1+1 , y2 = y1;

                return std::make_pair(chunkID(x1,y1),chunkID(x2,y2));
            }

            /*
            /// Compute all portals for a given chunk
            void computePortalsForChunk(ChunkID chunk_)
            {
                ChunkCoord chc = chunkCoordinates(chunk_);

                ChunkPortals& cPortals = m_chunkPortalsMap[chunk_];
            
                // Create on bottom and right portals since 
                // since each chunk will create portals (avoid duplication)
                // BUT this not the correct to do this.

                if(cPortals.isBorderDirty(Border::BOTTOM) && chc.y+1 < chunkColumnCount)
                    createPortals(chunk_,Border::BOTTOM);
                   
               if(cPortals.isBorderDirty(Border::RIGHT) && chc.x+1 < chunkRowCount)
                    createPortals(chunk_,Border::RIGHT);
             
            }
            */
            inline bool isNotMaxCost(Cell const& cell_)
            {
                return (m_costGrid.get(cell_.x, cell_.y) != g_maxCost);
            }

            /// Create portals for a given frontier
            /// This creation of portal removes every existing portals
            /// even if untouched. 
            //TODO: Improve efficiency of portal mutation by 
            // implementating Merge/Split/Extend/Reduce/Add/Remove mutation
            // on portals. This is tightly linked to agent's path recomputing.
            void createPortals(FrontierID frontier_)
            {
                // Chunk take in reference for portal creation
                // Doesn't matter which one is chosen
                // since implementation is symmetric
                ChunkID referenceChunk = chunksOfFrontier(frontier_).first;
                ChunkID otherChunk = chunksOfFrontier(frontier_).second;

                Border border = borderOfFrontierInChunk(frontier_,referenceChunk);

                Direction dir = directionFromFrontier(frontier_);

                ck::Vector2i vdir = direction(dir);

                assert((vdir.x != 0 || vdir.y != 0) && (abs(vdir.x) != abs(vdir.y)));

                // Direction in which the grid is processed
                // to find portals
                ck::Vector2i walk_dir(abs(vdir.y), abs(vdir.x));

                //ChunkPortals& cPortals = m_chunkPortalsMap[chID_];

                // List of portal for this frontier
    
                PortalList& list = m_portals[frontier_];//cPortals.portals(border_);

                // Remove all portals
                for(PortalList_it it = list.begin(); it != list.end();)
                    it = list.erase(it);

                ChunkCoord chCoord = chunkCoordinates(referenceChunk);

                // Origin of chunk 
                Cell corigin = chunkOrigin(referenceChunk) + borderOffset(border);
                
                Cell end = chunkEnd(referenceChunk);

                // Origin of the next portal
                Cell org = corigin;

                while(org.x < end.x && org.y < end.y)
                {
                    if(isNotMaxCost(org) && isNotMaxCost(org + vdir))
                    {
                        CellSize size(1,1);
                        while((org + walk_dir * size).x < end.x && (org + walk_dir * size).y < end.y &&
                              isNotMaxCost(org +  walk_dir * size) && 
                              isNotMaxCost(org +  walk_dir * size + vdir))
                        {
                            size += walk_dir;
                        }

                        Portal_ptr newPortal(new Portal(frontier_,referenceChunk, otherChunk,
                                                        CellRect(org, size), CellRect(org+vdir,size)));
                        list.push_back(newPortal);
                        
                        org += walk_dir * size;
                       
                    }
                    else
                        org += walk_dir;
                }
                
            }

            #pragma endregion

            /// Grid containing cost of cells
            CostGrid m_costGrid;

            CostChunkGraph m_costChunkGraph[chunkCount];

            /// Map ChunkID to ChunkPortals 
            ChunkPortalsMap m_chunkPortalsMap;
                
            /// [Unused for now]
            PortalGraph m_portalGraph;

            std::vector<ASPath> m_portalPaths[chunkCount];

            PortalList m_portals[frontierCount];
            
        };
    }
}

#endif //AI_FF_World_hpp