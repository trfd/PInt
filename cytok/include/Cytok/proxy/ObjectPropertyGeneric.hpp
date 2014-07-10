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
#include "Cytok/proxy/ObjectProperty.hpp"

namespace ck
{
    namespace proxy
    {
        template<
            typename GetType,
            typename SetType = GetType
        >
        class ObjectPropertyGeneric  : public ObjectProperty
        {
            
        public:
            
            ObjectPropertyGeneric(const std::string& name,
                                  Functor<GetType> getter ,
                                  Functor<void,SetType> setter)
            : ObjectProperty(name),
            myGetter(getter) , mySetter(setter)
            {}
            
            inline GetType value()
            {
                return myGetter();
            }
            
            inline void setValue(SetType value)
            {
                mySetter(value);
            }
            
            inline virtual std::string valueToString()
            {
                return boost::lexical_cast<std::string>( myGetter() );
            }
            
            inline virtual void valueFromString(const std::string& str)
            {
                mySetter(boost::lexical_cast<SetType>(str));
            }
            
        private:
            
            Functor<GetType> myGetter;
            Functor<void,SetType> mySetter;
            
        };
    }
}

#endif
