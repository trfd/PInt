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
#include "FFWorld.hpp"

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

            #pragma region Typedefs/Constants

            typedef Utils<_Config> Utils;

            static const int chunkWidth  = _Config::chunkWidth;
            static const int chunkHeight = _Config::chunkHeight;

            static const int gridWidth  = _Config::gridWidth;
            static const int gridHeight = _Config::gridHeight;

            typedef typename _Config::BufferCell BufferCell;
            typedef typename _Config::FlowCell   FlowCell;

            typedef Chunk<Cost      ,chunkWidth,chunkHeight> CostChunk;
            typedef Chunk<BufferCell,chunkWidth,chunkHeight> BufferChunk;
            typedef Chunk<FlowCell  ,chunkWidth,chunkHeight> FlowChunk;

            //typedef std::shared_ptr<Portal> Portal_ptr;

            //typedef std::vector<Portal_ptr>                  PortalList;
            //typedef typename PortalList::iterator            PortalList_it;
            //typedef std::vector<Portal::Entrance*>           EntranceList;

            //static const int chunkRowCount    = CostGrid::chunkRowCount;
            //static const int chunkColumnCount = CostGrid::chunkColumnCount;
            //static const int chunkCount       = CostGrid::chunkCount;

            //static const int frontierCount    = (2 * chunkRowCount - 1) * chunkColumnCount  - chunkRowCount ;

            #pragma endregion 

            Integrator(World<_Config>* world_, ChunkID chID_, const LocalCellArray& goals_)
            : m_world(world_), 
            m_chunkID(chID_), 
            m_goals(goals_)
            {}


            inline BufferCell& bufferAt(const LocalCell& lcell_)
            {
                return m_buffer.get(lcell_.x,lcell_.y);
            }

            inline Cost& costAt(const LocalCell& lcell_)
            {
                return m_world->costAt(Utils::localToGrid(m_chunkID, lcell_));
            }

            inline bool isPathable(const LocalCell& lcell_)
            {
                return m_world->isPathable(Utils::localToGrid(m_chunkID, lcell_));
            }

            void resetBuffer()
            {
                m_buffer.clear();

                for(LocalCell& cell : m_goals)
                {
                    addToWaveFront(cell);
                }

                m_waveFront.push_back(s_endCell);
            }

            void passLineOfSight()
            {

            }

            void passCostIntegration()
            {

            }

            void passFlowField()
            {

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

                    addToWaveFront(cell + ck::Vector2i(0 ,-1));
                    addToWaveFront(cell + ck::Vector2i(0 , 1));
                    addToWaveFront(cell + ck::Vector2i(-1, 0));
                    addToWaveFront(cell + ck::Vector2i( 1, 0));
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

            void drawWaveFront(float csize,float height)
            {
                float hcsize = 0.5f;

                glBegin(GL_QUADS);
                glColor3f(0, 1 , 0);

                for(LocalCell& cell : m_waveFront)
                {
                    if(cell == s_endCell)
                        continue;

                    glVertex3f( (cell.x+0.25f) * csize, (cell.y+0.25f) * csize, height+1);
                    glVertex3f( (cell.x+0.75f) * csize, (cell.y+0.25f) * csize, height+1);
                    glVertex3f( (cell.x+0.75f) * csize, (cell.y+0.75f) * csize, height+1);
                    glVertex3f( (cell.x+0.25f) * csize, (cell.y+0.75f) * csize, height+1);
                }
                glEnd();

                glBegin(GL_LINES);
                glColor3f(0, 1 , 0);

                for(LocalCell& cell : m_waveFront)
                {
                    if(cell == s_endCell)
                        continue;

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

        private:

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

            LocalCellArray m_goals;

            LocalCellDeque m_waveFront;

            static const Cell s_endCell;

        };

        template<typename _Config>
        const Cell Integrator<_Config>::s_endCell = Cell(-1,-1);
    }
}

#endif // AI_FF_Integrator_hpp