///
///  ASPathFinder.hpp
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

#ifndef AI_FF_ASPathFinder_hpp
#define AI_FF_ASPathFinder_hpp

#include <ctime>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <cfloat>

#include "ai/CKFunctor.hpp"

#include "ASGraph.hpp"
#include "ASPath.hpp"

namespace ai
{
    namespace ff
    {
        /// Generic implementation of A* pathfinding
        class ASPathFinder
        {
        public:
            enum State
            {
                RUNNING,
                FAILURE,
                TERMINATED
            };

            #pragma region Typedefs

            //Sets are more efficient for searches
            typedef std::set<Index>       NodeSet;
            typedef NodeSet::iterator     NodeSet_it;
            typedef std::map<Index,Index> NodeMap;
            typedef NodeMap::iterator     NodeMap_it;
            typedef std::vector<float>    CostArray;
            typedef CostArray::iterator   CostArray_it;

            #pragma endregion

            #pragma region Constructors

            ASPathFinder(ASGraph* graph_,
                         Index startIndx_, 
                         Index targetIndx_)
            : m_graph(graph_),
              m_startIndex(startIndx_), 
              m_targetIndex(targetIndx_),
              m_resultPath(graph_),
              m_callback(this , &ASPathFinder::emptyCallback)
            {
                ck_assert(startIndx_ != targetIndx_);

                m_currState = RUNNING;

                m_pastCost.resize(m_graph->count(),0);
                m_estimCost.resize(m_graph->count(),0);
                m_totalCost.resize(m_graph->count(),0);

                m_pastCost[m_startIndex] = 0;
                m_estimCost[m_startIndex] = m_graph->estimate(m_startIndex,m_targetIndex);
                m_totalCost[m_startIndex] = m_pastCost[m_startIndex] + m_estimCost[m_startIndex];

                m_openSet.insert(m_startIndex);
            }

            #pragma endregion

            #pragma region Accessors

            inline State state() const { return m_currState; }
             
            inline const ASPath& path() const
            { 
                if(m_currState == RUNNING) 
                    return g_failPath;
                return m_resultPath; 
            }
              
            inline void setCallback(ck::Functor<void, ASPath const&> const& callb_)
            {
                m_callback = callb_;
            }

            #pragma endregion
           
            #pragma region Process Interface

            void step()
            {   
                if(m_currState == RUNNING)
                    doStep();
                
                if(m_currState != RUNNING)
                 {
                    buildPath();
                    
                    // Callback
                    m_callback(m_resultPath);
                 }
            }
            
            /// Performs all steps at once
            void burnSteps()
            {

                while(m_currState == RUNNING)
                {
                    step();
                }
            }
           
            #pragma endregion

        private:
            
            void buildPath()
            {
                if(m_currState == FAILURE)
                {
                    m_resultPath = g_failPath;
                    return;
                }

                try
                { 
                    // Construct the path starting from the end

                    Index tmpIdx = m_targetIndex;
                
                    m_resultPath.m_nodeIndexes.push_back(m_targetIndex);

                    while(tmpIdx != m_startIndex)
                    {
                        tmpIdx = m_pathMap[tmpIdx];
                        m_resultPath.m_nodeIndexes.push_front(tmpIdx);
                    }
                }
                catch(std::exception_ptr& e)
                {
                    // Path map construction error
                    m_resultPath = g_failPath;
                }
            }

            void emptyCallback(ASPath const&)
            {}

            Index findLowestTotalCost()
            {
                // Note: windows.hpp defines min/max macros
                // which makes usage of std min/max and numeric_limits
                // unusable.

                float minCost = FLT_MAX; //std::numeric_limits<float>::max();
                
                // Index is typedef for size_t
                Index minIdx  = UINT_MAX; //std::numeric_limits<Index>::max(); 

                // O(N) can be reduced to O(log N) by sorting out the list
             
                for(Index idx : m_openSet)
                {
                    if(minCost>m_totalCost[idx] && m_totalCost[idx]!=0)
                    {
                        minCost = m_totalCost[idx];
                        minIdx = idx; 
                    }
                }

                if(minIdx == UINT_MAX)
                    throw std::exception();//    return m_startIndex;

                return minIdx;
            }

            inline bool isInClosedSet(Index idx)
            {
                return (std::find(m_closedSet.begin(),m_closedSet.end(),idx)!= m_closedSet.end());
            }

            inline bool isInOpenSet(Index idx)
            {
                return (std::find(m_openSet.begin(),m_openSet.end(),idx)!= m_openSet.end());
            }
            
            State doStep()
            {
                if(m_openSet.size() == 0)
                    return (m_currState = FAILURE);

                Index currIndex = findLowestTotalCost();

               

                if(currIndex == m_targetIndex)
                    return (m_currState = TERMINATED);

                m_openSet.erase(currIndex);
                m_closedSet.insert(currIndex);

                IndexArray& neighbors = m_graph->neighbors(currIndex);
                for(IndexArray_it it = neighbors.begin() ; 
                    it != neighbors.end() ; ++it)
                {
                    if(isInClosedSet(*it))
                        continue;
                    
                    int tmpCost = m_pastCost[currIndex] + m_graph->cost(currIndex,*it);

                    bool neighborIsOpen = isInOpenSet(*it);

                    if(!neighborIsOpen || tmpCost < m_pastCost[*it])
                    {
                        m_pathMap[*it] = currIndex;
                        m_pastCost[*it] = tmpCost;
                        m_totalCost[*it] = tmpCost + m_graph->estimate(*it,m_targetIndex);

                        if(!neighborIsOpen)
                            m_openSet.insert(*it);
                    }
                }

                return (m_currState = RUNNING);
            }
            
            ASGraph* m_graph;

            Index m_startIndex;
            Index m_targetIndex;

            /// Current state
            State m_currState;

            /// Set of node index already tested
            NodeSet m_closedSet;

            /// Set of node to test
            NodeSet m_openSet;

            NodeMap m_pathMap;

            /// Cost for moving to this index 
            CostArray m_pastCost;
            
            /// Estimation of cost for moving from this index
            /// to the target index
            CostArray m_estimCost;
            
            /// The sum of past cost and estimated cost
            /// That is estimated the cost of full path
            CostArray m_totalCost;

            /// Path result of search
            ASPath m_resultPath;

            /// Termination callback
            ck::Functor<void,ASPath const&> m_callback;

        };
    }
}

#endif //AI_FF_ASPathFinder_hpp