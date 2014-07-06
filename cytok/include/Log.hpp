//
//  Log.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_Log_hpp
#define CYTOK_Log_hpp

//#include "Logger.hpp"

namespace ck
{
    class ILogger;
    
	/// <summary>
	/// Log system for the CKEngine.
	/// </summary>
    class Log
	{
        
    public:
        
        /// <summary>
        /// Type of log, indicates gravity of report.
        /// </summary>
        enum Type
        {
            INFO,
            WARNING,
            ERROR,
            ASSERT,
            EXCEPTION,
            STACK
        };
        
        /// <summary>
        /// Context information for logs.
        /// Allow to dispatch or select/ignore reports
        /// for loggers.
        /// </summary>
        enum ContextInfo
        {
            NONE,
            CORE,
            ENGINE,
            PLATFORM,
            DEVICE,
            GRAPHICS,
            PHYSICS,
            TERRAIN,
            ASSET,
            NETWORK,
            AUDIO,
            GAMEPLAY,
            INPUT,
            AI,
            UI,
            LAUNCHER,
            OTHER
        };

        /// <summary>
        /// Set the active logger. Default uses the LoggerStd
        /// </summary>
        /// <param name="newLogger">Custom Logger instance</param>
        static void setLogger(ILogger* newLogger);

		/// <summary>
		/// Log an info message
		/// </summary>
		/// <param name="info">Info.</param>
		static void info(const std::string& info,
                         ContextInfo ctxInfo = NONE);
        
		/// <summary>
		/// Log a warning
		/// </summary>
		/// <param name="warn">Warn.</param>
		static void warning(const std::string& warn,
                            ContextInfo ctxInfo = NONE);
        
		/// <summary>
		/// Log an Error
		/// </summary>
		/// <param name="err">Error.</param>
		static void error(const std::string& err ,
                          ContextInfo ctxInfo = NONE);
        
		/// <summary>
		/// Log an exception
		/// </summary>
		/// <param name="excpt">Excpt.</param>
		static void exception(std::exception& excpt,
                              ContextInfo ctxInfo = NONE,
                              const std::string& addMsg = "");
        
        /// <summary>
		/// Simply wraps the call to ILogger::message()
		/// </summary>
		/// <param name="excpt">Excpt.</param>
		static void message(Type msgType,
                            const std::string& msg ,
                            ContextInfo ctxInfo = NONE);
        
    private:
        
        /// <summary>
		/// Logger implementation to use.
		/// </summary>
		static ILogger* mLogger;
        
        static ILogger* mFallbackLogger;
        
	};
}

#endif
