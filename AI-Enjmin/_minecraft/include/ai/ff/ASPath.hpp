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

#ifndef AI_FF_ASPath_hpp
#define AI_FF_ASPath_hpp

#include "ASGraph.hpp"

namespace ai
{
    namespace ff
    {
        class ASPathFinder;

        class ASPathIterator
        {
            friend ASPath;

            ASPathIterator(ASPath* path_, IndexList_it const& it_)
            : m_path(path_), m_list_it(it_)
            {}

        public:
            
            ASPathIterator()
            : m_path(nullptr)
            {}
            
            ASPathIterator(const ASPathIterator& it)
            : m_path(it.m_path), m_list_it(it.m_list_it)
            {}
            
            ////  Operators int
            
            ASPathIterator& operator++()
            {
                m_list_it++;
                return *this;
            }
            
            ASPathIterator operator++(int)
            {
                ASPathIterator tmp (*this);
                operator++();
                return tmp;
            }
            
            ASPathIterator& operator--()
            {
                m_list_it--;
                return *this;
            }
            
            ASPathIterator operator--(int)
            {
                ASPathIterator tmp (*this);
                operator++();
                return tmp;
            }
          
            
            ////  Operators comparaisons
            
            bool operator==(const ASPathIterator& rhs) const
            {
                return (m_path    == rhs.m_path   && 
                        m_list_it == rhs.m_list_it);
            }
            
            bool operator!=(const ASPathIterator& rhs) const
            {
                return (m_path    != rhs.m_path   ||
                        m_list_it != rhs.m_list_it);
            }
            
            inline Index index()
            {
                return *m_list_it;
            }

            /* Must template ASPath and ASPathFinder
            inline _NodeType& operator*();
            
            inline _NodeType* operator->();
            */
         private:
            
            /// Parent path
            ASPath* m_path;

            /// List iterator of parent path's list
            IndexList_it m_list_it; 

        };

        class ASPath
        {
            friend ASPathFinder;
            friend ASPathIterator;

        public:

            typedef ASPathIterator iterator;

            ASPath(ASGraph* graph_)
            : m_graph(graph_)
            {}

            iterator begin() const
            {
                return ASPathIterator(this, m_nodeIndexes.begin());
            }

            iterator end() const
            {
                return ASPathIterator(this, m_nodeIndexes.end());
            }

        private:
            
            /// Parent graph
            ASGraph* m_graph; 
         
            /// List of node indexes
            IndexList m_nodeIndexes;
        };

     
    }
}

#endif //AI_FF_ASPath_hpp