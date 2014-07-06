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
        myID = CK_INVALID_ID;
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
}