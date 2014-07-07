//
//  Object.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 05/07/2014.
//
//

#include <sstream>

#include "Cytok/Object.hpp"

#include "Cytok/IDManager.hpp"

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
        std::ostringstream stream;
        
        this->description(stream);

        stream<<"\n";
        
        return stream.str();
    }
    
    void Object::description(std::ostringstream& stream)
    {
        stream << "Object ClassID="<<typeid(*this).name()<<" [id:"<<std::hex<<myID.id<<"] Address=0x"<<this;
    }
    
    void Object::describe(Log::ContextInfo ctx, Log::Type logType)
    {
        Log::message(logType, this->toString() , ctx);
    }
}