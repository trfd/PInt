

#include "ai/ff/FFTypes.hpp"

// Preincrement
ai::ff::UniDirection& ai::ff::operator++(UniDirection& udir_)
{
    return udir_ = (UniDirection)((uint8_t)udir_+1);
}

// PostIncrement
ai::ff::UniDirection& ai::ff::operator++(UniDirection& udir_,int)
{
    UniDirection cResult(udir_);

    operator++(udir_);          

    return cResult;      
}
