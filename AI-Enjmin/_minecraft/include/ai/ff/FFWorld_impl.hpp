///
///  FFWorld_impl.hpp
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

#ifndef AI_FF_World_impl_hpp
#define AI_FF_World_impl_hpp

#include "ai/CKAssert.hpp"

#include "FFUtils.hpp"
#include "FFIntegrator.hpp"

#include "Tests.hpp"

#define __USE_SAFE_PORTAL_INSERT__
#define __ENABLE_DEBUG_DRAW__

namespace ai
{
    namespace ff
    {
        
        template<typename _Config>
        World<_Config>::World()
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
                auto pairID = Utils::chunksOfFrontier(i);
            
                Border bord1 = Utils::borderOfFrontierInChunk(i,pairID.first);
                Border bord2 = Utils::borderOfFrontierInChunk(i,pairID.second);
            
                std::cout << " Frointier: " << i << " Chunk: " << pairID.first << "Border :" << bord1 << "\n";
                std::cout << " Frointier: " << i << " Chunk: " << pairID.second << "Border :" << bord2 << "\n";
            }
                    
#ifdef __ENABLE_DEBUG_DRAW__
              Debug::addToRender(ck::makeFunctor(this,&World::drawGrid));
              Debug::addKeyDownListener('n',ck::makeFunctor(this,&World::stepIntegrator));
#endif
       
