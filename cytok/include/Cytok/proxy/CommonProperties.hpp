//
//  CommonProperties.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_CommonProperties_hpp
#define CYTOK_CommonProperties_hpp

#include "ObjectPropertyFactory.hpp"
#include "ObjectProperty.hpp"
#include "ObjectPropertyGeneric.hpp"

namespace ck
{
	namespace proxy
    {
        class IntProperty : public ObjectPropertyGeneric<int>
        {
        public:
            
            DEFINE_PROPERTY(ObjectProperty::INTEGER, IntProperty);
            
            IntProperty()
            : ObjectPropertyGeneric<int>()
            {}
            
            IntProperty(const std::string& name,
                        Functor<int>* getter,
                        Functor<void, int>* setter)
            : ObjectPropertyGeneric<int>(name,getter,setter)
            {}
            
        };
        
        class FloatProperty : public ObjectPropertyGeneric<float>
        {
        public:
            
            DEFINE_PROPERTY(ObjectProperty::FLOAT, FloatProperty);
            
            FloatProperty()
            : ObjectPropertyGeneric<float>()
            {}
            
            FloatProperty(const std::string& name,
                        Functor<float>* getter,
                        Functor<void, float>* setter)
            : ObjectPropertyGeneric<float>(name,getter,setter)
            {}
            
        };

        class StringProperty : public ObjectPropertyGeneric<std::string>
        {
        public:
            
            DEFINE_PROPERTY(ObjectProperty::STRING, StringProperty);
            
        };
        
    }
}

#endif
