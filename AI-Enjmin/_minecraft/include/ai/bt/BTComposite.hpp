///
///  BTComposite.hpp
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

#ifndef AI_BT_Composite_hpp
#define AI_BT_Composite_hpp

#include "../CKFunctor.hpp"

#include "BTBehaviour.hpp"
#include "BTCompositeTypes.hpp"

namespace ai
{
	namespace bt
	{
        #pragma region IComposite

        class IComposite
        : public Behaviour
        {
        public:
            virtual ~IComposite(){};

            virtual void addChild(Behaviour* child_) = 0;
            virtual void removeChild(Behaviour* child_) =0;
            virtual void removeAllChildren() = 0;
        };

        #pragma endregion

		#pragma region Composite

		template<template<class>class _ContainerType>
		class Composite
		: public IComposite
		{
		public:

			#pragma region Typedefs

			typedef _ContainerType<Behaviour*>	Container;
			typedef typename Container::iterator	Container_it;

			#pragma endregion

			virtual ~Composite();

			#pragma region Accessors
			
			inline Container& children();
            inline size_t childrenCount() const;
			
			#pragma endregion

			#pragma region Children List Mutation

			inline virtual void addChild(Behaviour* child_) override;
			// insert before
			inline virtual void insertChild(Container_it loc_, Behaviour* child_);
            inline virtual void removeChild(Behaviour* child_) override;
			inline virtual void removeChild(Container_it loc_);
            inline virtual void removeAllChildren() override;
			
			#pragma endregion

		private:

            inline static bool remove(Behaviour* child_);

			Container m_children;
		};

		#pragma endregion
	}
}

#include "BTComposite_impl.hpp"

#endif //AI_BT_Composite_hpp