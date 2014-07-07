//
//  ID.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 05/07/2014.
//
//

#ifndef CYTOK_ID_hpp
#define CYTOK_ID_hpp

namespace ck
{    
    union ID
    {
        struct
        {
            uint16_t type;
            uint16_t unique;
            uint32_t time;
        }stamps;
        
        uint64_t id;
        
        /// Overload operators
        
        inline ID operator =(uint64_t a)
        {
            ID m = {.id = a};
            return m;
        }
        
        inline bool operator ==(ID a)
        {
            return a.id == this->id;
        }
    };
    
    /// Invalid ID constant
    const ID gInvalidID = { .id = UINT64_MAX };
}

#define CK_INVALID_ID ck::gInvalidID

#endif
