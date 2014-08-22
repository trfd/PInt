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
#include <list>
#include <deque>

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

        #pragma region Direction

        /// 8-Direction of cells + zero direction
        /// Can be combined
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

        const uint8_t g_dirXMask = 0x03;
        const uint8_t g_dirYMask = 0x0C;
        
        inline Direction operator|(Direction a, Direction b);
        
        inline Direction operator&(Direction a, Direction b);
        
        ///  return -1,0,1
        inline int dir_x(Direction dir);
        
        ///  return -1,0,1
        inline int dir_y(Direction dir);

        inline ck::Vector2i direction(Direction dir);

        #pragma endregion


        #pragma region UniDirection

        /// UniDirection are direction enum using adjacent numbers.
        /// This way UniDirection can be iterated unlike Direction.
        /// However convertion between both are
        /// provided.
        /// see uniDirection(Direction) and 
        /// direction(UniDirection)
        enum class UniDirection : uint8_t
        {
            UP         = 0,
            DOWN       = 1,
            LEFT       = 2,
            RIGHT      = 3,

            UP_LEFT    = 4,
            UP_RIGHT   = 5,
            DOWN_LEFT  = 6,
            DOWN_RIGHT = 7,

            NONE       = 8
        };

        inline Direction uniToDir(UniDirection udir_);

        inline UniDirection dirToUni(Direction dir_);

        inline ck::Vector2i direction(UniDirection udir_);

        // Preincrement
        UniDirection& operator++(UniDirection& udir_);
        
        // PostIncrement
        UniDirection& operator++(UniDirection& udir_,int);


        #pragma endregion

        enum Border
        {
            TOP     = 0,
            BOTTOM  = 1,
            LEFT    = 2,
            RIGHT   = 3,

            NONE    = 4
        };

        typedef ck::Vector2i Cell;
        typedef ck::Vector2i LocalCell;

        typedef size_t   CellIndex;
        typedef uint16_t LocalCellIndex;

        typedef ck::Size2i CellSize;

        typedef ck::CellRect CellRect;

  
        typedef uint32_t ChunkID;
        
        const ChunkID g_badChunk = UINT32_MAX;

        /// The frontier is the line between two chunks
        typedef uint32_t FrontierID;

        const FrontierID g_badFrontier = UINT32_MAX;

        /// Chunk coordinates are coordinate of 
        /// chunks in grid's chunks-grid frame.
        //Example: In a grid 32x32 with chunks of size 16x16
        // chunk-grid is of size 2x2. Then ChunkCoord are: 
        // for chunk 0 : (0,0)
        // for chunk 1 : (1,0)
        // for chunk 2 : (0,1)
        // for chunk 3 : (1,1)
        typedef ck::Vector2i ChunkCoord;

        /// Type representing cost of grid cell (0-255).
        /// 255 is not a non-walking cell (usually walls).
        /// [1-254] are common cost (0 is reserved).
        typedef uint8_t Cost;
        typedef uint16_t IntegratedCost;

        const Cost g_maxCost = 0xFF;
        const IntegratedCost g_maxIntegratedCost = 0xFFFF;

        typedef std::vector<Cell>   CellArray;
        typedef CellArray::iterator CellArray_it;
        typedef std::list<Cell>     CellList;
        typedef CellList::iterator  CellList_it;
        typedef std::deque<Cell>    CellDeque;
        typedef CellList::iterator  CellDeque_it;

        typedef std::vector<LocalCell>   LocalCellArray;
        typedef LocalCellArray::iterator LocalCellArray_it;
        typedef std::list<LocalCell>     LocalCellList;
        typedef LocalCellList::iterator  LocalCellList_it;
        typedef std::deque<LocalCell>    LocalCellDeque;
        typedef LocalCellDeque::iterator LocalCellDeque_it;

        typedef std::vector<uint16_t>         LocalCellIndexArray;

        struct BufferCell
        {
            IntegratedCost sumCost   : 16;
            bool waveFront     : 1;
            bool pastWaveFront : 1;
            bool lineOfSight   : 1;
            // 5 bits available
        };

        struct FlowCell
        {
            UniDirection direction : 8;
            //bool pathable : 1;
            //bool hasLineOfSight : 1;
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