///
///  FFIntergrator.hpp
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


#ifndef AI_FF_Integrator_hpp
#define AI_FF_Integrator_hpp

#include "FFChunk.hpp"
#include "FFTypes.hpp"
//#include "FFWorld.hpp"

namespace ai
{
    namespace ff
    {
        /// The Integrator is in charge of computing
        /// individual flow field tiles (chunks)
        // Based on Elijah Emerson's article 
        // "Crowd Pathfinding and Steering using Flow Field Tiles"
        template<typename _Config>
        class Integrator
        {
        public:

            enum State
            {
                RUNNING,
                FAILURE,
                TERMINATED
            };

            #pragma region Typedefs/Constants

            typedef Utils<_Config>        Utils;
            typedef FlowTile<_Config>     FlowTile;
            typedef FlowTile_ptr<_Config> FlowTile_ptr;

            static const int chunkWidth  = _Config::chunkWidth;
            static const int chunkHeight = _Config::chunkHeight;
            static const int chunkSize   = _Config::chunkWidth * _Config::chunkHeight;

            static const int gridWidth  = _Config::gridWidth;
            static const int gridHeight = _Config::gridHeight;

            typedef typename _Config::BufferCell BufferCell;
            typedef typename _Config::FlowCell   FlowCell;

            typedef Chunk<Cost      ,chunkWidth,chunkHeight> CostChunk;
            typedef Chunk<BufferCell,chunkWidth,chunkHeight> BufferChunk;
            typedef Chunk<FlowCell  ,chunkWidth,chunkHeight> FlowChunk;

            #pragma endregion 

            Integrator(World<_Config>* world_)
            : m_world(world_)
            {}

            inline State state(){ return m_state; }

            inline FlowTile_ptr tile(){ return m_flowTile; }

            inline BufferCell& bufferAt(const LocalCell& lcell_)
            {
                return m_buffer.get(lcell_.x,lcell_.y);
            }

            inline Cost costAt(const LocalCell& lcell_)
            {
                return m_world->costAt(Utils::localToGrid(m_chunkID, lcell_));
            }

            inline bool isPathable(const LocalCell& lcell_)
            {
                return m_world->isPathable(Utils::localToGrid(m_chunkID, lcell_));
            }

            void run(FlowTile_ptr tile_ptr)
            {
                m_flowTile = tile_ptr;

                m_chunkID = m_flowTile->data().chunk();
                m_goals = m_flowTile->data().goals();
                
                m_state = State::RUNNING;

                resetBuffer();
            }

            void resetBuffer()
            {
                m_buffer.clear();

                for(LocalCellIndex& cell : m_goals)
                {
                    addToWaveFront(cell);
                }

                m_waveFront.push_back(s_endCell);
            }

            void stepLineOfSight()
            {

            }

            void stepCostIntegration()
            {
                if(m_state != State::RUNNING)
                    return;

                stepWaveFront();

                for(LocalCell& cell : m_waveFront)
                {
                    Cost cheapCost = cheapestCostAround(cell);
                    IntegratedCost cheapICost = cheapestIntegratedCostAround(cell);

                    ck_assert(cheapCost < g_maxCost);
                    ck_assert(cheapICost < g_maxIntegratedCost);

                    bufferAt(cell).sumCost = cheapCost + cheapICost;
                }

                if(m_waveFront.size() == 0 || 
                   ( m_waveFront.size() ==1 && m_waveFront.front() == s_endCell))
                    processFlowField();
            }

            /// Create the flow field once the integration is done
            void processFlowField()
            {
                if(m_state != State::RUNNING)
                    return;

                for(int i = 0 ; i< chunkSize ; i++)
                {
                    LocalCell lcell(i%chunkWidth , i/chunkHeight);
                    m_flowTile->flow().get(lcell.x,lcell.y).direction = 
                        uniDirOfCheapestIntegratedCostAround(lcell);
                }

                m_state = State::TERMINATED;
            }

            /// Returns the lowest cost (from cost field) of the 4 cells
            /// around the cell passed as argument.
            Cost cheapestCostAround(const LocalCell& cell_)
            {
                Cost minCost = g_maxCost;

                for(UniDirection udir = UniDirection::UP ;
                    udir <= UniDirection::RIGHT ; udir++)
                {
                    ck::Vector2i dir = direction(udir);

                    Cost cost = costAt(dir + cell_);
                       
                    // Warning cost field can have 0 value
                    // thus increase all value of 1
                    if(cost < g_maxCost)
                        cost++; 

                    if(cost < minCost)
                       minCost = cost;
                }
                
                return minCost;
            }

