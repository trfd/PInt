//
//  main.cpp
//  GBJ
//
//  Created by Baptiste Dupy on 24/10/13.
//  Copyright (c) 2013 Baptiste Dupy. All rights reserved.
//

#include <iostream>
#include <vector>

#include "Cytok/Hash.hpp"

#include "Cytok/Object.hpp"

#include "Cytok/IDManager.hpp"

#include "Cytok/Logs.hpp"

#include "Cytok/utils/VectorExtension.hpp"

class Test : public ck::Object
{
    int a;
    
public:
    
    virtual void description(std::ostringstream& stream)
    {
        Object::description(stream);
        
        stream<<" Hello World";
    }
    
};


int main(int argc, const char * argv[])
{
    ck::Object* obj = ck::Object::create<Test>();
    
    obj->describe();

}

