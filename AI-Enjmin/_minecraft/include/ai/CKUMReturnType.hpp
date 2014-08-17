///  
///  ReturnType.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 31/07/2014.
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

#ifndef CYTOK_ReturnType_hpp
#define CYTOK_ReturnType_hpp

namespace ck
{
	namespace utils
    {
        namespace meta
        {
            
            /// Return _DataType if is scalar
            /// otherwise _DataType& 
            
            template<typename _DataType,bool __copy_ret>
            struct ReturnType_impl;
            
            template<typename _DataType>
            struct ReturnType
            {
                typedef typename ReturnType_impl<_DataType,std::is_scalar<_DataType>::value>::type type;
            };
            
            template<typename _DataType>
            struct ReturnType_impl<_DataType,true>
            {
                typedef _DataType type;
            };
            
            template<typename _DataType>
            struct ReturnType_impl<_DataType,false>
            {
                typedef _DataType& type;
            };
        }
    }
}

#endif
