///
///  FFPortalGraph.hpp
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

#ifndef AI_FF_PortalGraph_hpp
#define AI_FF_PortalGraph_hpp
#include "ai/CKAssert.hpp"
#include <map>
#include <memory>

#include "ai/DtAdjacencyList.hpp"

#include "ASGraph.hpp"
#include "FFPortal.hpp"


namespace ai
{
    namespace ff
    {
        typedef std::shared_ptr<Portal> Portal_ptr;
        
        class PortalGraph 
        : public ASGraph, 
          public ck::gen::data::AdjacencyList<std::shared_ptr<Portal>>
        {
        public:
           
            /// Returns the number of nodes 
            /// in the graph
            inline virtual size_t count() override
            {
                return this->nodeCount();
            }

            void computeAllNeighbors()
            {
                m_neighbors.resize(count());

                for(Index idx = 0 ; idx < count() ; idx++)
                {
                    computeNeighbors(idx);
                }
            }

            void computeNeighbors(Index idx_)
            {
                m_neighbors[idx_].first = true;

                for(Index adj = 0 ; adj < nodeAt(idx_)->edgeCount() ; adj++)
                {
                    Node* n = this->nodeAt(idx_)->adjacentNodeAt(adj);
                    
                    Index nIdx = this->indexOf(n);

                    if(nIdx < INT_MAX)
                        m_neighbors[idx_].second.push_back(nIdx);
                }
            }

            /// Returns estimation of cost of path
            inline virtual float estimate(Index stIdx_, Index tgIdx_) override
            {
                return distance(stIdx_,tgIdx_);
            }

            /// Returns the node's neighbors
            inline virtual IndexArray& neighbors(Index idx_) override
            {
                if(idx_ >= m_neighbors.size())
                    throw std::exception();

                if(!m_neighbors[idx_].first)
                    computeNeighbors(idx_);

                return m_neighbors[idx_].second;
            }

            /// Returns cost of adjacent nodes
            inline virtual float cost(Index fromIdx_, Index toIdx_) override
            {
                return distance(fromIdx_,toIdx_);
            }

            inline float distance(Index fromIdx_, Index toIdx_)
            {
                Node* nf = this->nodeAt(fromIdx_);
                Node* nt = this->nodeAt(toIdx_);

                int x = nf->data()->entrance1().cells.origin.x -
                        nt->data()->entrance1().cells.origin.x; 
                int y = nf->data()->entrance1().cells.origin.y -
                        nt->data()->entrance1().cells.origin.y; 

                return sqrtf(x*x+y*y);
            }

            ASPath<Portal_ptr> makePortalPath(ASPath<Index>const& indexPath_)
            {
                ASPath<Portal_ptr> result;
                
                for(auto it = indexPath_.begin() ; it != indexPath_.end() ; it++)
                {
                    result.nodeList().push_back(nodeAt(*it)->data());
                }
                
                return result;
            }

        private:

             /// Neigbors of node's indexes
             /// The boolean (pair.first) is true if the neighbors have been computed
             /// The array of neighbors is hold in pair.second.
             std::vector<std::pair<bool, IndexArray>> m_neighbors;
        };
    }
}

#endif //AI_FF_PortalGraph_hpp