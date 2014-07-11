//
//  ComponentHolder.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_ComponentHolder_hpp
#define CYTOK_ComponentHolder_hpp

#include <vector>

#include "Cytok/Logs.hpp"
#include "Cytok/Exceptions.hpp"

namespace ck
{
    template<typename BaseClass,
             template<typename>class ListType = std::vector
            >
	class ComponentHolder
	{
    public:
        
        typedef ListType<BaseClass*> ComponentList;
        typedef typename ListType<BaseClass*>::iterator ComponentList_it;
        
        template<typename ComponentClass,typename... Args>
        void emplaceComponent(Args&&... args);
        virtual void addComponent(BaseClass* comp);
        
        virtual void removeComponent(int atIdx);
        
        virtual void removeComponent(BaseClass* comp);
		
        virtual int componentCount();
        
        virtual ComponentList& components();
        
        virtual BaseClass* component(int atIdx);
        
        template<typename Type>
        Type* findComponent();
        
        template<typename Type>
        ListType<Type*> findAllComponents();
 
        virtual BaseClass* findComponent(std::function<bool(BaseClass*)>& funct);
        
        virtual ComponentList findAllComponents(std::function<bool(BaseClass*)>& funct);
        
    protected:
        
        ComponentList mComponents;
        
	};
}

#include "ComponentHolder_impl.hpp"

#endif
