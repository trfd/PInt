///
///  FFChunk.hpp
///
///  Created by Baptiste Dupy on 13/08/2014.
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

#ifndef AI_FF_Chunk_hpp
#define AI_FF_Chunk_hpp

#include "ai/CKUMReturnType.hpp"

using namespace ck::utils::meta;

namespace ai
{
    namespace ff
    {
        /// Classic 2D atomic grid
        template<typename _Data,int __width,int __height>
        class Chunk
        {
        public:

            typedef _Data Data;
            typedef typename ReturnType<_Data>::type ReturnType;

            static const int width  = __width;
            static const int height = __height;
            static const int size   = __width*__height; 

            Chunk()
            {
                clear();
            }

            virtual ~Chunk(){}

            inline bool isInChunk(int x_, int y_)
            {
                return (x_>=0 && y_>=0 && x_ < __width && y_ < __height);
            }

            inline bool isInChunk(int idx_)
            {
                return (idx_ >= 0 && idx_ < size);
            }

            inline void set(_Data&& d_, int x_, int y_)
            {
                if(!isInChunk(x_,y_))
                    return;

                m_grid[x_ + __width * y_] = std::move(d_);
            }

            inline void set(_Data const& d_, int x_, int y_)
            {
                if(!isInChunk(x_, y_))
                    return;

                m_grid[x_ + __width * y_] = d_;
            }

            inline ReturnType get(int x_, int y_)
            {
                if(!isInChunk(x_, y_))
                    return ReturnType();

               return m_grid[x_ + __width * y_];
            }

            inline void clear()
            {
                std::memset(m_grid, 0, sizeof(m_grid));
            }
           
        private:
             _Data m_grid[size];
        };
    }
}

#endif //AI_FF_Chunk_hpp