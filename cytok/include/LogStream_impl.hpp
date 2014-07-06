//
//  LogStream_impl.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_LogStream_impl_hpp
#define CYTOK_LogStream_impl_hpp

namespace ck
{
    template<typename InputType>
    LogStream& operator << (LogStream& ls , const InputType& input)
    {
        ls.stream<<input;
        
        return ls;
    }
}

#endif
