//
//  ComponentHolder_impl.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_ComponentHolder_impl_hpp
#define CYTOK_ComponentHolder_impl_hpp

namespace ck
{
	template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    template
    <
        typename ComponentClass,
        typename... Args
    >
    void ComponentHolder<BaseClass,ListType>::emplaceComponent(Args&&... args)
    {
        if(!std::is_base_of<BaseClass,ComponentClass>::value)
        {
            throw ck::InheritanceException();
        }
        
        BaseClass* comp = new ComponentClass(std::forward<Args>(args)...);
        
        this->addComponent(comp);
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    void ComponentHolder<BaseClass,ListType>::addComponent(BaseClass* comp)
    {
        mComponents.push_back(comp);
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    void ComponentHolder<BaseClass,ListType>::removeComponent(int atIdx)
    {
        mComponents.erase(mComponents.begin()+atIdx);
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    void ComponentHolder<BaseClass,ListType>::removeComponent(BaseClass* comp)
    {
        ComponentList_it it =
        std::find(mComponents.begin() , mComponents.end(), comp);
        
        if(it == mComponents.end())
            return;
        
        mComponents.erase(it);
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    int ComponentHolder<BaseClass,ListType>::componentCount()
    {
        return mComponents.size();
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    typename ComponentHolder<BaseClass,ListType>::ComponentList&
    ComponentHolder<BaseClass,ListType>::components()
    {
        return mComponents;
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    BaseClass* ComponentHolder<BaseClass,ListType>::component(int atIdx)
    {
        return mComponents[atIdx];
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    template<typename Type>
    Type* ComponentHolder<BaseClass,ListType>:: findComponent()
    {
        Type* result;
        
        for(ComponentList_it it = mComponents.begin() ;
            it != mComponents.end() ; ++it)
        {
            if((result = dynamic_cast<Type*>(*it)))
                return result;
        }
        
        return NULL;
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    template<typename Type>
    ListType<Type*> ComponentHolder<BaseClass,ListType>::findAllComponents()
    {
        ComponentList tmpList;
        Type* tmpR;
        for(ComponentList_it it = mComponents.begin() ;
            it != mComponents.end() ; ++it)
        {
            if((tmpR = dynamic_cast<Type*>(*it)))
                tmpList.push_back(tmpR);
        }
        
        return tmpList;
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    BaseClass* ComponentHolder<BaseClass,ListType>
    ::findComponent(std::function<bool(BaseClass*)>& funct)
    {
        for(ComponentList_it it = mComponents.begin() ;
            it != mComponents.end() ; ++it)
        {
            if(funct(*it))
                return *it;
        }
        
        return NULL;
    }
    
    template
    <
        typename BaseClass,
        template<typename>class ListType
    >
    typename ComponentHolder<BaseClass,ListType>::ComponentList
    ComponentHolder<BaseClass,ListType>::findAllComponents(std::function<bool(BaseClass*)>& funct)
    {
        ComponentList tmpList;
        
        for(ComponentList_it it = mComponents.begin() ;
            it != mComponents.end() ; ++it)
        {
            if(funct(*it))
                tmpList.push_back(*it);
        }
        
        return tmpList;
    }
    
}

#endif
