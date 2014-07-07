//
//  LogMacros.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_LogMacros_hpp
#define CYTOK_LogMacros_hpp

#define __CK_LOG_CODE_INFO__ "("<<__func__<<":"<<__LINE__<<") "

/// Define convenient macro for logs
/// Note that if LOG_type is already defined
/// it won't be overridden
/// CK_LOG_type will always be defined.


/// Info

#define CK_LOG_INFO \
ck::LogStream::getDefault(ck::Log::INFO)<<__CK_LOG_CODE_INFO__

#define CK_LOG_INFO_I(_ctx_) \
ck::LogStream::getDefault(ck::Log::INFO,_ctx_)<<__CK_LOG_CODE_INFO__

#ifndef LOG_INFO

#define LOG_INFO \
ck::LogStream::getDefault(ck::Log::INFO)<<__CK_LOG_CODE_INFO__

#define LOG_INFO_I(_ctx_) \
ck::LogStream::getDefault(ck::Log::INFO,_ctx_)<<__CK_LOG_CODE_INFO__

#endif

/// Warning

#define CK_LOG_WARNING \
ck::LogStream::getDefault(ck::Log::WARNING)<<__CK_LOG_CODE_INFO__

#define CK_LOG_WARNING_I(_ctx_) \
ck::LogStream::getDefault(ck::Log::WARNING,_ctx_)<<__CK_LOG_CODE_INFO__

#ifndef LOG_WARNING

#define LOG_WARNING \
ck::LogStream::getDefault(ck::Log::WARNING)<<__CK_LOG_CODE_INFO__

#define LOG_WARNING_I(_ctx_) \
ck::LogStream::getDefault(ck::Log::WARNING,_ctx_)<<__CK_LOG_CODE_INFO__

#endif

/// Error

#define CK_LOG_ERROR \
ck::LogStream::getDefault(ck::Log::ERROR)<<__CK_LOG_CODE_INFO__

#define CK_LOG_ERROR_I(_ctx_) \
ck::LogStream::getDefault(ck::Log::ERROR,_ctx_)<<__CK_LOG_CODE_INFO__

#ifndef LOG_ERROR

#define LOG_ERROR \
ck::LogStream::getDefault(ck::Log::ERROR)<<__CK_LOG_CODE_INFO__

#define LOG_ERROR_I(_ctx_) \
ck::LogStream::getDefault(ck::Log::ERROR,_ctx_)<<__CK_LOG_CODE_INFO__

#endif

/// Exceptions

#define CK_LOG_EXCEPT \
ck::LogStream::getDefault(ck::Log::EXCEPT)<<__CK_LOG_CODE_INFO__

#define CK_LOG_EXCEPT_I(_ctx_) \
ck::LogStream::getDefault(ck::Log::EXCEPT)<<__CK_LOG_CODE_INFO__

#ifndef LOG_EXCEPT

#define LOG_EXCEPT \
ck::LogStream::getDefault(ck::Log::EXCEPTION)<<__CK_LOG_CODE_INFO__

#define LOG_EXCEPT_I(_ctx_) \
ck::LogStream::getDefault(ck::Log::EXCEPTION,_ctx_)<<__CK_LOG_CODE_INFO__

#endif

/// End

#define CK_LOG_END \
ck::LogStream::end

#define CK_END_LOG \
ck::LogStream::end

#ifndef END_LOG
#define END_LOG \
ck::LogStream::end
#endif

#ifndef LOG_END
#define LOG_END \
ck::LogStream::end
#endif

#ifndef END_LOG
#define END_LOG \
ck::LogStream::end
#endif

#endif
