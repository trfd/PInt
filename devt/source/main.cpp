//
//  main.cpp
//  GBJ
//
//  Created by Baptiste Dupy on 24/10/13.
//  Copyright (c) 2013 Baptiste Dupy. All rights reserved.
//

#include <iostream>
#include <vector>

#include <Cytok.hpp>

using namespace ck;
using namespace proxy;

class Test : public Object
{
    int _c;
    
public:
    
    void setC(int v)
    {
        _c = v;
    }
    
    int c(){ return _c; }
    
    int inout(int c)
    {return ++c;}
};

class TestProxy : public ObjectProxy
{
public:
    
    TestProxy(Test* test)
    : ObjectProxy(test)
    {}
    
    virtual void buildPropertyList()
    {
        Test* testObj =(Test*)this->object();
        
        addProperty(new ObjectPropertyGeneric<int>(
                    "ParameterC",
                    makeFunctor(testObj, &Test::c),
                    Functor<void, int>(testObj, &Test::setC))
                    );
    }
};

int main(int argc, const char * argv[])
{
    Test obj;
    
    obj.setC(10);
    
    Functor<void, int>* f = new Functor<void, int>(&obj, &Test::setC);
    
    Functor<int, int>* f2 = new Functor<int, int>(&obj, &Test::inout);
    
    TestProxy prox = TestProxy(&obj);
    
    prox.buildPropertyList();
    
    prox.property(0)->valueFromString("20");

    std::cout<<(*f2)(42)<<"\n";
    
    std::cout<<prox.property(0)->valueToString()<<"\n";
    
}

