///
///  ASPathFinder.hpp
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

#ifndef AI_FF_ASPath_hpp
#define AI_FF_ASPath_hpp

#include "ai/CKUMReturnType.hpp"
#include "ASGraph.hpp"

using namespace ck::utils::meta;

namespace ai
{
    namespace ff
    {
        class ASPathFinder;

        template<typename _NodeType>
        class ASPath;

        template<typename _NodeType>
        class ASPathIterator
        {
            friend ASPath<_NodeType>;

            typedef std::list<_NodeType>                 NodeList;
            typedef typename NodeList::iterator          NodeList_it;
            typedef typename NodeList::const_iterator    NodeList_cit;
            typedef typename ReturnType<_NodeType>::type ReturnType;

            ASPathIterator(const ASPath<_NodeType>* path_, NodeList_cit & it_)
            : m_path(path_), 
              m_list_it(unconst(m_path,it_)) 
            {}

        public:

            static NodeList_it unconst(const ASPath<_NodeType>* path_,const NodeList_cit& cit)
            {
                NodeList_it it = const_cast<ASPath<_NodeType>*>(path_)->m_nodes.begin();
                std::advance(it, std::distance<NodeList_cit>(path_->m_nodes.begin(), cit));
                return it; 
            }
 
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
            
            inline ReturnType node()
            {
                return *m_list_it;
            }

            ReturnType operator*()
            {
                return *m_list_it;
            }

         private:
            
            /// Parent path
            const ASPath<_NodeType>* m_path;

            /// List iterator of parent path's list
            NodeList_it m_list_it; 

        };

        template<typename _NodeType>
        class ASPath
        {
            friend ASPathFinder;
            friend ASPathIterator<_NodeType>;

        public:

            typedef ASPathIterator<_NodeType>   iterator;
            typedef std::list<_NodeType>        NodeList;
            typedef typename NodeList::iterator NodeList_it;


            ASPath()
            : m_graph(nullptr)
            {}

            ASPath(ASGraph* graph_)
            : m_graph(graph_)
            {}

            iterator begin() const
            {
                return iterator(this, m_nodes.begin());
            }

            iterator end() const
            {
                return iterator(this, m_nodes.end());
            }

            NodeList& nodeList() { return m_nodes; }

        private:
            
            /// Parent graph
            ASGraph* m_graph; 
         
            /// List of node indexes
            NodeList m_nodes;
        };

    }
}

#endif //AI_FF_ASPath_hpp