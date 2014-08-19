///
///  FFFlowTile.hpp
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

#ifndef AI_FF_FlowTile_hpp
#define AI_FF_FlowTile_hpp

#include "ai/CKHash.hpp"

#include "FFWorld.hpp"


namespace ai
{
    namespace ff
    {
        template<typename _Config>
        class FlowTile
        {
            
            #pragma region Typedefs/Constants

            typedef Utils<_Config> Utils;

            static const int chunkWidth  = _Config::chunkWidth;
            static const int chunkHeight = _Config::chunkHeight;
            static const int chunkSize   = _Config::chunkWidth * _Config::chunkHeight;

            typedef typename _Config::FlowCell FlowCell;

            typedef Chunk<FlowCell,chunkWidth,chunkHeight> FlowChunk;

            typedef std::vector<Portal::Entrance> EntranceArray;
            typedef std::vector<EntranceID>       EntranceIDArray;
            typedef std::vector<uint16_t>         LocalCellIndexArray;

            #pragma endregion 
            
            #pragma region ID

            union ID
            {
                struct
                {
                    uint32_t chunkHash : 32;
                    
                    /// Hash of goals (either cells or Entrance's cells)
                    uint32_t goalHash  : 32;
                };

                uint64_t id;
            };

            #pragma endregion

            #pragma region Data
            
            class Data
            {
            public:

                #pragma region Data-Constructors

                Data(ChunkID chID_,const EntranceArray& out_)
                : m_chunk(chID_)
                {
                    fillExits(out_);

                    m_id = makeID(m_chunk , m_goals);
                }

                Data(ChunkID chID_, const std::vector<LocalCell>& lcells_)
                : m_chunk(chID_)
                {
                    fillGoals(lcells_);

                    m_id = makeID(m_chunk , m_goals);
                }

                Data(ChunkID chID_, const LocalCellIndexArray& lcells_)
                : m_chunk(chID_)
                {
                    m_goals = lcells_;

                    std::sort(m_goals.begin(), m_goals.end());

                    m_id = makeID(m_chunk , m_goals);
                }

                #pragma endregion

                #pragma region Data-Accessors

                inline ID id() { return m_id; }

                inline ChunkID chunk() { return m_chunk; }

                inline LocalCellIndexArray& goals() { return m_goals; }

                #pragma endregion

            private:

                #pragma region Data-Private-Utils

                void fillExits(const EntranceArray& out_)
                {
                    for(auto outEntr : out_)
                    { 
                        ck_assert(m_chunk == outEntr.chunk);
                        addExitCellsToGoals(outEntr);
                    }
                   
                    std::sort(m_goals.begin(), m_goals.end());
                }

                void addExitCellsToGoals(const Portal::Entrance& entr_)
                {
                    for(int x = entr_.cells.origin.x ; x < entr_.cells.size.width ; x++)
                        for(int y = entr_.cells.origin.y ; y < entr_.cells.size.height ; y++)
                            m_goals.push_back(Utils::indexOfLocalCell(Utils::gridToLocalCell(m_chunk,x,y)));
                }

                void fillGoals(const std::vector<LocalCell>& lcells_)
                {
                    for(auto cell : lcells_)
                    {
                        ck_assert(Utils::isValidLocalCell(cell));
                        m_goals.push_back(Utils::indexOfLocalCell(cell));
                    }
                    std::sort(m_goals.begin(), m_goals.end());
                }

                #pragma endregion
            
                ID m_id;

                ChunkID m_chunk;

                LocalCellIndexArray m_goals;

            };
            
            #pragma endregion

            #pragma region Static-Utils

            /// Create an ID for a flow tile that map entrance 
            /// to goal cells.
            /// Arrays passed needs to be sorted into ascending order.
            inline static ID makeID(ChunkID chID_, 
                                    const EntranceIDArray& inEntr_,
                                    const LocalCellIndexArray& goalCells_)
            {
                ID id;

                id.chunkHash = ck::utils::hash<32>::get(chID_);
                id.inHash    = ck::utils::hash<16>::get(inEntr_);

                id.finalTile = true;
                id.goalHash = ck::utils::hash<16>::get(goalCells_);

                return id;
            }

            inline std::string hexIDString(ID id_)
            {
                std::stringstream stream;
                stream << std::hex << m_id.id;
                return stream.str();
            }

            inline std::string filename(ID id_,const std::string& worldID_)
            {
                return hexIDString(id_)+"_"+worldID_+"."+std::to_string(chunkSize)+".bft";
            }

            #pragma endregion

            #pragma region Constructor

            FlowTile(World<_Config>* world_, const Data& data_)
            : m_world(world_), m_data(data_)
            {}

            #pragma endregion

            #pragma region Accessors

            inline Data& data(){ return m_data; }

            inline ID id(){ return m_data.id(); }

            inline ChunkID chunk(){ return m_data.chunk(); }

            inline FlowChunk& flow(){ return m_flow; }
            /*
            inline std::string hexIDString()
            {
                FlowTile::hexIDString(m_id.id);
            }

            inline std::string filename()
            {
                return hexIDString()+"_"+m_world->hexIDString()+"."+std::to_string(chunkSize)+".bft";
            }
            */
            #pragma endregion

            #pragma region Serialization
            /*
            inline void deserialize(const std::string& rootDir_)
            {
                std::streampos size;
                
                std::ifstream file (rootDir_+"/"+filename(), ios::in|ios::binary|ios::ate);
                if (file.is_open())
                {
                  size = file.tellg();

                  if(size != sizeof(FlowCell) * chunkSize)
                    throw std::exception();

                  file.seekg(0, ios::beg);
                  file.read(m_flow.data(), size);
                  file.close();
                }
                else throw std::exception();
            }

            inline void serialize(const std::string& rootDir_)
            {
                std::streampos size;
                
                std::ofstream file(rootDir_+"/"+filename(), ios::in|ios::binary|ios::ate);
                if (file.is_open())
                {
                  file.seekg(0, ios::beg);
                  file.read(m_flow.data(), size);
                  file.close();
                }
                else throw std::exception();
            }
            */
            #pragma endregion
            
        private:

            World<_Config>* m_world;

            Data m_data;

            FlowChunk m_flow;
            
        };

        template<typename _Config>
        using FlowTile_ptr = std::shared<FlowTile<_Config>>;

    }
}

#endif //AI_FF_FlowTile_hpp