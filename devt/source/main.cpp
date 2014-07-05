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

bool myfunction (ck::ID i,ck::ID j) { return (i.id<j.id); }

int main(int argc, const char * argv[])
{
    
    ck::Object obj = ck::Object();
    
    std::vector<ck::ID> ids;
    
    for(int i=0 ; i< UINT16_MAX ; i++)
    {
        ids.push_back( ck::IDManager::get(&obj));
    }
    
    std::sort (ids.begin(), ids.end(),myfunction);
    
    uint64_t collision = 0;
    uint16_t collisionUnique = 0;
    /*
    for (int i=0; i< ids.size(); i++)
    {
        for (int j=i+1; j< ids.size(); j++)
        {
            if(i!= j && ids[i].id == ids[j].id)
                collision++;
            
            if(i!= j && ids[i].stamps.unique == ids[j].stamps.unique)
                collisionUnique++;
        }
    }
     */
    
    for (int i=0; i< ids.size(); i++)
    {
        std::cout<<std::hex<<ids[i].id;
        
        if(i>0 && ids[i] == ids[i-1])
        {
            std::cout<<"<--- Collision";
            collision++;
        }
        
        std::cout<<"\n";
    }

    std::cout<<std::dec<<"Collisions: "<<collision<<" Frequency: "<<collision*1.f/ids.size()<<" Rate: 1:"<<ids.size()*1.f/collision<<"\n";
    //std::cout<<std::dec<<"CollisionsUnique: "<<collisionUnique<<" Frequency: "<<collisionUnique*1.f/ids.size()<<" Rate: 1:"<<ids.size()*1.f/collisionUnique<<"\n";
}

