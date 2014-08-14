///  
///  CKFunctor.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 09/07/2014.
///  Contact:
///          Mail:       <baptiste.dupy@gmail.com>
///          GitHub:     trfd <https://github.com/trfd>
///          Website:    <http://www.trf-d.com>
///
///  Copyright (C) 2014 Baptiste (trfd) Dupy
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <http://www.gnu.org/licenses/>.
/// 

#ifndef CYTOK_Functor_hpp
#define CYTOK_Functor_hpp

#include <utility>

//TODO: Refactor to allow usage of smart pointer
// in the same way as regular pointer are used

namespace ck
{
	template
    <
        typename _ReturnType,
        typename... _Args
    >
    class FunctorImpl
    {
    public:
        
        virtual _ReturnType operator()(_Args&&... args) = 0;
        virtual ~FunctorImpl(){}
        
        virtual FunctorImpl* copy() = 0;
    };
    
    template
    <
        typename _ReturnType,
        typename... _Args
    >
    class FunctorPtrImpl : public FunctorImpl<_ReturnType,_Args...>
    {
        typedef _ReturnType(*FuncPtr)(_Args...);
        FuncPtr _funct;
        
    public:
        
        FunctorPtrImpl(const FunctorPtrImpl& f)
        : _funct(f._funct)
        {
        }
        
        FunctorPtrImpl(FuncPtr funct)
        : _funct(funct)
        {}
        
        inline virtual _ReturnType operator()(_Args&&...args)
        {
            return (*_funct)(std::forward<_Args>(args)...);
        }
    
        virtual FunctorImpl<_ReturnType,_Args...>* copy()
        {
            return new FunctorPtrImpl<_ReturnType,_Args...>(_funct);
        }
    };
    
    template
    <
        typename _MemClass ,
        typename _ReturnType,
        typename... _Args
    >
    class FunctorMethodPtrImpl : public FunctorImpl<_ReturnType,_Args...>
    {
        typedef _ReturnType (_MemClass::*MethodPtr)(_Args...);
        
        _MemClass* _targetPtr;
        MethodPtr _method;
        
    public:
        
        FunctorMethodPtrImpl(const FunctorMethodPtrImpl& f)
        : _targetPtr(f._targetPtr) , _method(f._method)
        {}

        
        FunctorMethodPtrImpl(_MemClass* target , MethodPtr method)
        : _targetPtr(target) ,
          _method(method)
        {}
        
        inline virtual _ReturnType operator()(_Args&&... args)
        {
            return (_targetPtr->*_method)(std::forward<_Args>(args)...);
        }
        
        virtual FunctorImpl<_ReturnType,_Args...>* copy()
        {
            return new FunctorMethodPtrImpl<_MemClass,_ReturnType,_Args...>(_targetPtr,_method);
        }
    };
    
    template
    <
        typename _ReturnType,
        typename... _Args
    >
    class Functor
    {
        FunctorImpl<_ReturnType,_Args...> *_impl;
        
    public:
        
        Functor(const Functor& f)
        {
            _impl = f._impl->copy();
        }
        
        Functor(_ReturnType(*ptr_f)(_Args...))
        : _impl(new FunctorPtrImpl<_ReturnType,_Args...>(ptr_f))
        {}
        
        template<typename _MemClass>
        Functor (_MemClass* ptr_mem , _ReturnType (_MemClass::*ptr_f)(_Args...))
        : _impl(new FunctorMethodPtrImpl<_MemClass,_ReturnType,_Args...>(ptr_mem,ptr_f))
        {}
        
        ~Functor()
        {
            delete _impl;
            _impl = NULL;
        }
        
        inline _ReturnType operator()(_Args&&... args)
        {
            return (*_impl)(std::forward<_Args>(args)...);
        }
        
    };
    
    template
    <
        typename _BaseClass,
        typename _ReturnType,
        typename... _Args
    >
    Functor<_ReturnType,_Args...> makeFunctor(_BaseClass* ptr,
                                        _ReturnType(_BaseClass::*fPtr)(_Args...))
    {
        return Functor<_ReturnType,_Args...>(ptr,fPtr);
    }
    
    template
    <
        typename _ReturnType,
        typename... _Args
    >
    Functor<_ReturnType,_Args...> makeFunctor(_ReturnType(*fPtr)(_Args...))
    {
        return Functor<_ReturnType,_Args...>(fPtr);
    }
    
    
    template
    <
        typename _BaseClass,
        typename _ReturnType,
        typename... _Args
    >
    Functor<_ReturnType,_Args...>* makeNewFunctor(_BaseClass* ptr,
											_ReturnType(_BaseClass::*fPtr)(_Args...))
    {
        return new Functor<_ReturnType,_Args...>(ptr,fPtr);
    }
    
    template
    <
        typename _ReturnType,
        typename... _Args
    >
    Functor<_ReturnType,_Args...>* makeNewFunctor(_ReturnType(*fPtr)(_Args...))
    {
        return new Functor<_ReturnType,_Args...>(fPtr);
    }
}

#endif
