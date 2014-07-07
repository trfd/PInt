//
//  IDManager.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#include "Cytok/IDManager.hpp"

#include "Cytok/IDException.hpp"

namespace ck
{
    /// Inits
    
    int IDManager::myCount = 0;
    int IDManager::myLastSeconds;
    
    const struct tm IDManager::sDay0 =
    {
      .tm_hour = 0 ,   .tm_min = 0 , .tm_sec = 0 ,
      .tm_year = 114 , .tm_mon = 0 , .tm_mday = 1
    };
    
    /// Implementations
    
    ID IDManager::get(size_t typeHashCode)
    {
        ID myID;
        
        // Get Current Global Time
        time_t currTime;
        time(&currTime);
        struct tm* currGTime = gmtime(&currTime);
        
        int currSeconds =currGTime->tm_sec-sDay0.tm_sec;
        
        if(currSeconds == myLastSeconds)
            myCount++;
        else
            myCount = 0;
        
        if(myCount >= UINT16_MAX)
            throw new OverflowIDException();
        
        myLastSeconds = currSeconds;
        
        myID.stamps.type = utils::hash<16>::get(typeHashCode);
        myID.stamps.unique = myCount;
        myID.stamps.time = utils::hash<32>::get(myLastSeconds);
        
        if(myID == gInvalidID)
            throw new InvalidIDException();
        
        return myID;
    }
}