//
//  Object.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 05/07/2014.
//
//

#include <sstream>

#include "Object.hpp"

#include "IDManager.hpp"

namespace ck
{
    Object::Object()
    {
        if(myID == CK_INVALID_ID)
        {
            /// WARNING: Possible incorrect ID
            /// If derived class forget about fetching
            /// the ID, the ck::Object will set using this.
            /// But this has a type of ck::Object at this
            /// location. Thus ID will not be relevant.
            IDManager::get(type().hash_code());
        }
    }
    
    
    ID Object::id()
    {
        return myID;
    }
    
    std::string Object::toString()
    {
        std::ostringstream out;
        
        out << "Object [id:"<<std::hex<<myID.id<<"] Address: 0x"<<this<<std::endl;
        
        return out.str();
    }
    
    const std::type_info& Object::type()
    {
        return typeid(Object);
    }
}