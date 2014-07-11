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
        
        addDefaultProperty(ObjectProperty::INTEGER, "C", base, &Basic::c, &Basic::setC);
    }
};

REGISTER_PROXY(BasicProxy);


class Test : public Object
{
    float _c = 9.f;
    Basic obj;
    
public:
    
    void setC(float v)
    {
        _c = v;
    }
    
    float c(){ return _c; }
    
    float inout(float c)
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

        addProperty(new ObjectProxyProperty("Object" , ObjectProxyFactory::currentFactory()->createProxy(&testObj->object())));

        addDefaultProperty(ObjectProperty::FLOAT, "Parameter C", testObj, &Test::c, &Test::setC);
 
    }
};

int main(int argc, const char * argv[])
{

    ObjectPropertyFactory* propertyFactory = new ObjectPropertyFactory();
    
    ObjectPropertyFactory::setCurrentFactory(propertyFactory);
    
    ObjectProxyFactory* proxyFactory = new ObjectProxyFactory();
    
    ObjectProxyFactory::setCurrentFactory(proxyFactory);
    
    Test obj;
    
    //obj.setC(10);
    
    TestProxy prox = TestProxy(&obj);
    
    prox.buildProxy();
    
    prox.property(0)->property(0)->valueFromString("23");
    
    for(int i= 0 ; i<prox.propertyCount() ; i++)
    {
        std::cout<<prox.property(i)->name()<<"=";
        
        if(prox.property(i)->propertyCount() > 0)
        {
            std::cout<<"{\n";
            
            for(int j=0 ; j<prox.property(i)->propertyCount() ; j++)
            {
               std::cout<<prox.property(i)->property(j)->name()<<"="
                        <<prox.property(i)->property(j)->valueToString()<<"\n";
            }
            std::cout<<"}\n";
        }
        else
            std::cout<<prox.property(i)->valueToString()<<"\n";
    }

}

