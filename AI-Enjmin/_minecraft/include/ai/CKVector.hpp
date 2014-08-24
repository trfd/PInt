///  
///  Vector.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 24/07/14.
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

#ifndef CYTOK_Vector_hpp
#define CYTOK_Vector_hpp

namespace ck
{
    
    template<typename _T>
    class Size2;
    
    template<typename _T>
    class Vector2
    {
    public:
        
        union
        {
            struct
            {
                _T x,y;
            };
            
            _T components[2];
        };
        
        static const Vector2 zero;

        inline static float distance(const Vector2& v1, const Vector2& v2)
        {
            float x = v1.x-v2.x;
            float y = v1.y-v2.y;

            return sqrtf(x*x+y*y);
        }

        inline static _T distance2(const Vector2& v1, const Vector2& v2)
        {
             _T x = v1.x-v2.x;
             _T y = v1.y-v2.y;

            return (x*x+y*y);
        }

        Vector2()
        : x(0) , y(0)
        {}
        
        Vector2(_T x_ , _T y_)
        : x(x_) , y(y_)
        {}
        
        bool operator== (const Vector2 v2)
        {
            return (x == v2.x && y == v2.y);
        }
        
        bool operator!= (const Vector2 v2)
        {
            return (x != v2.x || y != v2.y);
        }
        
        /// Binary operation Vector2
        
        
        Vector2 operator+ (const Vector2& a) const
        {
            return Vector2(x+a.x,y+a.y);
        }
        
        Vector2 operator- (const Vector2& a) const
        {
            return Vector2(x-a.x,y-a.y);
        }
        
        Vector2 operator* (const Vector2& a) const
        {
            return Vector2(x*a.x,y*a.y);
        }

        Vector2& operator+= (const Vector2& a)
        {
            x += a.x;
            y += a.y;
            
            return *this;
        }
        
        Vector2& operator-= (const Vector2& a)
        {
            x -= a.x;
            y -= a.y;
            
            return *this;
        }

         Vector2& operator*= (const Vector2& a)
        {
            x *= a.x;
            y *= a.y;
            
            return *this;
        }
        
        // Operator Size2
        
        Vector2 operator+ (const Size2<_T>& a) const
        {
            return Vector2(x+a.width,y+a.height);
        }
        
        Vector2 operator- (const Size2<_T>& a) const
        {
            return Vector2(x-a.width,y-a.height);
        }
        Vector2 operator* (const Size2<_T>& a) const
        {
            return Vector2(x*a.width,y*a.height);
        }
        Vector2 operator* (float sc) const
        {
            return Vector2(x*sc,y*sc);
        }
        Vector2& operator+= (const Size2<_T>& a)
        {
            x += a.width;
            y += a.height;
            
            return *this;
        }
        
        Vector2& operator-= (const Size2<_T>& a)
        {
            x -= a.width;
            y -= a.height;
            
            return *this;
        }

        Vector2& operator*= (const Size2<_T>& a)
        {
            x *= a.width;
            y *= a.height;
            
            return *this;
        }

        // Conversion

        operator Size2<_T>()
        {
            return Size2<_T>(x,y);
        }
    };
    
    typedef Vector2<int> Vector2i;
    typedef Vector2<char> Vector2b;
    typedef Vector2<float> Vector2f;
    typedef Vector2<double> Vector2d;
    typedef Vector2<unsigned int> Vector2ui;
    typedef Vector2<unsigned char> Vector2ub;
    
    
    template<typename _T>
    class Size2
    {
    public:
        
        union
        {
            struct
            {
                _T width,height;
            };
            
            _T components[2];
        };
        
        static const Size2 zero;
        
        Size2()
        : width(0), height(0)
        {}
        
        Size2(_T width_ , _T height_)
        : width(width_), height(height_)
        {}
        
        bool operator== (const Size2 v2)
        {
            return (width == v2.width && height == v2.height);
        }
        
        bool operator!= (const Size2 v2)
        {
            return (width != v2.width || height != v2.height);
        }

         /// Binary operation with Vector2
        
        Size2 operator+ (const Size2& a)
        {
            return Size2(width+a.width,height+a.height);
        }
        
        Size2 operator- (const Size2& a)
        {
            return Vector2(width-a.width,height-a.height);
        }
        
        Size2& operator+= (const Size2& a)
        {
            width += a.width;
            height += a.height;
            
            return *this;
        }
        
        Size2& operator-= (const Size2& a)
        {
            width -= a.width;
            height -= a.yheight;
            
            return *this;
        }

        // Conversion

        operator Vector2<_T>()
        {
            return Vector2<_T>(width,height);
        }
    };
    
    typedef Size2<int> Size2i;
    typedef Size2<char> Size2b;
    typedef Size2<float> Size2f;
    typedef Size2<double> Size2d;
    typedef Size2<unsigned int> Size2ui;
    typedef Size2<unsigned char> Size2ub;
    
}

#include "CKVector_impl.hpp"

#endif
