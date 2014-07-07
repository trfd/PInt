//
//  LogStream.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//
#include <iomanip>

#include "Cytok/logs/LogStream.hpp"

namespace ck
{
    LogStream* LogStream::sDefaultStream = new LogStream();
    
    LogStream::LogStream() : mType(Log::INFO) , mInfo(Log::NONE)
    {}
    
    LogStream::LogStream(Log::Type logType,
                         Log::ContextInfo info)
    : mType(logType) , mInfo(info)
    {}
    
    void operator << (LogStream& ls , LogStream::End end)
    {
        Log::message(ls.mType, ls.mStream.str() , ls.mInfo);
        
        // Clear content of stream
        ls.clearStream(ls.mStream);
        
        ls.mType = Log::INFO;
        ls.mInfo = Log::NONE;
    }
    
    LogStream& LogStream::getDefault(Log::Type logType,
                                     Log::ContextInfo info)
    {
        /// Check if the default stream contains information not sent
        /// Typically user forgot to append message with end.
        if( sDefaultStream->mStream.str() != "" )
        {
            /// Send message
            Log::message(sDefaultStream->mType, sDefaultStream->mStream.str() , sDefaultStream->mInfo);
            
            /// Add warning to inform of delay and to recall using end at the end
            Log::warning(
                         "The previous message has been delayed: add '<<ck::LogStream::end' or \
                         'CLOG_END' at the end of the log stream",Log::ENGINE);
            
            // Clear content
            sDefaultStream->clearStream(sDefaultStream->mStream);
        }
        
        sDefaultStream->mType = logType;
        sDefaultStream->mInfo = info;
        
        return *sDefaultStream;
    }
    
    void LogStream::clearStream(std::ostringstream& stream)
    {
        stream.clear();
        stream<<std::resetiosflags(std::ios::floatfield);
        stream<<std::resetiosflags(std::ios::basefield);
        stream<<std::resetiosflags(std::ios::adjustfield);
        stream.str(std::string());
    }
}