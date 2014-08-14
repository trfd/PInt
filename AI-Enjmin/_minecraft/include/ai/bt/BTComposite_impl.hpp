///
///  BTBehaviours_impl.hpp
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


#ifndef AI_BT_Behaviours_impl_hpp
#define AI_BT_Behaviours_impl_hpp

namespace ai
{
	namespace bt
	{
		#pragma region Composite

        template<template<class>class _ContainerType>
        inline bool
        Composite<_ContainerType>
        ::remove(Behaviour* child_)
        {
            delete child_;
            return true;
        }

        template<template<class>class _ContainerType>
        Composite<_ContainerType>
        ::~Composite()
        {
            m_children.remove_if(remove);
            m_children.clear();
        }

		template<template<class>class _ContainerType>
        inline 
        typename Composite<_ContainerType>::Container&
		Composite<_ContainerType>
		::children()
		{
			return m_children;
		}

		template<template<class>class _ContainerType>
        inline size_t
         Composite<_ContainerType>
		::childrenCount() const
		{
			return m_children.size();
		}

		template<template<class>class _ContainerType>
		inline void 
        Composite<_ContainerType>
		::addChild(Behaviour* child_)
		{
			m_children.push_back(child_);
		}

		template<template<class>class _ContainerType>
		inline void 
        Composite<_ContainerType>
		::insertChild(Container_it loc_, Behaviour* child_)
		{
			// Complexity of push_back in vector
			// and push_back/push_front in deque are O(1)
			// Worth testing if insertion point is 
			// not back or front (in this order)
			if(loc_ == m_children.end())
				m_children.push_back(child_);
			else if (loc_ == m_children.begin())
				m_children.push_front(child_);
			else
				m_children.insert(loc_,child_);
		}

		template<template<class>class _ContainerType>
		inline void 
        Composite<_ContainerType>
		::removeChild(Behaviour* child_)
		{
			removeChild(std::find(m_children.begin(),
						       m_children.end(), 
							   child_));
		}

		template<template<class>class _ContainerType>
		inline void 
        Composite<_ContainerType>
		::removeChild(Container_it loc_)
		{
            if(loc_ == m_children.end())
                return;

			// Compisite owns its children

			remove(*loc_);
			m_children.erase(loc_);
		}

        template<template<class>class _ContainerType>
        inline void  
        Composite<_ContainerType>
        ::removeAllChildren()
        {
            m_children.remove_if(remove);
            m_children.clear();
        }

		#pragma endregion
	}
}

#endif //AI_BT_Behaviours_impl_hpp