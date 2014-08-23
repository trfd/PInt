///
///  DtAdjacencyList.hpp
///  part of CytokGen Library.
///
///  CytokGen Library is the subpart of CytokEngine dedicated to
///  the procedural generation systems.
///  CytokEngine is a research game-engine project.
///
///  Created by Baptiste Dupy on 08/07/2014.
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

#ifndef CYTOK_GEN_DtAdjacencyList_hpp
#define CYTOK_GEN_DtAdjacencyList_hpp

#include <vector>

namespace ck
{
    namespace gen
    {
        namespace data
        {
            template<typename _DataNode>
            class AdjacencyList
            {
            public:
                
                class Node;

                typedef std::vector<Node*> NodeList;
                typedef typename std::vector<Node*>::iterator NodeList_it;

                class Node
                {
                    friend class AdjacencyList;
                    
                public:
                    
                    bool isAdjacent(Node* node)
                    {
                        return (std::find(m_adjNodes.begin(), m_adjNodes.end(), node)
                                != m_adjacencies.end());
                    }
                    
                    _DataNode data()
                    {
                        return m_data;
                    }
                    
                    void setData(_DataNode&& data)
                    {
                        m_data = std::move(data);
                    }

                    void setData(_DataNode const& data)
                    {
                        m_data = data;
                    }
                    
                    size_t edgeCount()
                    {
                        return m_adjNodes.size();
                    }
                    
                    Node* adjacentNodeAt(int idx)
                    {
                        return m_adjNodes[idx];
                    }
                    
                private:
                    
                    void addEdge(Node* node)
                    {
                        NodeList_it it = std::find(m_adjNodes.begin(),
                                                   m_adjNodes.end(),
                                                   node);
                        
                        if(it != m_adjNodes.end())
                            return;
                        
                        m_adjNodes.push_back(node);
                    }

                    void removeEdge(Node* node)
                    {
                        NodeList_it it = std::find(m_adjNodes.begin(),
                                                   m_adjNodes.end(),
                                                   node);

                        if(it == m_adjNodes.end())
                            return;

                        m_adjNodes.erase(it);
                    }
          
                    NodeList m_adjNodes;
                    
                    _DataNode m_data;
                };

                size_t nodeCount()
                {
                    return m_list.size();
                }
                
                Node* addNode()
                {
                    Node* n = new Node();
                    m_list.push_back(n);
                    return n;
                }

                Node* addNode(_DataNode&& dt)
                {
                    Node* n = new Node();
                    n->setData(std::forward<_DataNode>(dt));
                    m_list.push_back(n);
                    return n;
                }

                Node* addNode(_DataNode const& dt)
                {
                    Node* n = new Node();
                    n->setData(dt);
                    m_list.push_back(n);
                    return n;
                }
                
                void removeNode(Node* n)
                {
                    for(NodeList_it it = m_list.begin() ;
                        it != m_list.end() ; ++it)
                    {
                        removeEdge(n);
                    }

                    NodeList_it it = std::find(m_list.begin(),m_list.end(),n);

                    m_list.erase(it);
                }

                int indexOf(Node* node)
                {
                    NodeList_it it = std::find(m_list.begin(),m_list.end(),node);

                    if(it == m_list.end())
                        return INT_MAX;

                    return std::distance(m_list.begin() , it);
                }

                template<typename _U>
                size_t indexOfData(_U&& d_)
                {
                   for(auto it = m_list.begin() ;
                       it != m_list.end() ; ++it)
                    {
                        if((*it)->data() == d_)
                            return std::distance(m_list.begin(), it);
                    }
                    return UINT_MAX;
                }

                Node* nodeAt(int idx)
                {
                    return m_list[idx];
                }

                template<typename _U>
                Node* nodeWithData(_U&& d_)
                {
                    for(Node* n : m_list)
                    {
                        if(n->data() == d_)
                            return n;
                    }
                    return nullptr;
                }
                
                template<typename _U,typename _V>
                void addEdge(_U&& d1_, _V&& d2_)
                {
                    Node* n1 = nodeWithData(d1_);
                    Node* n2 = nodeWithData(d2_);

                    if(!n1 || !n2)
                        throw std::exception();
                    
                    addEdge(n1,n2);
                }

                void addEdge(Node* n1, Node* n2)
                {
                    NodeList_it it1 = std::find(m_list.begin() , m_list.end() , n1);
                    
                    if(it1 == m_list.end())
                        return;
                    
                   NodeList_it it2 = std::find(m_list.begin() , m_list.end() , n2);
                    
                    if(it2 == m_list.end())
                        return;
                    
                    addEdge(it1, it2);
                }

                inline void addEdge(int n1, int n2)
                {
                    addEdge(m_list.begin() + n1,
                            m_list.begin() + n2);
                }

                inline void removeEdge(Node* n1, Node* n2)
                {
                    NodeList_it it1 = std::find(m_list.begin(), m_list.end(), n1);

                    if(it1 == m_list.end())
                        return;

                    NodeList_it it2 = std::find(m_list.begin(), m_list.end(), n2);

                    if(it2 == m_list.end())
                        return;

                    removeEdge(it1, it2);
                }

                inline void removeEdge(int n1, int n2)
                {
                    removeEdge(m_list.begin() + n1,
                               m_list.begin() + n2);
                }
                
                //....
                
            private:
                
                inline void addEdge(NodeList_it it1,
                                    NodeList_it it2)
                {   
                    (*it1)->addEdge(*it2);
                    (*it2)->addEdge(*it1);
                }

                inline void removeEdge(NodeList_it it1,
                                       NodeList_it it2)
                {
                    (*it1)->removeEdge(*it2);
                    (*it2)->removeEdge(*it1);
                }
                
                NodeList m_list;
            };

        }
    }
}

#endif
