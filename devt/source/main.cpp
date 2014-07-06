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

int main(int argc, const char * argv[])
{
    //ck::Log::setLogger(new ck::LoggerStd);
    
    ck::Log::error("MyInfo");
    
    ck::LogStream stream(ck::Log::ERROR);
    
    stream<<"OOF"<<ck::LogStream::end;
    stream<<"OO1"<<"OO2"<<ck::LogStream::end;
    
    ck::LogStream::getDefault(ck::Log::WARNING)<<"Hello";
    
    ck::LogStream::getDefault(ck::Log::INFO)<<"World"<<ck::LogStream::end;
    
    LOG_INFO<<"OhMy"<<ck::LogStream::end;
    
    CK_LOG_INFO_I(ck::Log::GRAPHICS)<<"OhMy"<<ck::LogStream::end;
    
    LOG_EXCEPT<<(new std::bad_alloc())<<LOG_END;
    
    
}