            /// Returns the lowest integrated cost of the 4 cells
            /// around the cell passed as argument.
            IntegratedCost cheapestIntegratedCostAround(const LocalCell& cell_)
            {
                IntegratedCost minCost = g_maxIntegratedCost;
                IntegratedCost tmp;
                for(UniDirection udir = UniDirection::UP ;
                    udir <= UniDirection::RIGHT ; udir++)
                {
                    ck::Vector2i dir = direction(udir);

                    tmp = bufferAt(dir + cell_).sumCost;

                    if(tmp > 0 && tmp< minCost)
                       minCost = bufferAt(dir + cell_).sumCost;
                }
                
                if(minCost == g_maxIntegratedCost)
                    return 0;

                return minCost;
            }

            /// Returns the direction pointing to the cell containing the lowest
            /// integrated cost of the 8 surrounding cells of the cell 
            /// passed as argument.
            UniDirection uniDirOfCheapestIntegratedCostAround(const LocalCell& cell_)
            {
                UniDirection result;
                IntegratedCost minCost = g_maxIntegratedCost;
                IntegratedCost tmp;
                for(UniDirection udir = UniDirection::UP ;
                    udir <= UniDirection::DOWN_RIGHT ; udir++)
                {
                    ck::Vector2i dir = direction(udir);

                    tmp = bufferAt(dir + cell_).sumCost;

                    if(tmp > 0 && tmp< minCost)
                    {
                       minCost = bufferAt(dir + cell_).sumCost;
                       result = udir;
                    }
                }
                
                if(minCost == g_maxIntegratedCost)
                {
                    return UniDirection::NONE;
                }

                return result;
            }

            bool hasUnIntegratedAround(const LocalCell& cell_)
            {
                for(UniDirection udir = UniDirection::UP ;
                    udir <= UniDirection::RIGHT ; udir++)
                {
                    if(bufferAt(cell_ + dir).sumCost == 0)
                        return true;
                }
                
                return false;
            }

            /// Propagate the wave front of one cell
            void stepWaveFront()
            {
                // End cell helps pushing and poping 
                // elements during the same iteration
                // without iterating through on newly 
                // inserted elements.
                // Simply place endCell at the end
                // and push_back new item. Iteration will stop
                // when it reaches endCell.

                ck_assert(m_waveFront.back() == s_endCell);
                
                // Add new wave front to buffer

                // Cannot use iterator since push_back invalidate deque's iterators
                for(int idx = 0; m_waveFront[idx] != s_endCell ; ++idx)
                {
                    LocalCell& cell = m_waveFront[idx];

                    for(UniDirection udir = UniDirection::UP ; 
                        udir <= UniDirection::RIGHT ; udir++)
                        addToWaveFront(cell + direction(udir));
                }

                // Remove previous wave front from buffer
                // removal must be seperated from add to
                // avoid backward propagation

                for(auto it = m_waveFront.begin(); *it != s_endCell ;)
                {
                    bufferAt(*it).waveFront = false;
                    bufferAt(*it).pastWaveFront = true;
                    it = m_waveFront.erase(it);
                }
                
                ck_assert(m_waveFront.front() == s_endCell);
                
                // remove previous end (should be at the top)
                m_waveFront.pop_front();

                // set new end
                m_waveFront.push_back(s_endCell);
            }

            void drawBuffer(float csize,float height)
            {
                float hcsize = 0.5f;

                glBegin(GL_QUADS);
                

                for(int i = 0 ; i< chunkSize ; i++)
                {
                    LocalCell lcell(i%chunkWidth , i/chunkHeight);

                    IntegratedCost cost = bufferAt(lcell).sumCost;

                    Cell cell = Utils::localToGrid(m_chunkID,lcell);

                    glColor3f(0, 0 , cost / 10.f);

                    glVertex3f( (cell.x+0.25f) * csize, (cell.y+0.25f) * csize, height+1);
                    glVertex3f( (cell.x+0.75f) * csize, (cell.y+0.25f) * csize, height+1);
                    glVertex3f( (cell.x+0.75f) * csize, (cell.y+0.75f) * csize, height+1);
                    glVertex3f( (cell.x+0.25f) * csize, (cell.y+0.75f) * csize, height+1);

                }
                glEnd();

                glBegin(GL_LINES);
                
                for(int i = 0 ; i< chunkSize ; i++)
                {
                    LocalCell lcell(i%chunkWidth , i/chunkHeight);

                    IntegratedCost cost = bufferAt(lcell).sumCost;

                    Cell cell = Utils::localToGrid(m_chunkID,lcell);

                    glColor3f(0, 0 , cost / 10.f);

                    glVertex3f( (cell.x)   * csize, (cell.y)   * csize, height+1);
                    glVertex3f( (cell.x+1) * csize, (cell.y)   * csize, height+1);

                    glVertex3f( (cell.x+1) * csize, (cell.y)   * csize, height+1);
                    glVertex3f( (cell.x+1) * csize, (cell.y+1) * csize, height+1);

                    glVertex3f( (cell.x+1) * csize, (cell.y+1) * csize, height+1);
                    glVertex3f( (cell.x)   * csize, (cell.y+1) * csize, height+1);

                    glVertex3f( (cell.x)   * csize, (cell.y+1) * csize, height+1);
                    glVertex3f( (cell.x)   * csize, (cell.y)   * csize, height+1);
                }
                glEnd();
            }


