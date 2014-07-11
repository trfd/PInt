//
//  ObjectPropertyGeneric.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 09/07/2014.
//
//

#ifndef CYTOK_ObjectPropertyGeneric_hpp
#define CYTOK_ObjectPropertyGeneric_hpp

#include <boost/lexical_cast.hpp>

#include "Cytok/Functor.hpp"

#include "Cytok/proxy/ProxyExceptions.hpp"
#include "Cytok/proxy/ObjectProperty.hpp"

namespace ck
{
    namespace proxy
    {
        template
        <
            typename GetType,
            typename SetType = GetType
        >
        class ObjectPropertyGeneric  : public ObjectProperty
        {
            
        public:
            
            ObjectPropertyGeneric()
            : myGetter(makeNewFunctor(&invalidGetter)) ,
              mySetter(makeNewFunctor(&invalidSetter))
            {}
            
            ObjectPropertyGeneric(const std::string& name,
                                  Functor<GetType>* getter ,
                                  Functor<void,SetType>* setter)
            : ObjectProperty(name),
            myGetter(getter) , mySetter(setter)
            {}
            
            template<typename BaseClass>
            void setGetter(BaseClass* ptr, GetType(BaseClass::*getter)())
            {
                if(myGetter)
                    delete myGetter;
                
                myGetter = makeNewFunctor(ptr , getter);
            }
            
            ~ObjectPropertyGeneric()
            {
                if(myGetter)
                    delete myGetter;
                
                if(mySetter)
                    delete mySetter;
            }
            
            template<typename BaseClass>
            void setSetter(BaseClass* ptr, void(BaseClass::*setter)(SetType))
            {
                if(mySetter)
                    delete mySetter;
                
                mySetter = makeNewFunctor(ptr,setter);
            }
            
            void setGetter(Functor<GetType>* getter)
            {
                if(myGetter)
                    delete myGetter;
                
                myGetter = getter;
            }
            
            void setSetter(Functor<void,SetType>* setter)
            {
                if(mySetter)
                    delete mySetter;
                
                mySetter = setter;
            }

            
            inline virtual GetType value()
            {
                return (*myGetter)();
            }
            
            inline virtual void setValue(SetType&& value)
            {
                (*mySetter)(std::forward<SetType>(value));
            }
            
            inline virtual std::string valueToString()
            {
                return boost::lexical_cast<std::string>( (*myGetter)() );
            }
            
            inline virtual void valueFromString(const std::string& str)
            {
                (*mySetter)(boost::lexical_cast<SetType>(str));
            }
            
        private:
            
            Functor<GetType>* myGetter;
            Functor<void,SetType>* mySetter;
            
            static GetType invalidGetter()
            {
                throw InvalidPropertyException();
            }
            
            static void invalidSetter(SetType setter)
            {
                throw InvalidPropertyException();
            }
         
            
        };
    }
}

#endif
