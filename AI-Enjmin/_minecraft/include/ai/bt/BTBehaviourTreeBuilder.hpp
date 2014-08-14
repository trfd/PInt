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

#include "BTComposite.hpp"
#include "BTBehaviourTree.hpp"

namespace ai
{
    namespace bt
    {
        class IBuilder_impl
        {
        public:
            virtual ~IBuilder_impl(){}
            virtual bool canAdd() = 0;
            virtual void add(Behaviour* parent_, Behaviour* child_) = 0;
        };

        template<typename _T>
        class ChildrenBuilder_impl
            : public IBuilder_impl
        {
        public:
            virtual bool canAdd() override { return true; }
            virtual void add(Behaviour* parent_, Behaviour* child_) override
            {
                static_cast<_T*>(parent_)->addChild(child_);
            }
        };

        template<typename _T>
        class LeafBuilder_impl
            : public IBuilder_impl
        {
        public:
            virtual bool canAdd() override {
                return false;
            }
            virtual void add(Behaviour* parent_, Behaviour* child_) override
            {
                // this type of behaviour doesn't support children
            }
        };


        class BehaviourTreeBuilder
        {
        public:

            BehaviourTreeBuilder(IComposite* root_)
            : m_root(root_),
            m_impl(new ChildrenBuilder_impl<IComposite>())
            {}
           
            template<typename _T>
            BehaviourTreeBuilder(_T* root_)
            : m_root(root_),
            m_impl(new LeafBuilder_impl<_T>())
            {}

            virtual ~BehaviourTreeBuilder()
            {
                delete m_impl;
            }

            Behaviour* root() { return m_root; }

            template<typename _Type,typename... _Args>
            BehaviourTreeBuilder& add(_Args&&... args)
            {
                _Type* newNode = new _Type(std::forward<Args>(args)...);
               
                if(m_impl->canAdd())
                {
                    m_impl->add(m_root, newNode);
                    BehaviourTreeBuilder newBhBuilder(newNode);
                    newBhBuilder.m_parent = this;
                    return newBhBuilder;
                }
                return *this;
            }

            BehaviourTreeBuilder& parent()
            {
                return *m_parent;
            }

            BehaviourTree end()
            {
                return BehaviourTree(m_root);
            }

        private:
            Behaviour* m_root;
            BehaviourTreeBuilder* m_parent = NULL;
            IBuilder_impl* m_impl;
        };
    }
}

#endif // AI_BT_BehaviourTreeBuilder_hpp