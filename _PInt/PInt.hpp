///
/// The MIT License (MIT)
/// 
/// Copyright (c) 2014 Baptiste Dupy.
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <iostream>

template<typename _DeriveType>
class AMutator;

///
/// Base Class implementating a PInt
///
class A
{
public:
    
    /// Private mutation interface
    /// Grants access to AMutator<_Ctx> and whather AMutator<_Ctx> allows
    /// Where we use CRTP with AMutator to allow subclass of AMutator.
    template<typename _Ctx>
    class MutationInterface
    {
        friend AMutator<_Ctx>;
        friend typename AMutator<_Ctx>::Type;
        
        inline static void setter(A* a) { a->setter(); }
    };
    
protected:
    
    /// Actual mutation
    virtual void setter()
    {
        std::cout<<"A Perform Mutation\n";
    }
    
};

///
/// SubClass of PInt holder
///
class B : public A
{
protected:
    
    virtual void setter()
    {
        std::cout<<"B Perform Mutation\n";
    }
    
};

///
/// Base Mutator
/// Implement CRTP to allow subclassing
///
template<typename _DeriveType>
class AMutator
{
protected:
    
    A _a;
    
public:
    
    /// Our PInt will need Type
    /// to allow subclass accessing
    /// the interface
    typedef _DeriveType Type;
    
    virtual void applyMutation()
    {
        std::cout<<"Basic Mutation\n";
        
        /// Access to the PInt
        A::MutationInterface<_DeriveType>::setter(&_a);
    }
};


///
/// Enhanced Mutator
/// Can access to the PInt the same way
/// as its base class does.
///
class EnhAMutator
: public AMutator<EnhAMutator>
{
protected:
    
    B _b;
    
public:
    
    virtual void applyMutation()
    {
        std::cout<<"Enhanced Mutation\n";
        A::MutationInterface<EnhAMutator>::setter(&_a);
        A::MutationInterface<EnhAMutator>::setter(&_b);
    }
};
