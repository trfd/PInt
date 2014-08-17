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

#include "ai/CKAssert.hpp"
#include <map>

#include "ai/DtAdjacencyList.hpp"

#include "FFTypes.hpp"
#include "FFChunk.hpp"
#include "FFGrid.hpp"
#include "FFPortal.hpp"

#include "ASPathFinder.hpp"
#include "FFCostChunkGraph.hpp"
#include "FFPortalGraph.hpp"

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
       
            World()
            {
                for(int idx = 0 ; idx< chunkCount ; idx++)
                { 
                    m_costGrid.setChunk(CostChunk_ptr(new CostChunk()),idx);
                }

                for(int i= 0 ; i<gridWidth*gridHeight ; i++)
                { 
                    int c = 0;
                    if(test::map[i])
                        c = 255;
                    m_costGrid.set(c, i % gridWidth, i / gridHeight);

                }

                for(int i = 0 ; i < frontierCount ; i++)
                {
                    auto pairID = chunksOfFrontier(i);

                    Border bord1 = borderOfFrontierInChunk(i,pairID.first);
                    Border bord2 = borderOfFrontierInChunk(i,pairID.second);

                    std::cout << " Frointier: " << i << " Chunk: " << pairID.first << "Border :" << bord1 << "\n";
                    std::cout << " Frointier: " << i << " Chunk: " << pairID.second << "Border :" << bord2 << "\n";
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
                for(int i = 0 ; i < frontierCount ; ++i)
                {
                    PortalList& list = m_portals[i];
                    for(int pIdx = 0 ; pIdx < list.size() ; pIdx++)
                    {
                        Portal_ptr p = list[pIdx];

                        CellRect rect1 = p->entrance1().cells; 
                        CellRect rect2 = p->entrance2().cells;

                        glVertex3f( rect1.origin.x * csize,
                                    rect1.origin.y * csize, height+pIdx);
                        glVertex3f((rect1.origin.x + rect1.size.width) * csize,
                                    rect1.origin.y * csize, height+pIdx);

                        glVertex3f((rect1.origin.x + rect1.size.width) * csize,
                                    rect1.origin.y * csize, height+pIdx);
                        glVertex3f((rect1.origin.x + rect1.size.width) * csize,
                                   (rect1.origin.y + rect1.size.height) * csize, height+pIdx);

                        glVertex3f((rect1.origin.x + rect1.size.width) * csize,
                                   (rect1.origin.y + rect1.size.height) * csize, height+pIdx);
                        glVertex3f( rect1.origin.x * csize,
                                   (rect1.origin.y + rect1.size.height) * csize, height+pIdx);
                         
                        glVertex3f( rect1.origin.x * csize,
                                   (rect1.origin.y + rect1.size.height) * csize, height+pIdx);
                        glVertex3f( rect1.origin.x * csize,
                                    rect1.origin.y * csize, height+pIdx);
                        
                        glVertex3f( rect2.origin.x * csize,
                                    rect2.origin.y * csize, height+pIdx);
                        glVertex3f((rect2.origin.x + rect2.size.width) * csize,
                                    rect2.origin.y * csize, height+pIdx);
                                        
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

                    for(std::vector<ASPath<Index>>::iterator p = m_portalPaths[i].begin();
                        p != m_portalPaths[i].end() ; ++p)
                    {
                      
                        int r = rand() %128;
                        int v = rand() %128;
                        int b = rand() %128;

                        glColor3ub(r+128,v+128,b+128);

                        prevX= (*p->begin())%chunkWidth;
                        prevY=(*p->begin())/chunkWidth;

                        for(ASPath<Index>::iterator it = p->begin() ; it != p->end() ; ++it)
                        {   
                            x = (*it)%chunkWidth ; y = (*it)/chunkWidth;

                            //glVertex3f((corg.x+prevX)*csize+csize/2,(corg.y+prevY)*csize+csize/2,height + 4);
                            //glVertex3f((corg.x+x)*csize+csize/2,(corg.y+y)*csize+csize/2,height + 4);

                            prevX = x; prevY = y;
                        }
                    }
                }

                glEnd();


                /// Draw portal graph

                glBegin(GL_LINES);
                glColor3f(1,1,1);

                for(int idx = 0 ; idx < m_portalGraph.nodeCount() ; idx++)
                {
                    auto node1 = m_portalGraph.nodeAt(idx);

                    Portal_ptr portal1 = node1->data();

                    Cell orig1 = portal1->entrance1().cells.origin;

                    orig1.x +=  portal1->entrance1().cells.size.width>>1;
                    orig1.y +=  portal1->entrance1().cells.size.height>>1;

                    for(int edge = 0 ; edge < m_portalGraph.nodeAt(idx)->edgeCount() ; edge++)
                    {
                         Portal_ptr portal2 = node1->adjacentNodeAt(edge)->data();

                         Cell orig2 = portal2->entrance1().cells.origin;

                         orig2.x +=  portal2->entrance1().cells.size.width>>1;
                         orig2.y +=  portal2->entrance1().cells.size.height>>1;

                         glVertex3f( orig1.x * csize , orig1.y * csize, height + 4 );
                         glVertex3f( orig2.x * csize , orig2.y * csize, height + 3 );
                    }
                }

                glEnd();

                glBegin(GL_QUADS);

                glColor3f(1 , 1 , 1);

                Cell c = m_portalGraph.nodeAt(16)->data()->entrance1().cells.origin;

                glVertex3f(  c.x    * csize,  c.y    * csize, height+1);
                glVertex3f( (c.x+1) * csize,  c.y    * csize, height+1);
                glVertex3f( (c.x+1) * csize, (c.y+1) * csize, height+1);
                glVertex3f(  c.x    * csize, (c.y+1) * csize, height+1);

                 c = m_portalGraph.nodeAt(0)->data()->entrance1().cells.origin;

                glVertex3f(  c.x    * csize,  c.y    * csize, height+1);
                glVertex3f( (c.x+1) * csize,  c.y    * csize, height+1);
                glVertex3f( (c.x+1) * csize, (c.y+1) * csize, height+1);
                glVertex3f(  c.x    * csize, (c.y+1) * csize, height+1);

                glEnd();


                glBegin(GL_LINES);
                glColor3f(1,0,1);
                Cell prev = (*portalPathTest.begin())->entrance1().cells.origin;

                for(auto it = portalPathTest.begin() ; it != portalPathTest.end() ; ++it)
                {
                    Cell cell = (*it)->entrance1().cells.origin;

                    glVertex3f(prev.x*csize,prev.y*csize, height +3);
                    glVertex3f(cell.x*csize,cell.y*csize, height +3);

                    prev = cell;
                
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
                    createPortals(frIdx);
                }

                computePortalGraph();
                
            }

            void computePortalGraph()
            {
                for(ChunkID chID = 0 ; chID < chunkCount ; chID++)
                {
                    Cell chunkOrig = chunkOrigin(chID);

                    for(EntranceList::iterator entr_first_it = m_entrances[chID].begin();
                        entr_first_it != m_entrances[chID].end() ; ++entr_first_it)
                    {
                        Cell firstOrig = (*entr_first_it)->cells.origin - chunkOrig ;
                        int firstCellIdx = firstOrig.x + firstOrig.y * chunkWidth;

                        for(EntranceList::iterator entr_sec_it = entr_first_it+1;
                            entr_sec_it != m_entrances[chID].end() ; ++entr_sec_it)
                        {
                            Cell secOrig = (*entr_sec_it)->cells.origin - chunkOrig ;
                            int secCellIdx = secOrig.x + secOrig.y * chunkWidth;

                            if(firstCellIdx == secCellIdx)
                                continue;

                             ASPathFinder pf(&m_costChunkGraph[chID],firstCellIdx,secCellIdx);

                             pf.burnSteps();

                             if(pf.state() == ASPathFinder::TERMINATED)
                             {
                                 m_portalPaths[chID].push_back(pf.path());
                              
                                 m_portalGraph.addEdge((*entr_first_it)->portal.ptr(),
                                                       (*entr_sec_it)->portal.ptr());
                             }
                             else
                                 std::cout << "ASPathFinder fails...\n";
                        }
                    }
                }

                m_portalGraph.computeAllNeighbors();

                ASPathFinder pf(&m_portalGraph,0,16);

                pf.burnSteps();

                if(pf.state() == ASPathFinder::TERMINATED)
                {
                    portalPathTest = m_portalGraph.makePortalPath(pf.path());
                    std::cout << "Found graph path\n";
                }
                else
                 std::cout << "Did not Found graph path\n";

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
            inline Direction directionOfFrontierInChunk(FrontierID frontier_,ChunkID chID_)
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
            inline Border borderOfFrontierInChunk(FrontierID frontier_,ChunkID chID_)
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
            inline std::pair<ChunkID, ChunkID> chunksOfFrontier(FrontierID frontier_)
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

                ck_assert(border != Border::NONE);

                Direction dir = directionFromBorder(border);

                ck_assert(dir != Direction::NONE);

                ck::Vector2i vdir = direction(dir);

                ck_assert((vdir.x != 0 || vdir.y != 0) && (abs(vdir.x) != abs(vdir.y)));

                // Direction in which the grid is processed
                // to find portals
                ck::Vector2i walk_dir(abs(vdir.y), abs(vdir.x));

                // List of portal for this frontier

                //ck_assert(m_portals[frontier_].size() == 0);
    
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

                        Portal_ptr newPortal(new Portal(frontier_,
                                                        referenceChunk, otherChunk,
                                                        borderOfFrontierInChunk(frontier_,referenceChunk),
                                                        borderOfFrontierInChunk(frontier_,otherChunk),
                                                        CellRect(org, size), CellRect(org+vdir,size)));
                        list.push_back(newPortal);
                        
                        m_entrances[referenceChunk].push_back(&newPortal->entrance(referenceChunk));
                        m_entrances[otherChunk].push_back(&newPortal->entrance(otherChunk));

                        m_portalGraph.addNode(newPortal);

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
                
            PortalGraph m_portalGraph;

            std::vector<ASPath<Index>> m_portalPaths[chunkCount];

            PortalList m_portals[frontierCount];

            EntranceList m_entrances[chunkCount];

            ASPath<Portal_ptr> portalPathTest;
            
        };
    }
}

#endif //AI_FF_World_hpp