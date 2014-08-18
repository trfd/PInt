///  
///  Hash.hpp
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

#ifndef CYTOK_Hash_hpp
#define CYTOK_Hash_hpp

#include <cstdint>

#include "ai/CKNumericTypes.hpp"

namespace ck
{
    namespace utils
    {
        static const uint8_t HashTable[256] = {
            // 256 values 0-255 in any (random) order suffices
            98,  6,   85,  150, 36,  23,  112, 164, 135, 207, 169, 5,   26,  64,  165, 219,     //  1 16
            61,  20,  68,  89,  130, 63,  52,  102, 24,  229, 132, 245, 80,  216, 195, 115,     //  2 32
            90,  168, 156, 203, 177, 120, 2,   190, 188, 7,   100, 185, 174, 243, 162, 10,      //  3
            237, 18,  253, 225, 8,   208, 172, 244, 255, 126, 101, 79,  145, 235, 228, 121,     //  4 64
            123, 251, 67,  250, 161, 0,   107, 97,  241, 111, 181, 82,  249, 33,  69,  55,      //  5
            59,  153, 29,  9,   213, 167, 84,  93,  30,  46,  94,  75,  151, 114, 73,  222,     //  6
            197, 96,  210, 45,  16,  227, 248, 202, 51,  152, 252, 125, 81,  206, 215, 186,     //  7
            39,  158, 178, 187, 131, 136, 1,   49,  50,  17,  141, 91,  47,  129, 60,  99,      //  8 128
            154, 35,  86,  171, 105, 34,  38,  200, 147, 58,  77,  118, 173, 246, 76,  254,     //  9
            133, 232, 196, 144, 198, 124, 53,  4,   108, 74,  223, 234, 134, 230, 157, 139,     // 10
            189, 205, 199, 128, 176, 19,  211, 236, 127, 192, 231, 70,  233, 88,  146, 44,      // 11
            183, 201, 22,  83,  13,  214, 116, 109, 159, 32,  95,  226, 140, 220, 57,  12,      // 12
            221, 31,  209, 182, 143, 92,  149, 184, 148, 62,  113, 65,  37,  27,  106, 166,     // 13
            3,   14,  204, 72,  21,  41,  56,  66,  28,  193, 40,  217, 25,  54,  179, 117,     // 14
            238, 87,  240, 155, 180, 170, 242, 212, 191, 163, 78,  218, 137, 194, 175, 110,     // 15
            43,  119, 224, 71,  122, 142, 42,  160, 104, 48,  247, 103, 15,  11,  138, 239      // 16
        };
        
        // Pearson Hashing
        
        static inline uint8_t hash8_t(const unsigned char* value, size_t size,int8_t offset = 0)
        {
            uint8_t hash = offset;
            
            for(size_t i=0 ; i<size ; i++)
            {
                hash = HashTable[ (hash ^ value[i]) ];
            }
            
            return hash;
        }
        
        
        template<int __n>
        struct hash_impl
        {
            typedef typename uint_t<__n>::type ReturnType;
            
            static ReturnType get(const unsigned char* value,size_t size)
            {
                ReturnType hash = hash8_t(value,size);
                
                for (int i = 0; i < __n/8; i++)
                {
                    hash = hash | ( hash8_t(value, size , HashTable[(value[0] + i) % 256] ) << i*8 );
                }
                
                return hash;
            }
        };
        
    
        template<int __n>
        struct hash
        {
            typedef typename uint_t<__n>::type ReturnType;

            static ReturnType get(const std::string value)
            {
                return hash_impl<__n>::get((const unsigned char*)value.c_str() , value.length());
            }
            
            static ReturnType get(const char* value)
            {
                return hash_impl<__n>::get((const unsigned char*)value , strlen(value));
            }
            
            template<typename _T>
            static ReturnType get(const _T value,int8_t offset = 0)
            {
                size_t size = sizeof(_T);
                
                const uint8_t* val = reinterpret_cast<const uint8_t*>(&value);
                
                return hash_impl<__n>::get(val,size);
            }
        };

    }
}

#endif
