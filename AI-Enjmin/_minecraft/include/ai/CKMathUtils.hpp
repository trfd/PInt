///  
///  MathUtils.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 25/07/14.
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

#ifndef CYTOK_MathUtils_hpp
#define CYTOK_MathUtils_hpp

namespace ck
{
    namespace math
    {
        /// Returns (value >= minValue && value <= maxValue)
        template<typename _T>
        bool belongsTo(_T value, _T minValue, _T maxValue)
        {
            return (value >= minValue && value <= maxValue);
        }
        
        /// Returns (value > minValue && value < maxValue)
        template<typename _T>
        bool strictlyBelongsTo(_T value, _T minValue, _T maxValue)
        {
            return (value > minValue && value < maxValue);
        }
        
    }
}

#endif
