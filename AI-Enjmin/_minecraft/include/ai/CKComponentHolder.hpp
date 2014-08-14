///  
///  ComponentHolder.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 11/07/2014.
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

#ifndef CYTOK_ComponentHolder_hpp
#define CYTOK_ComponentHolder_hpp

#include <vector>
#include <functional>
#include <set>

#include "CKPair.hpp"

namespace ck
{
    template
	<
		typename _BaseClass  
	>
	class ComponentHolder
	{
    public:

        #pragma region Typedefs

        typedef ck::Pair<int, _BaseClass*, ck::Compare::FIRST, ck::Equal::SECOND> PriorityPair;
        typedef std::set<PriorityPair> ComponentList;
        typedef typename ComponentList::iterator ComponentList_it;
       
        #pragma endregion

        #pragma region Accessors

        virtual int componentCount();

        virtual ComponentList& components();

        //virtual _BaseClass* component(int atIdx);

        #pragma endregion

        #pragma region Component List Mutation

        template<typename _ComponentClass,typename... _Args>
        _ComponentClass* emplaceComponent(int priority,_Args&&... args);
       
       /// Returns whether or not the component has been added
        virtual bool addComponent(int priority, _BaseClass* comp);
        
        //virtual void removeComponent(int atIdx);
        
        virtual void removeComponent(_BaseClass* comp);
		
        #pragma endregion
    
        #pragma region Utils
        
        template<typename _Type>
        _Type* findComponent();
        
        template<typename _Type>
        std::vector<_Type*> findAllComponents();

        virtual _BaseClass* findComponent(std::function<bool(_BaseClass*)>& funct);
        
        virtual std::vector<_BaseClass*> findAllComponents(std::function<bool(_BaseClass*)>& funct);
        
        #pragma endregion

    protected:
        
        #pragma region Override interface

        virtual void onAddComponent(_BaseClass* comp_){}

        virtual void onRemoveComponent(_BaseClass* comp_){}

        #pragma endregion

        ComponentList _components;
	};

}

#include "CKComponentHolder_impl.hpp"

#endif
