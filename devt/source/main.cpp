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

class Basic : public Object
{
    int _c = 4;
    
public:
    
    void setC(int v)
    {
        _c = v;
    }
    
    int c(){ return _c; }
    
    int inout(int c)
    {return ++c;}
};

class BasicProxy : public ObjectProxy
{
public:
    BasicProxy(Basic* basic)
    : ObjectProxy(basic)
    {}
    
    virtual void buildProxy()
    {
        Basic* base = (Basic*) this->object();
        
        this->setName("Basic");
        
        addProperty(new ObjectPropertyGeneric<int>("C",
                                                   makeFunctor(base,&Basic::c),
                                                   makeFunctor(base,&Basic::setC)));
    }
};

class Test : public Object
{
    int _c;
    Basic obj;
    
public:
    
    void setC(int v)
    {
        _c = v;
    }
    
    int c(){ return _c; }
    
    int inout(int c)
    {return ++c;}
    
    Basic& object()
    {
        return obj;
    }
};

class TestProxy : public ObjectProxy
{
public:
    
    TestProxy(Test* test)
    : ObjectProxy(test)
    {}
    
    virtual void buildProxy()
    {
        Test* testObj =(Test*)this->object();
        
        BasicProxy* prox = new BasicProxy(&testObj->object());
        
        prox->buildProxy();
        
        addProxy(prox);
        
        addProperty(new ObjectPropertyGeneric<int>(
                    "ParameterC",
                    makeFunctor(testObj, &Test::c),
                    makeFunctor(testObj, &Test::setC))
                    );
    }
};

int main(int argc, const char * argv[])
{
    Test obj;
    
    obj.setC(10);
    
    TestProxy prox = TestProxy(&obj);
    
    prox.buildProxy();
    
    prox.property(0)->valueFromString("20");

    std::cout<<prox.property(0)->valueToString()<<"\n";
    
    std::cout<<"Basic::c="<<prox.proxy(0)->property(0)->valueToString()<<"\n";
    
}

