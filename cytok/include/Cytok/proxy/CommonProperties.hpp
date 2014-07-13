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

#include "ObjectPropertyRequiredBase.hpp"

namespace ck
{
	namespace proxy
    {
        class IntProperty : public IntPropertyBase
        {
        public:
            
            DEFINE_PROPERTY(ObjectProperty::INTEGER, IntProperty);
            
            DEFINE_VISITABLE();
            
            IntProperty()
            : ObjectPropertyGeneric<int>()
            {}
            
            IntProperty(const std::string& name,
                        Functor<int>* getter,
                        Functor<void, int>* setter)
            : ObjectPropertyGeneric<int>(name,getter,setter)
            {}
        };
        
        class FloatProperty : public FloatPropertyBase
        {
        public:
            
            DEFINE_PROPERTY(ObjectProperty::FLOAT, FloatProperty);
            
            DEFINE_VISITABLE();
            
            FloatProperty()
            : ObjectPropertyGeneric<float>()
            {}
            
            FloatProperty(const std::string& name,
                        Functor<float>* getter,
                        Functor<void, float>* setter)
            : ObjectPropertyGeneric<float>(name,getter,setter)
            {}
            
        };

        class StringProperty : public StringPropertyBase
        {
        public:
            
            DEFINE_PROPERTY(ObjectProperty::STRING, StringProperty);
            
            DEFINE_VISITABLE();
            
            
            inline virtual std::string valueToString()
            {
                return this->value() ;
            }
            
            inline virtual void valueFromString(const std::string& str)
            {
                this->setValue(str);
            }

            
        };
        
    }
}

#endif
