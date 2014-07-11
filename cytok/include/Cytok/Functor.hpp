//
//  Functor.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 09/07/2014.
//
//

#ifndef CYTOK_Functor_hpp
#define CYTOK_Functor_hpp

#include <utility>

namespace ck
{
	template<typename ReturnType,typename... Args>
    class FunctorImpl
    {
    public:
        
        virtual ReturnType operator()(Args&&... args) = 0;
        virtual ~FunctorImpl(){}
        
        virtual FunctorImpl* copy() = 0;
    };
    
    template<typename ReturnType,typename... Args>
    class FunctorPtrImpl : public FunctorImpl<ReturnType,Args...>
    {
        typedef ReturnType(*FuncPtr)(Args...);
        FuncPtr _funct;
        
    public:
        
        FunctorPtrImpl(const FunctorPtrImpl& f)
        : _funct(f._funct)
        {}
        
        FunctorPtrImpl(FuncPtr funct)
        : _funct(funct)
        {}
        
        inline virtual ReturnType operator()(Args&&...args)
        {
            return (*_funct)(std::forward<Args>(args)...);
        }
    
        virtual FunctorImpl<ReturnType,Args...>* copy()
        {
            return new FunctorPtrImpl<ReturnType,Args...>(_funct);
        }
    };
    
    template<
        typename MemClass ,
        typename ReturnType,
        typename... Args>
    class FunctorMethodPtrImpl : public FunctorImpl<ReturnType,Args...>
    {
        typedef ReturnType (MemClass::*MethodPtr)(Args...);
        
        MemClass* _targetPtr;
        MethodPtr _method;
        
    public:
        
        FunctorMethodPtrImpl(const FunctorMethodPtrImpl& f)
        : _targetPtr(f._targetPtr) , _method(f._method)
        {}

        
        FunctorMethodPtrImpl(MemClass* target , MethodPtr method)
        : _targetPtr(target) ,
          _method(method)
        {}
        
        inline virtual ReturnType operator()(Args&&... args)
        {
            return (_targetPtr->*_method)(std::forward<Args>(args)...);
        }
        
        virtual FunctorImpl<ReturnType,Args...>* copy()
        {
            return new FunctorMethodPtrImpl<MemClass,ReturnType,Args...>(_targetPtr,_method);
        }
    };
    
    template<typename ReturnType,typename... Args>
    class Functor
    {
        FunctorImpl<ReturnType,Args...> *_impl;
        
    public:
        
        Functor(const Functor& f)
        {
            _impl = f._impl->copy();
        }
        
        Functor(ReturnType(*ptr_f)(Args...))
        : _impl(new FunctorPtrImpl<ReturnType,Args...>(ptr_f))
        {}
        
        template<typename MemClass>
        Functor (MemClass* ptr_mem , ReturnType (MemClass::*ptr_f)(Args...))
        : _impl(new FunctorMethodPtrImpl<MemClass,ReturnType,Args...>(ptr_mem,ptr_f))
        {}
        
        ~Functor()
        {
            delete _impl;
            _impl = NULL;
        }
        
        inline ReturnType operator()(Args&&... args)
        {
            return (*_impl)(std::forward<Args>(args)...);
        }
        
    };
    
    template<
        typename BaseClass,
        typename ReturnType,
        typename... Args
    >
    Functor<ReturnType,Args...> makeFunctor(BaseClass* ptr,
                                            ReturnType(BaseClass::*fPtr)(Args...))
    {
        return Functor<ReturnType,Args...>(ptr,fPtr);
    }
    
    template<
        typename ReturnType,
        typename... Args
    >
    Functor<ReturnType,Args...> makeFunctor(ReturnType(*fPtr)(Args...))
    {
        return Functor<ReturnType,Args...>(fPtr);
    }
}

#endif
