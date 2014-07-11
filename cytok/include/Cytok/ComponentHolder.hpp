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
        void emplaceComponent(Args&&... args)
        {
            if(!std::is_base_of<BaseClass,ComponentClass>::value)
            {
                throw ck::InheritanceException();
            }
            
            BaseClass* comp = new ComponentClass(std::forward<Args>(args)...);
            
            this->addComponent(comp);
        }
        
        virtual void addComponent(BaseClass* comp)
        {
            mComponents.push_back(comp);
        }
        
        virtual void removeComponent(int atIdx)
        {
            mComponents.erase(mComponents.begin()+atIdx);
        }
        
        virtual void removeComponent(BaseClass* comp)
        {
            ComponentList_it it =
                std::find(mComponents.begin() , mComponents.end(), comp);
            
            if(it == mComponents.end())
                return;
            
            mComponents.erase(it);
        }
		
        virtual int componentCount()
        {
            return mComponents.size();
        }
        
        virtual ComponentList& components()
        {
            return mComponents;
        }
        
        virtual BaseClass* component(int atIdx)
        {
            return mComponents[atIdx];
        }
        
        template<typename Type>
        Type* findComponent()
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
        
        template<typename Type>
        virtual ListType<Type*> findAllComponents()
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
 
        virtual BaseClass* findComponent(std::function<bool(BaseClass*)>& funct)
        {
            for(ComponentList_it it = mComponents.begin() ;
                it != mComponents.end() ; ++it)
            {
                if(funct(*it))
                    return *it;
            }
            
            return NULL;
        }
        
        virtual ComponentList findAllComponents(std::function<bool(BaseClass*)>& funct)
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
        
    protected:
        
        ComponentList mComponents;
        
	};
}

#endif
