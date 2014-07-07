//
//  Logger.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_Logger_hpp
#define CYTOK_Logger_hpp

#include "Cytok/logs/Log.hpp"

namespace ck
{
	/// <summary>
	/// Logger interface. Logger is the subsystem
	/// that actually write something somewhere when a CK.Log method is called.
	/// The most basic Logger is the one that writes into the standard output stream.
	/// Enhanced debugger can be create such as HTML or ingame console logs.
	/// To change the logger simply set the value of CK.Log.logger to an
	/// instance of your custom logger implementation.
	/// </summary>
	class ILogger
	{
    public:
        
		/// <summary>
		/// Generic call from ck::log for every type of logging event,
		/// except for exception (See ILogger.Exception())
		/// </summary>
		/// <param name="lType">Type of the logging event</param>
		/// <param name="msg">Message to log</param>
		/// <param name="ctxInfo">Context info</param>
		virtual void message(Log::Type lType,
                             const std::string& msg,
                             Log::ContextInfo ctxInfo = Log::NONE) = 0;
        
		/// <summary>
		/// Call from ck::Log when exception is logged.
		/// </summary>
		/// <param name="excpt">Excption.</param>
		/// <param name="ctxInfo">Context info</param>
		virtual void exception(std::exception& excpt,
                               Log::ContextInfo ctxInfo = Log::NONE,
                               const std::string& addMsg = "" ) = 0;
        
	};

}

#endif
