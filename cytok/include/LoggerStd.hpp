//
//  LoggerStd.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_LoggerStd_hpp
#define CYTOK_LoggerStd_hpp

#include <iostream>

#include "Logger.hpp"

namespace ck
{
	class LoggerStd : public ILogger
	{
    public:
        
        virtual void message(Log::Type lType,
                             const std::string& msg,
                             Log::ContextInfo ctxInfo = Log::NONE);
        
		/// <summary>
		/// Call from ck::Log when exception is logged.
		/// </summary>
		/// <param name="excpt">Excption.</param>
		/// <param name="ctxInfo">Context info</param>
		virtual void exception(std::exception& excpt,
                               Log::ContextInfo ctxInfo = Log::NONE,
                               const std::string& addMsg = "");
		
	};
}

#endif
