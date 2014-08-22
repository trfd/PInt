
#include "ai/ff/FFTypes.hpp"

namespace ai
{
    namespace ff
    {
                
        inline Direction operator|(Direction a, Direction b)
        {return static_cast<Direction>(static_cast<int>(a) | static_cast<int>(b));}
        
        inline Direction operator&(Direction a, Direction b)
        {return static_cast<Direction>(static_cast<int>(a) & static_cast<int>(b));}
        
        ///  return -1,0,1
        inline int dir_x(Direction dir)
        {
            int dx = static_cast<int>(dir) & g_dirXMask;
            
            if(dx == 0) return 0;
            
            return dx - 2;
        }
        
        ///  return -1,0,1
        inline int dir_y(Direction dir)
        {
            int dy =( static_cast<int>(dir) & g_dirYMask ) >> 2;
            
            if(dy == 0) return 0;
            
            return dy - 2;
        }

        inline ck::Vector2i direction(Direction dir)
        {
            return ck::Vector2i(dir_x(dir), dir_y(dir));
        }
        
        inline Direction uniToDir(UniDirection udir_)
        {
            switch(udir_)
            {
            case UniDirection::UP   : return Direction::UP;
            case UniDirection::DOWN : return Direction::DOWN;
            case UniDirection::LEFT : return Direction::LEFT;
            case UniDirection::RIGHT: return Direction::RIGHT;

            case UniDirection::UP_LEFT   : return (Direction::UP   | Direction::LEFT );
            case UniDirection::UP_RIGHT  : return (Direction::UP   | Direction::RIGHT);
            case UniDirection::DOWN_LEFT : return (Direction::DOWN | Direction::LEFT );
            case UniDirection::DOWN_RIGHT: return (Direction::DOWN | Direction::RIGHT);

            case UniDirection::NONE : return Direction::NONE;
            default: return Direction::NONE;
            }
        }

        inline UniDirection dirToUni(Direction dir_)
        {
            int x = dir_x(dir_);
            int y = dir_y(dir_);

                 if(x == -1 || y == -1) return UniDirection::UP_LEFT;
            else if(x ==  1 || y == -1) return UniDirection::UP_RIGHT;
            else if(x == -1 || y ==  1) return UniDirection::DOWN_LEFT;
            else if(x ==  1 || y ==  1) return UniDirection::DOWN_RIGHT;
            else if(x ==  0 || y == -1) return UniDirection::UP;
            else if(x ==  0 || y ==  1) return UniDirection::DOWN;
            else if(x == -1 || y ==  0) return UniDirection::LEFT;
            else if(x ==  1 || y ==  0) return UniDirection::RIGHT;
            else return UniDirection::NONE;   
        }


        inline ck::Vector2i direction(UniDirection udir_)
        {
            Direction dir = uniToDir(udir_);
            return ck::Vector2i(dir_x(dir), dir_y(dir));
        }

        // Preincrement
        UniDirection& operator++(UniDirection& udir_)
        {
            return udir_ = (UniDirection)((uint8_t)udir_+1);
        }
        
        // PostIncrement
        UniDirection& operator++(UniDirection& udir_,int)
        {
            UniDirection cResult(udir_);

            operator++(udir_);          

            return cResult;      
        }

    }
}