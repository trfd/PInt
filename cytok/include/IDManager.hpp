//
//  IDManager.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 04/07/2014.
//
//

#ifndef CYTOK_IDManager_hpp
#define CYTOK_IDManager_hpp

#include <iostream>
#include <ctime>
#include <cstdint>
#include <typeinfo>

#include "Hash.hpp"
#include "ID.hpp"
#include "IDException.hpp"

namespace ck
{
    class Object;
    
    class IDManager
    {
    public:
        
        /// WARNING: May not be suitable
        /// for online server.
        /// Need solid unit testing
        template<typename Type>
        static ID get(Type obj)
        {
            ID myID;
            
            // Get Current Global Time
            time_t currTime;
            time(&currTime);
            struct tm* currGTime = gmtime(&currTime);
            
            myID.stamps.type = utils::hash<16>::get(typeid(Type).hash_code());
            myID.stamps.unique = ++mCount;
            myID.stamps.time = utils::hash<32>::get(currGTime->tm_sec-sDay0.tm_sec);
            
            if(myID == gInvalidID)
                throw new InvalidIDException();
            
            return myID;
        }
        
    private:
        
        /// Time since 1 January 2014
        static const struct tm sDay0;
        
        static int mCount;
    };
    
    int IDManager::mCount = 0;
    
    const struct tm IDManager::sDay0 =
    { .tm_hour = 0 ,   .tm_min = 0 , .tm_sec = 0 ,
      .tm_year = 114 , .tm_mon = 0 , .tm_mday = 1
    };

}

#endif
