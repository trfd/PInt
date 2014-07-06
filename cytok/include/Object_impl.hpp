//
//  Object_impl.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_Object_impl_hpp
#define CYTOK_Object_impl_hpp

#include "IDManager.hpp"

namespace ck
{
    template<typename This>
    void Object::fetchID(This thisObj)
    {
        myID = IDManager::get(thisObj);
    }

}

#endif
