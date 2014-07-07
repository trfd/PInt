//
//  Log.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#include <string>

#include "Cytok/logs/Log.hpp"

#include "Cytok/logs/Logger.hpp"

#include "Cytok/logs/LoggerStd.hpp"

namespace ck
{
    ILogger* Log::mLogger = nullptr;
    
    ILogger* Log::mFallbackLogger = new LoggerStd();
    
    void Log::setLogger(ILogger* newLogger)
    {
        if(!newLogger && mLogger)
            return;
        
        if(newLogger)
            mLogger = newLogger;
        else
            mLogger = mFallbackLogger;
    }
    
    void Log::info(const std::string& info,
                   Log::ContextInfo ctxInfo)
    {
        if(!mLogger)
            mLogger = mFallbackLogger;
        
        mLogger->message(INFO , info , ctxInfo);
    }
    
    void Log::warning(const std::string& warn,
                      Log::ContextInfo ctxInfo)
    {
        if(!mLogger)
            mLogger = mFallbackLogger;

        mLogger->message(WARNING , warn , ctxInfo);
    }
    
    void Log::error(const std::string& err ,
                    Log::ContextInfo ctxInfo)
    {
        if(!mLogger)
            mLogger = mFallbackLogger;

        mLogger->message(ERROR , err , ctxInfo);
    }
    
    void Log::exception(std::exception& excpt,
                        Log::ContextInfo ctxInfo,
                        const std::string& addMsg)
    {
        if(!mLogger)
            mLogger = mFallbackLogger;

        mLogger->exception(excpt , ctxInfo , addMsg);
    }
    
    void Log::message(Log::Type msgType,
                      const std::string& msg ,
                      Log::ContextInfo ctxInfo)
    {
        if(!mLogger)
            mLogger = mFallbackLogger;
        
        mLogger->message(msgType , msg , ctxInfo);
    }
}