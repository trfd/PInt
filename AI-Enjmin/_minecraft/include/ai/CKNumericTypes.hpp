///  
///  NumericTypes.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 04/07/2014.
///  Contact:
///          Mail:       <baptiste.dupy@gmail.com>
///          GitHub:     trfd <https://github.com/trfd>
///          Website:    <http://www.trf-d.com>
///
///  Copyright (C) 2014 Baptiste (trfd) Dupy
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <http://www.gnu.org/licenses/>.
/// 

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
    
    template<int __n>
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
