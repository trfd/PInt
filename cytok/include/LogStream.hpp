//
//  LogStream.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_LogStream_hpp
#define CYTOK_LogStream_hpp

#include <sstream>

#include "Log.hpp"

namespace ck
{
	class LogStream
	{
        class End
        {
        };
        
    public:
        
        static End end;
        
        static LogStream& getDefault(Log::Type logType,
                                     Log::ContextInfo info = Log::NONE);
        
        LogStream();
        
        LogStream(Log::Type logType,
                  Log::ContextInfo info = Log::NONE);
    
        
    private:
        
        template<typename InputType>
        friend LogStream& operator << (LogStream& ls ,
                                       const InputType& input);
        
        friend void operator << (LogStream& ls , End end);
        
        std::ostringstream stream;
        
        Log::Type mType = Log::INFO;
        Log::ContextInfo mInfo = Log::NONE;
    
        static LogStream* sDefaultStream;
	};
    
}

#include "LogStream_impl.hpp"

#endif
