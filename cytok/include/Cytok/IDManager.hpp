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

#include "Cytok/Hash.hpp"
#include "Cytok/ID.hpp"


namespace ck
{
    class Object;
    
    /// Every new second the count is reset to zero.
    /// Thus when asked for ID several time during the
    /// same second the counter will keep track of the
    /// ID with same time. When the count of ID with
    /// the same time stamps (i.e. generated during the same
    /// second is greater than UINT16_MAX the
    /// manager throws an OverflowIDException
    class IDManager
    {
    public:
        
        /// WARNING: May not be suitable
        /// for online server.
        /// Need solid unit testing
        template<typename Type>
        static ID get() { return get(typeid(Type).hash_code());}
        
        static ID get(size_t typeHashCode);
        
    private:
        
        /// Time since 1 January 2014
        static const struct tm sDay0;
        
        static int myCount;
        
        /// The seconds when last generate an ID
        /// Use to keep track of overflow
        static int myLastSeconds;
    };

}

#endif
