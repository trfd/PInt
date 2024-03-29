///
///  CKAssert.hpp
///
///  Created by Baptiste Dupy on 16/08/2014.
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
///
///

#ifndef __CK_ASSERT_HPP__
#define __CK_ASSERT_HPP__

#if (_DEBUG || DEBUG)
#   ifdef _MSC_VER
#       define ck_assert(expr) \
do { if (!(expr) && (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__,"", #expr))) \
         _CrtDbgBreak(); \
} while (0)
#   endif
#endif

#endif //__CK_ASSERT_HPP__
