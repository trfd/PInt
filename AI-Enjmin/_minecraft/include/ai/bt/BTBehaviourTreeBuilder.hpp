///
///  BTBehaviourTreeBuilder.hpp
///
///  Created by Baptiste Dupy on 10/08/2014.
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

#ifndef AI_BT_BehaviourTreeBuilder_hpp
#define AI_BT_BehaviourTreeBuilder_hpp

#include <stack>

#include "BTComposite.hpp"
#include "BTBehaviourTree.hpp"

namespace ai
{
    namespace bt
    {
        class INodeHolder
        {
        public:
            virtual ~INodeHolder(){}
            virtual bool canAdd() = 0;
            virtual void add(Behaviour* child_) = 0;
            Behaviour* node;
        };

        template<typename _T>
        class BranchHolder : public INodeHolder
        {
        public:
            virtual bool canAdd() override { return true; }
            virtual void add(Behaviour* child_) override
            {
                static_cast<_T*>(node)->addChild(child_);
            }
        };

        template<typename _T>
        class LeafHolder : public INodeHolder
        {
        public:
            virtual bool canAdd() override {
                return false;
            }
            virtual void add(Behaviour* child_) override
            {
                // this type of behaviour doesn't support children
                static_assert(true,"Building error");
            }
        };

        template<typename _T>
        INodeHolder* __makeHolder(_T* node_)
        {
            typedef std::conditional<std::is_base_of<IComposite,_T>::value,BranchHolder<_T>,LeafHolder<_T>>::type HolderType;
            HolderType* newHolder = new HolderType;
            newHolder->node = node_;
            
            return newHolder;
        }

        class BehaviourTreeBuilder
        {
        public:

            virtual ~BehaviourTreeBuilder()
            {
            }

            Behaviour* root() { return m_root; }

            template<typename _Type,typename... _Args>
            BehaviourTreeBuilder& add(_Args&&... args)
            {
                _Type* newNode = new _Type(std::forward<_Args>(args)...);

                if(!m_root)
                {
                    m_root = newNode;
                    m_currNode = __makeHolder(newNode);
                    m_nodeStack.push(m_currNode);
                }
                else
                    add(newNode);
                
                return *this;
            }
            
            BehaviourTreeBuilder& parent()
            {
                m_nodeStack.pop();
                m_currNode = m_nodeStack.top();
                return *this;
            }
            
            BehaviourTree end()
            {
                return BehaviourTree(m_root);
            }

        private:

            template<typename _Type>
            void add(_Type* newNode)
            {
                m_currNode->add(newNode);

                INodeHolder* newholder = __makeHolder(newNode);

                if(newholder->canAdd())
                {
                    m_currNode = newholder;
                    m_nodeStack.push(m_currNode);
                }
                else
                    delete newholder;
            }

            Behaviour* m_root = nullptr;
            std::stack<INodeHolder*> m_nodeStack;
            INodeHolder* m_currNode = nullptr;

        };
    }
}

#endif // AI_BT_BehaviourTreeBuilder_hpp