            for(int cIdx = 0 ; cIdx< chunkCount ; cIdx++)
            {
                m_costChunkGraph[cIdx] = CostChunkGraph(m_costGrid.chunk(cIdx));
                m_costChunkGraph[cIdx].computeAllNeighbors();
            }
        }

        #pragma region Debug

        template<typename _Config>
        void World<_Config>
        ::stepIntegrator()
        {
            if(testIntegrator)
                testIntegrator->stepCostIntegration();
        }

        template<typename _Config>
        void World<_Config>
        ::drawGrid()
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
                Cell corg = Utils::chunkOrigin(i);

                for(int x = 0 ; x < chunkWidth ; x++)
                {
                    for(int y = 0 ; y < chunkHeight ; y++)
                    {
                        IndexArray& neighbors = m_costChunkGraph[i].neighbors(x+y*chunkWidth);

                        for(Index idx : neighbors)
                        {
                           // glVertex3f((corg.x+x)*csize+csize/2,(corg.y+y)*csize+csize/2,height + 3);
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

                Cell corg = Utils::chunkOrigin(i);

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

            testIntegrator->drawBuffer(csize,height);
        
            testIntegrator->drawWaveFront(csize,height);

            testIntegrator->drawFlow(csize,height);
        }

        #pragma endregion 

        #pragma region Accessors

        template<typename _Config>
        inline Cost World<_Config>
        ::costAt(int x_, int y_)    
        { 
            return m_costGrid.get(x_,y_); 
        }
        
        template<typename _Config>
        inline Cost World<_Config>
        ::costAt(const Cell& cell_) 
        { 
            return m_costGrid.get(cell_.x,cell_.y); 
        }

        template<typename _Config>
        inline void World<_Config>
        ::setCostAt(const Cost& cost_,const Cell& cell_)
        {
            setCostAt(cost_,cell_.x,cell_.y);
        }

        template<typename _Config>
        inline void World<_Config>
        ::setCostAt(const Cost& cost_, int x_, int y_)
        {
            Cost prev = m_costGrid.get(x_, y_);

            if(prev == cost_)
                return;

            // Mark corresponding chunk as dirty
            ChunkID chID = m_costGrid.indexOfChunkAt(x_,y_);
            m_chunkPortalsMap[chID]->cellHasChanged(prev,cost_,x_,y_);

            m_costGrid.set(cost_, x_, y_);
        }

        template<typename _Config>
        inline bool World<_Config>
        ::isPathable(Cell const& cell_)
        {
            return (m_costGrid.get(cell_.x, cell_.y) != g_maxCost);
        }
        
        template<typename _Config>
        inline PortalGraph& World<_Config>
        ::portalGraph()
        {
            return m_portalGraph;
        }

        #pragma endregion


        #pragma region Portals

        template<typename _Config>
        void World<_Config>
        ::computePortals()
        {
            for(int frIdx = 0 ; frIdx < frontierCount ; frIdx++)
            {
                createAllPortalsOn(frIdx);
            }

            computePortalGraph();
            
        }

        template<typename _Config>
        void World<_Config>
        ::computePortalGraph()
        {
            for(ChunkID chID = 0 ; chID < chunkCount ; chID++)
            {
                Cell chunkOrig = Utils::chunkOrigin(chID);

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

            testIntegrator = new Integrator<_Config>(this, 10, {  Cell(7,8),Cell(8,8),Cell(8,7),Cell(7,6) });
            testIntegrator->resetBuffer();
        }

        template<typename _Config>
        Portal_ptr World<_Config>
        ::createNewPortal(FrontierID frontier_,
                          ChunkID chunk1_, ChunkID chunk2_,
                          const Cell& entr1Origin_, const Cell& entr2Origin_,
                          const CellSize& size_)
        {
            // Create Portal Unique ID

            PortalID portalId = Utils::makePortalID(entr1Origin_,entr2Origin_,size_);

            // Create Entrance ID's

            EntranceID entr1ID = Utils::makeEntranceID(chunk1_,entr1Origin_,size_);
            EntranceID entr2ID = Utils::makeEntranceID(chunk2_,entr2Origin_,size_);

            // Entrances' Data

            Portal::EntranceData entrance1(entr1ID,chunk1_, CellRect(entr1Origin_, size_),
                                           Utils::borderOfFrontierInChunk(frontier_,chunk1_));

            Portal::EntranceData entrance2(entr2ID,chunk2_, CellRect(entr2Origin_, size_),
                                           Utils::borderOfFrontierInChunk(frontier_,chunk2_));

            // Instantiate 

            return Portal_ptr(new Portal(portalId,frontier_,entrance1,entrance2));
        }

        template<typename _Config>
        void World<_Config>
        ::addPortal(Portal_ptr portal_)
        {
            m_portals[portal_->frontier()].push_back(portal_);
            
            // Add newly created portal entrances to their respectives chunks

            m_entrances[portal_->entrance1().chunk].push_back(&portal_->entrance1());
            m_entrances[portal_->entrance2().chunk].push_back(&portal_->entrance2());

            // And add the portal to the graph

            m_portalGraph.addNode(portal_);
        }


        template<typename _Config>
        void World<_Config>
        ::createAllPortalsOn(FrontierID frontier_)
        {
            // Chunk took in reference for portal creation
            // Doesn't matter which one is chosen
            // since implementation is symetric
            ChunkID referenceChunk = Utils::chunksOfFrontier(frontier_).first;
            ChunkID otherChunk = Utils::chunksOfFrontier(frontier_).second;

            Border border = Utils::borderOfFrontierInChunk(frontier_,referenceChunk);

            ck_assert(border != Border::NONE);

            Direction dir = Utils::directionFromBorder(border);

            ck_assert(dir != Direction::NONE);

            ck::Vector2i vdir = direction(dir);

            ck_assert((vdir.x != 0 || vdir.y != 0) && (abs(vdir.x) != abs(vdir.y)));

            // Direction in which the grid is processed
            // to find portals
            ck::Vector2i walk_dir(abs(vdir.y), abs(vdir.x));

            // List of portal for this frontier
    
            PortalList& list = m_portals[frontier_];

            // Remove all portals
            for(PortalList_it it = list.begin(); it != list.end();)
                it = list.erase(it);

            ChunkCoord chCoord = Utils::chunkCoordinates(referenceChunk);

            // Origin of chunk 
            Cell corigin = Utils::chunkOrigin(referenceChunk) + Utils::borderOffset(border);
            
            Cell end = Utils::chunkEnd(referenceChunk);

            // Origin of the next portal
            Cell org = corigin;

            while(org.x < end.x && org.y < end.y)
            {
                if(isPathable(org) && isPathable(org + vdir))
                {
                    // Are pathable
                    // Then keep moving forward to find the end of the portal
                    CellSize size(1,1);
                    while((org + walk_dir * size).x < end.x && (org + walk_dir * size).y < end.y &&
                          isPathable(org +  walk_dir * size) && 
                          isPathable(org +  walk_dir * size + vdir))
                    {
                        size += walk_dir;
                    }

                    Cell entr1Origin = org;
                    Cell entr2Origin = org+vdir;

                    // Instantiate 

                    Portal_ptr newPortal = createNewPortal(frontier_,referenceChunk,otherChunk,entr1Origin,entr2Origin,size);

                    addPortal(newPortal);

                    // Move origin to the end of the portal

                    org += walk_dir * size;
                   
                }
                else // Cells are not pathable then keep moving forward
                    org += walk_dir;
            }
        }

    }

}
#endif //AI_FF_World_impl_hpp