//
//  Object_impl.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_Object_impl_hpp
#define CYTOK_Object_impl_hpp

#include <type_traits>

#include "IDManager.hpp"

namespace ck
{
    template<typename ObjectType , typename... Args>
    ObjectType* Object::create(Args... args)
    {
        if(std::is_base_of<ck::Object, ObjectType>::value)
        {
            ObjectType* obj = new ObjectType(args...);
            
            obj->myID = IDManager::get<ObjectType>();
            
            obj->init();
            
            return obj;
        }
        
        return NULL;
    }

}

#endif
