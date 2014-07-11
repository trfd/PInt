//
//  ClassType.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#include "Cytok/ClassType.hpp"

namespace ck
{
    ClassType::ClassType(const std::string& className ,
                         ClassType* rootClass)
    :  myRootClass(rootClass) ,myName(className)
    {
        if(myRootClass)
            myFullname = myRootClass->myFullname+"-"+myName;
        else
            myFullname = myName;
        
        myID = utils::hash<32>::get(myFullname);
    }
    
    ClassType* ClassType::root()
    {
        return myRootClass;
    }
    
    std::string& ClassType::fullname()
    {
        return myFullname;
    }
    
    std::string& ClassType::name()
    {
        return myName;
    }
    
    uint32_t ClassType::id()
    {
        return myID;
    }
}