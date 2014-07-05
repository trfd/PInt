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
        myID = IDManager::get(this);
        std::cout<<"OOO:"<<std::hex<<myID.id<<"\n";
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