            void drawWaveFront(float csize,float height)
            {
                float hcsize = 0.5f;

                glBegin(GL_QUADS);
                glColor3f(0, 1 , 0);

                for(LocalCell& lcell : m_waveFront)
                {
                    if(lcell == s_endCell)
                        continue;

                    Cell cell = Utils::localToGrid(m_chunkID,lcell);

                    glVertex3f( (cell.x+0.25f) * csize, (cell.y+0.25f) * csize, height+1);
                    glVertex3f( (cell.x+0.75f) * csize, (cell.y+0.25f) * csize, height+1);
                    glVertex3f( (cell.x+0.75f) * csize, (cell.y+0.75f) * csize, height+1);
                    glVertex3f( (cell.x+0.25f) * csize, (cell.y+0.75f) * csize, height+1);
                }
                glEnd();

                glBegin(GL_LINES);
                glColor3f(0, 1 , 0);

                for(LocalCell& lcell : m_waveFront)
                {
                    if(lcell == s_endCell)
                        continue;

                    Cell cell = Utils::localToGrid(m_chunkID,lcell);

                    glVertex3f( (cell.x)   * csize, (cell.y)   * csize, height+1);
                    glVertex3f( (cell.x+1) * csize, (cell.y)   * csize, height+1);

                    glVertex3f( (cell.x+1) * csize, (cell.y)   * csize, height+1);
                    glVertex3f( (cell.x+1) * csize, (cell.y+1) * csize, height+1);

                    glVertex3f( (cell.x+1) * csize, (cell.y+1) * csize, height+1);
                    glVertex3f( (cell.x)   * csize, (cell.y+1) * csize, height+1);

                    glVertex3f( (cell.x)   * csize, (cell.y+1) * csize, height+1);
                    glVertex3f( (cell.x)   * csize, (cell.y)   * csize, height+1);
                }
                glEnd();
            }

            void drawFlow(float csize,float height)
            {
                float hcsize = 0.5f;

                glBegin(GL_LINES);
                glColor3f(1, 1 , 1);

                for(int i = 0 ; i< chunkSize ; i++)
                {
                    LocalCell lcell(i%chunkWidth , i/chunkHeight);

                    IntegratedCost cost = bufferAt(lcell).sumCost;

                    Cell cell = Utils::localToGrid(m_chunkID,lcell);

                    ck::Vector2i dir = direction(m_flowTile->flow().get(lcell.x,lcell.y).direction);

                    glVertex3f( (cell.x + 0.5f) * csize, (cell.y+ 0.5f) * csize, height+5);
                    glVertex3f( (cell.x+ 0.5f) * csize + dir.x*0.5f*csize, (cell.y+ 0.5f) * csize + dir.y*0.5f*csize, height+5);

                }
                glEnd();
            }

        private:

            inline void addToWaveFront(Index cellIndex_)
            {
                addToWaveFront(Utils::localCellAtIndex(cellIndex_));
            }

            /// Adds a cell to wave front if not already wavefront
            /// That is sets its "waveFront" flag in the buffer to true
            ///  and adds it the waveFront cell list
            inline void addToWaveFront(const LocalCell& cell_)
            {
                // If not already in wave front and if pathable
                if(Utils::isValidLocalCell(cell_) &&
                  !bufferAt(cell_).waveFront &&
                  !bufferAt(cell_).pastWaveFront &&
                  isPathable(cell_))
                {
                    bufferAt(cell_).waveFront = true;
                    m_waveFront.push_back(cell_);
                }
            }

            ChunkID m_chunkID;

            World<_Config>* m_world = nullptr;

            BufferChunk m_buffer;

            FlowTile_ptr m_flowTile;

            LocalCellIndexArray m_goals;

            LocalCellDeque m_waveFront;

            State m_state;

            static const Cell s_endCell;
        };

        template<typename _Config>
        const Cell Integrator<_Config>::s_endCell = Cell(-1,-1);
    }
}

#endif // AI_FF_Integrator_hpp