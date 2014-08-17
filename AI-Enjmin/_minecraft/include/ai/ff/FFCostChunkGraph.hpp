///
///  FFCostChunkGraph.hpp
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

#ifndef AI_FF_CostChunkGraph_hpp
#define AI_FF_CostChunkGraph_hpp


#include "ASGraph.hpp"
#include "ai/CKUMReturnType.hpp"

#include "FFTypes.hpp"
#include "FFChunk.hpp"

using namespace ck::utils::meta;

namespace ai
{
    namespace ff
    {

        template<int __width, int __height>
        class CostChunkGraph : public ASGraph
        {
        public:
           
            typedef Chunk<Cost,__width,__height> CostChunk;
            typedef std::shared_ptr<CostChunk>   CostChunk_ptr;

            static const int width  = __width;
            static const int height = __height;
            static const int size   = __width*__height; 

            CostChunkGraph()
            : m_chunk(nullptr)
            {
                for(int idx = 0 ; idx<size ; idx++)
                    m_neighbors[idx].first = false;
            }

            CostChunkGraph(CostChunk_ptr chunk_)
            : m_chunk(chunk_)
            {
                for(int idx = 0 ; idx<size ; idx++)
                    m_neighbors[idx].first = false;
            }

            virtual ~CostChunkGraph(){}

            void computeAllNeighbors()
            {
                for(int idx = 0 ; idx<size ; idx++)
                    computeNeighbors(idx);
            }

            void computeNeighbors(Index idx_)
            {
                // Set the "computed" flag
                m_neighbors[idx_].first = true;

                if(!isWalkable(idx_))
                    return;

                int x = idx_ % __width;
                int y = idx_ / __width;

                bool left   = isWalkable(x-1,y);
                bool right  = isWalkable(x+1,y);
                bool up     = isWalkable(x  ,y-1);
                bool down   = isWalkable(x  ,y+1);

                // Direct neighbors

                if(left)
                    m_neighbors[idx_].second.push_back(idx_-1);

                if(right)
                     m_neighbors[idx_].second.push_back(idx_+1);

                if(up)
                     m_neighbors[idx_].second.push_back(idx_-__width);

                if(down)
                     m_neighbors[idx_].second.push_back(idx_+__width);

                // Diagonal

                if((left || up) && isWalkable(x-1, y-1))
                     m_neighbors[idx_].second.push_back(idx_-1-__width);
                
                if((right || up) && isWalkable(x+1,y-1))
                     m_neighbors[idx_].second.push_back(idx_+1-__width);

                if((left || down) && isWalkable(x-1,y+1))
                     m_neighbors[idx_].second.push_back(idx_-1+__width);

                if((right || down) && isWalkable(x+1,y+1))
                     m_neighbors[idx_].second.push_back(idx_+1+__width);
            }

            /// Returns the number of nodes 
            /// in the graph
            inline virtual size_t count() override
            {
                return size;
            }

            /// Returns estimation of cost of path
            virtual float estimate(Index stIdx_, Index tgIdx_) override
            {
                // Heurisitic is straigh line

                int x = (tgIdx_%__width-stIdx_%__width);
                int y = (tgIdx_/__width-stIdx_/__width); 

                float value = sqrtf(x*x+y*y);

                return value;
            }

            /// Returns the node's neighbors
            inline virtual IndexArray& neighbors(Index idx_) override
            {
                if(!m_neighbors[idx_].first)
                    computeNeighbors(idx_);
                return m_neighbors[idx_].second;
            }

            /// Returns cost of adjacent nodes
            inline virtual float cost(Index fromIdx_, Index toIdx_) override
            {
                return get(toIdx_)+1;
            }

            inline Cost get(Index idx_)
            {
                if(!m_chunk->isInChunk(idx_))
                    throw std::out_of_range("Out of Chunk");

                return m_chunk->get(idx_%__width, idx_ / __height);
            }

            inline bool isWalkable(Index idx_)
            {
                if(!m_chunk->isInChunk(idx_))
                    return false;

                return (g_maxCost != get(idx_));
            }

            inline bool isWalkable(int x_, int y_)
            {
                if(!m_chunk->isInChunk(x_,y_))
                    return false;

                return (g_maxCost != m_chunk->get(x_,y_));
            }

            inline ChunkID otherChunk(ChunkID chunk_)
            {
                if(m_entrance1.chunk == chunk_)
                    return m_entrance2.chunk;
                else if(m_entrance2.chunk == chunk_)
                    return m_entrance1.chunk;
                else
                    throw std::exception(); //Bad chunkID
            }
           
        private:
             
             CostChunk_ptr m_chunk;

             /// Neigbors of node's indexes
             /// The boolean (pair.first) is true if the neighbors have been computed
             /// The array of neighbors is hold in pair.second.
             std::pair<bool, IndexArray> m_neighbors[size];

        };
    }
}

#endif //AI_FF_CostChunkGraph_hpp