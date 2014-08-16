///
///  FFTypes.hpp
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

#ifndef AI_FF_Types_hpp
#define AI_FF_Types_hpp

#include <cstdint>
#include <vector>

#include "ai/CKVector.hpp"
#include "ai/CKCellRect.hpp"

namespace ai
{
    namespace ff
    {
        /// Default implementation of Cost/Integration type
        /// are provided here for ease of use.
        /// However this types implies restriction
        /// on maximun chunk size. They can should 
        /// not be larger than 16x16 (or 256 cells).

        /// [This restriction is set by the sizes of
        /// the type Cost and Integration.sumCost.
        /// Since they're resp. coded on 8 and 16 bits
        /// and since sumCost is a sum of Cost they
        /// can not be more than 2^8 Costs summed.
        /// (This is a very large approx, a path 
        /// can almost never go through all chunk's
        /// cells, a fortiri all cells of maximum cost)]

        /// 8-Direction of cells + zero direction
        /// First and third bits indicates
        /// whether or not the direction has 
        /// resp. a vertical and horizontal component.
        /// The second and fourth bits store the value
        /// of the component.
        /// The value is ignored if the respective
        /// first or third bit is 0
        enum class Direction : uint8_t
        {
            NONE    = 0x0,
            
            X       = 0x1,
            
            LEFT    = 0x1,
            RIGHT   = 0x3,

            Y       = 0x4,
            
            UP      = 0x4,
            DOWN    = 0xC
        
        };
        
        inline Direction operator|(Direction a, Direction b)
        {return static_cast<Direction>(static_cast<int>(a) | static_cast<int>(b));}
        
        inline Direction operator&(Direction a, Direction b)
        {return static_cast<Direction>(static_cast<int>(a) & static_cast<int>(b));}
        
        ///  return -1,0,1
        inline int dir_x(Direction dir)
        {
            int dx = static_cast<int>(dir) & 0x03;
            
            if(dx == 0) return 0;
            
            return dx - 2;
        }
        
        ///  return -1,0,1
        inline int dir_y(Direction dir)
        {
            int dy =( static_cast<int>(dir) & 0x0C ) >> 2;
            
            if(dy == 0) return 0;
            
            return dy - 2;
        }

        inline ck::Vector2i direction(Direction dir)
        {
            return ck::Vector2i(dir_x(dir), dir_y(dir));
        }

        enum Border
        {
            TOP     = 0,
            BOTTOM  = 1,
            LEFT    = 2,
            RIGHT   = 3,

            NONE    = 4
        };

        

        typedef ck::Vector2i Cell;

        typedef ck::Size2i CellSize;

        typedef ck::CellRect CellRect;

  
        typedef uint32_t ChunkID;

        /// The frontier is the line between two chunks
        typedef uint32_t FrontierID;

        const FrontierID g_badFrontier = UINT32_MAX;

        typedef ck::Vector2i ChunkCoord;

        /// Type representing cost of grid cell (0-255).
        /// 255 is not a non-walking cell (usually walls).
        /// [1-254] are common cost (0 is reserved).
        typedef uint8_t Cost;

        const Cost g_maxCost = 0xFF;

        struct BufferCell
        {
            uint16_t sumCost : 16;
            bool waveFront : 1;
            bool lineOfSight : 1;
            // 6 bits available
        };

        struct FlowCell
        {
            Direction direction : 8;
            bool pathable : 1;
            bool hasLineOfSight : 1;
            //  6 bits available
        };

        /*
        struct Frontier
        {
            FrontierID id;
            ChunkID chunk1;
            ChunkID chunk2;
        };
        */

    }
}

#endif //AI_FF_Types_hpp