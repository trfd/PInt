//
//  ObjectPropertyFactory.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_ObjectPropertyFactory__hpp
#define CYTOK_ObjectPropertyFactory__hpp

#include "Cytok/Functor.hpp"
#include "Cytok/utils/Singleton.hpp"
#include "Cytok/Factory.hpp"

#include "ObjectProperty.hpp"


namespace ck
{
    namespace proxy
    {
        ///
        ///
        ///
        class ObjectPropertyFactoryMap
        : public FactoryMap
            <
                ObjectProperty,
                ObjectProperty::PropertyType ,
                Functor<ObjectProperty*>
            >,
        public utils::Singleton<ObjectPropertyFactoryMap>
        {};
        
        ///
        ///
        ///
        class ObjectPropertyFactory
        : public Factory
            <
                ObjectProperty,
                ObjectProperty::PropertyType,
                Functor<ObjectProperty*>
            >
        {
        public:
            
            static ObjectPropertyFactory* currentFactory()
            {
                return myCurrFactory;
            }
            
            static void setCurrentFactory(ObjectPropertyFactory* f)
            {
                myCurrFactory = f;
            }

            ObjectPropertyFactory()
            : Factory(ObjectPropertyFactoryMap::instance())
            {}
            
        private:
            
            static ObjectPropertyFactory* myCurrFactory;
        };
        
        template<typename Type>
        struct ObjectPropertyFactoryRegister
        {
            ObjectPropertyFactoryRegister()
            {
                Type::registerObjectPropertyFactory();
            }
        };
    }
}

#endif
