///  
///  ComponentHolder_impl.hpp
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

#ifndef CYTOK_ComponentHolder_impl_hpp
#define CYTOK_ComponentHolder_impl_hpp



namespace ck
{

    #pragma region Accessors

    template<typename _BaseClass>
    int ComponentHolder<_BaseClass>
    ::componentCount()
    {
        return _components.size();
    }

    template<typename _BaseClass>
    typename ComponentHolder<_BaseClass>::ComponentList&
        ComponentHolder<_BaseClass>
    ::components()
    {
        return _components;
    }
    /*
    template<typename _BaseClass>
    _BaseClass* ComponentHolder<_BaseClass>
    ::component(int atIdx)
    {
        return _components[atIdx];
    }
    */
    #pragma endregion

    #pragma region Component List Mutation

    template<typename _BaseClass>
    template<typename _ComponentClass,typename... _Args>
    _ComponentClass* ComponentHolder<_BaseClass>
    ::emplaceComponent(int priority_, _Args&&... args )
    {
        if(!std::is_base_of<_BaseClass,_ComponentClass>::value)
        {
            //throw ck::InheritanceException();
            throw std::exception();
        }
        
        _ComponentClass* comp = new _ComponentClass(std::forward<_Args>(args)...);
        
        if(this->addComponent(priority_,comp))
            return comp;

        return nullptr;
    }
    
    template<typename _BaseClass>
    bool ComponentHolder<_BaseClass>
        ::addComponent(int priority_, _BaseClass* comp_)
    {
        std::pair<ComponentList_it, bool> plct =
        _components.insert(PriorityPair(priority_,comp_));

        if(plct.second)
            onAddComponent(comp_);

        return plct.second;
    }

    /*
    template<typename _BaseClass>
    void ComponentHolder<_BaseClass>
    ::removeComponent(int atIdx)
    {
        onRemoveComponent(*(_components.begin() + atIdx));

        _components.erase(_components.begin()+atIdx);
    }
    */
    
    template<typename _BaseClass>
    void ComponentHolder<_BaseClass>
    ::removeComponent(_BaseClass* comp)
    {
        ComponentList_it it =
        std::find(_components.begin() , _components.end(), comp);
        
        if(it == _components.end())
            return;
        
        onRemoveComponent(it->second);

        _components.erase(it);
    }

    #pragma endregion

    #pragma region Utils

    template<typename _BaseClass>
    template<typename _Type>
    _Type* ComponentHolder<_BaseClass>
    ::findComponent()
    {
        _Type* result;
        
        for(ComponentList_it it = _components.begin() ;
            it != _components.end() ; ++it)
        {
            if((result = dynamic_cast<_Type*>(it->second)))
                return result;
        }
        
        return NULL;
    }
    
    template<typename _BaseClass>
    template<typename _Type>
    std::vector<_Type*> ComponentHolder<_BaseClass>
	::findAllComponents()
    {
        std::vector<_Type*> tmpList;
        _Type* tmpR;
        for(ComponentList_it it = _components.begin() ;
            it != _components.end() ; ++it)
        {
            if((tmpR = dynamic_cast<_Type*>(it->second)))
                tmpList.push_back(tmpR);
        }
        
        return tmpList;
    }
    
    template<typename _BaseClass>
    _BaseClass* ComponentHolder<_BaseClass>
    ::findComponent(std::function<bool(_BaseClass*)>& funct)
    {
        for(ComponentList_it it = _components.begin() ;
            it != _components.end() ; ++it)
        {
            if(funct(it->second))
                return it->second;
        }
        
        return NULL;
    }
    
    template<typename _BaseClass>
    std::vector<_BaseClass*>
    ComponentHolder<_BaseClass>
	::findAllComponents(std::function<bool(_BaseClass*)>& funct)
    {
        std::vector<_BaseClass*> tmpList;
        
        for(ComponentList_it it = _components.begin() ;
            it != _components.end() ; ++it)
        {
            if(funct(it->second))
                tmpList.push_back(it->second);
        }
        
        return tmpList;
    }
    
    #pragma endregion
}

#endif
