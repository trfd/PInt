//
//  main.cpp
//  GBJ
//
//  Created by Baptiste Dupy on 24/10/13.
//  Copyright (c) 2013 Baptiste Dupy. All rights reserved.
//

#include <iostream>
#include <vector>

#include "Hash.hpp"

#include "Object.hpp"

#include "IDManager.hpp"

#include "Log.hpp"
#include "Logger.hpp"
#include "LogStream.hpp"
#include "LoggerStd.hpp"
#include "LogMacros.hpp"

class Test : public ck::Object
{
    int a;
};


int main(int argc, const char * argv[])
{

    Test obj1;
    ck::Object obj2;
    
    
    LOG_INFO<<"Object 2:"<<std::hex<<obj2.id().id<<LOG_END;
    
    LOG_INFO<<"Object 1:"<<std::hex<<obj1.id().id<<LOG_END;

}

