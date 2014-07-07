//
//  LoggerStd.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#include "Cytok/logs/LoggerStd.hpp"

namespace ck
{
    void LoggerStd::message(Log::Type lType,
                            const std::string& msg,
                            Log::ContextInfo ctxInfo)
    {
        std::string strCtx = "";
        
        switch (lType) {
            case Log::INFO:
                strCtx = "INFO";
                break;
            case Log::WARNING:
                strCtx = "WARNING";
                break;
            case Log::ERROR:
                strCtx = "ERROR";
                break;
            default:
                break;
        }
        
        std::cout<<"["<<strCtx<<"] "<<msg<<std::endl;
    }
    
    void LoggerStd::exception(std::exception& excpt,
                              Log::ContextInfo ctxInfo,
                              const std::string& addMsg)
    {
        std::cout<<"[EXCEPT.] "<<addMsg<<" {"<<excpt.what()<<"}"<<std::endl;
    }
}