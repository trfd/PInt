///
///  ASGraph.hpp
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

#ifndef AI_FF_ASGraph_hpp
#define AI_FF_ASGraph_hpp

#include <vector>
#include <list>


namespace ai
{
    namespace ff
    {
        typedef size_t               Index;
        typedef std::vector<Index>   IndexArray;
        typedef IndexArray::iterator IndexArray_it;
        typedef std::list<Index>     IndexList;
        typedef IndexList::iterator  IndexList_it;

        /// Interface for ASPathFinder
        class ASGraph
        {
        public:

            virtual ~ASGraph(){}

            /// Returns the number of nodes 
            /// in the graph
            virtual size_t count() = 0;

            /// Returns estimation of cost of path
            virtual float estimate(Index stIdx_, Index tgIdx_) = 0;

            /// Returns the node's neighbors
            virtual IndexArray& neighbors(Index idx_) = 0;

            /// Returns cost of adjacent nodes
            virtual float cost(Index fromIdx_, Index toIdx_) = 0;
        };
    }
}

#endif // AI_FF_ASGraph_hpp
