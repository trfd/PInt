//
//  NumericTypes.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 04/07/2014.
//
//

#ifndef CYTOK_NumericTypes_hpp
#define CYTOK_NumericTypes_hpp

#include <cstdint>

namespace ck
{
    /// Typedef unsigned types
    
    typedef unsigned short byte;
    typedef unsigned short ushort;
	typedef unsigned int uint;
    typedef unsigned long ulong;
    
    /// Typedef sized int
    
    template<int n>
    struct uint_t;
    
    template<>
    struct uint_t<8>
    {
        typedef uint8_t type;
    };
    
    template<>
    struct uint_t<16>
    {
        typedef uint16_t type;
    };

    template<>
    struct uint_t<32>
    {
        typedef uint32_t type;
    };

    
    template<>
    struct uint_t<64>
    {
        typedef uint64_t type;
    };

}

#endif
