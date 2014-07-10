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
    
    DEFINE_CLASS(Basic);
    
    void setC(int v)
    {
        _c = v;
    }
    
    int c(){ return _c; }
    
    int inout(int c)
    {return ++c;}
};

REGISTER_CLASS(Basic,ck::Object);

class BasicProxy : public ObjectProxy
{
public:
    
    DEFINE_PROXY(Basic, BasicProxy);
    
    BasicProxy() : ObjectProxy(NULL)
    {}
    
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

REGISTER_PROXY(BasicProxy);


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

struct generator
{

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

        addProperty(new ObjectProxyProperty("Object" , ObjectProxyFactory::instance()->createProxy(&testObj->object())));
        
        
        
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
    
//    prox.property(0)->valueFromString("20");

  //  std::cout<<prox.property(0)->valueToString()<<"\n";
    
    prox.property(0)->property(0)->valueFromString("20");
    
    std::cout<<"Basic::c="<<prox.property(0)->property(0)->valueToString()<<"\n";
    
    std::cout<<"Object: "<<std::hex<<Object::type.id()<<"\n";
    
    
}

