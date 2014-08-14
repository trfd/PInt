///  
///  CellRect.hpp
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

#ifndef CYTOK_CellRect_hpp
#define CYTOK_CellRect_hpp


#include <algorithm>
#include <cassert>
#include <vector>

#include "CKVector.hpp"
#include "CKMathUtils.hpp"

using namespace std;

namespace ck
{
    /// Axis-Aligned top-left rectangles
    class CellRect
    {
    public:
        
        Vector2<int> origin;
        Size2<int> size;
        
        CellRect()
        : origin(Vector2<int>::zero) ,
        size(Size2<int>::zero)
        {}
        
        CellRect(int x, int y , int width , int height)
        : origin(x,y) ,
          size(width , height)
        {}
        
        CellRect( Vector2<int> const& origin_,  Size2<int> const& size_)
        : origin(origin_),
          size(size_)
        {}
        
        int left() const { return origin.x; }
        int right() const { return origin.x + size.width - 1; }
        int top() const { return origin.y; }
        int bottom() const { return origin.y + size.height - 1; }
        
        //Vector2<int> center() { return origin + size >> 2; }
        
        string toString()
        {
            ostringstream str;
            
            str<<"{"<<origin.x<<","<<origin.y<<","<<size.width<<","<<size.height<<"}";
            
            return str.str();
        }
        
        bool operator== ( const CellRect& r2)
        {
            return (origin == r2.origin && size == r2.size);
        }
        
        bool operator!= (const CellRect& r2)
        {
            return (origin != r2.origin || size != r2.size);
        }
        
        vector<CellRect> operator- (const CellRect& a) const
        {
            vector<CellRect> ret;
            
            int xstart = left();
            
            if( math::belongsTo(a.left(), left() , right()) && a.left() != left())
            {
                ret.emplace_back(xstart, origin.y, a.left() - xstart , size.height);
                
                xstart = a.left();
            }
            
            if(math::belongsTo(a.top() , top() , bottom()) && a.top() != top())
            {
                ret.emplace_back(xstart , origin.y , min(a.right(), right()) + 1 - xstart , a.top() - origin.y);
            }
            
            if(math::belongsTo(a.bottom() , top() , bottom()) && a.bottom() != bottom())
            {
                ret.emplace_back(xstart , a.bottom()+1 , min(a.right(), right()) + 1 - xstart , size.height  - a.bottom() - 1 + origin.y);
            }
            
            xstart = min(a.right() , right()) + 1;
            
            if(math::belongsTo(a.right() , left() , right()) && a.right() != right())
            {
                ret.emplace_back(xstart, origin.y, size.width - xstart + origin.x , size.height);
            }
            
            return ret;
        }
        
        
        /// Static
        
        static bool intersects(const CellRect& r1 , const CellRect& r2)
        {
            if(!math::belongsTo(r1.left() , r2.left() , r2.right()) &&
               !math::belongsTo(r1.right() , r2.left() , r2.right())&&
               !math::belongsTo(r2.left() , r1.left() , r1.right()) &&
               !math::belongsTo(r2.right() , r1.left() , r1.right()))
                return false;
            
            if(!math::belongsTo(r1.top() , r2.top(), r2.bottom()) &&
               !math::belongsTo(r1.bottom() , r2.top(), r2.bottom())&&
               !math::belongsTo(r2.top() , r1.top(), r1.bottom()) &&
               !math::belongsTo(r2.bottom() , r1.top(), r1.bottom()))
                return false;
            
            return true;
        }
        
        static CellRect intersection(const CellRect& r1 , const CellRect& r2)
        {
            assert(intersects(r1,r2));
            
            int xleft = max(r1.left() , r2.left());
            int xright = min(r1.right() , r2.right());
            int ytop = max(r1.top() , r2.top());
            int ybottom = min(r1.bottom() , r2.bottom());
            
            return CellRect(xleft,ytop,xright-xleft+1,ybottom-ytop+1);
        }
        
    };
}

#endif
