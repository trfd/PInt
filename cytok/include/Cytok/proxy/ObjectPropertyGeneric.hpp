//
//  ObjectPropertyGeneric.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 09/07/2014.
//
//

#ifndef CYTOK_ObjectPropertyGeneric_hpp
#define CYTOK_ObjectPropertyGeneric_hpp

#include <type_traits>

#include <boost/lexical_cast.hpp>

#include "Cytok/Functor.hpp"
#include "Cytok/FunctorAdaptator.hpp"

#include "Cytok/proxy/ProxyExceptions.hpp"
#include "Cytok/proxy/ObjectProperty.hpp"

namespace ck
{
    namespace proxy
    {
        /////////////////////////////////////////////////////
        ///
        /////////////////////////////////////////////////////
        template
        <
            typename OutGetType,
            typename OutSetType
        >
        class IObjectPropertyGeneric_impl
        {
        public:
            
            virtual ~IObjectPropertyGeneric_impl(){}
            
            virtual OutGetType getValue() = 0;
            virtual void setValue(OutSetType&& value) = 0;
        };
        
        /////////////////////////////////////////////////////
        ///
        /////////////////////////////////////////////////////
        template
        <
            typename OutGetType,
            typename OutSetType,
            typename InGetType,
            typename InSetType
        >
        class ObjectPropertyGeneric_impl
        : public IObjectPropertyGeneric_impl<OutGetType, OutSetType>
        {
        public:
            
            ObjectPropertyGeneric_impl(Functor<InGetType>* getter,
                                       Functor<void,InSetType>* setter)
            : myGetter(getter),
            mySetter(setter)
            {
            }
            
            template<typename BaseClass>
            ObjectPropertyGeneric_impl(BaseClass* ptr,
                                       InGetType(BaseClass::*getter)(),
                                       void(BaseClass::*setter)(InSetType))
            : myGetter(makeNewFunctor(ptr, getter)),
            mySetter(makeNewFunctor(ptr, setter))
            {
            }
            
            template<typename BaseClass>
            ObjectPropertyGeneric_impl(InGetType(*getter)(),
                                       void(*setter)(InSetType))
            : myGetter(makeNewFunctor(getter)),
            mySetter(makeNewFunctor(setter))
            {
            }
            
            ~ObjectPropertyGeneric_impl()
            {
                if(myGetter)
                    delete myGetter;
                
                if(mySetter)
                    delete mySetter;
            }

            template<typename BaseClass>
            void setAccessors(BaseClass* ptr,
                              InGetType(BaseClass::*getter)(),
                              void(BaseClass::*setter)(InSetType))
            {
                if(myGetter)
                    delete myGetter;
                
                if(mySetter)
                    delete mySetter;
                
                myGetter = makeNewFunctor(ptr, getter);
                mySetter = makeNewFunctor(ptr, setter);
            }

            void setAccessors(Functor<InGetType>* getter,
                              Functor<void,InSetType>* setter)
            {
                if(myGetter)
                    delete myGetter;
                
                if(mySetter)
                    delete mySetter;
                
                myGetter = getter;
                mySetter = setter;
            }
            
            template<typename BaseClass>
            void setGetter(BaseClass* ptr, InGetType(BaseClass::*getter)())
            {
                if(myGetter)
                    delete myGetter;
                
                myGetter = makeNewFunctor(ptr , getter);
            }
            
            
            template<typename BaseClass>
            void setSetter(BaseClass* ptr, void(BaseClass::*setter)(InSetType))
            {
                if(mySetter)
                    delete mySetter;
                
                mySetter = makeNewFunctor(ptr,setter);
            }
            
            void setGetter(Functor<InGetType>* getter)
            {
                if(myGetter)
                    delete myGetter;
                
                myGetter = getter;
            }
            
            void setSetter(Functor<void,InSetType>* setter)
            {
                if(mySetter)
                    delete mySetter;
                
                mySetter = setter;
            }
            
            virtual OutGetType getValue()
            {
                return myGetterAdaptator(myGetter);
            }
            
            virtual void setValue(OutSetType&& value)
            {
                return mySetterAdaptator(mySetter,std::forward<OutSetType>(value));
            }
            
        private:
            
            FunctorAdaptator<OutGetType> myGetterAdaptator;
            FunctorAdaptator<void,OutSetType> mySetterAdaptator;
            
            Functor<InGetType>* myGetter        = NULL;
            Functor<void,InSetType>* mySetter   = NULL;
            
        };
        
        /////////////////////////////////////////////////////
        ///
        /////////////////////////////////////////////////////
        template
        <
            typename GetType,
            typename SetType = GetType
        >
        class ObjectPropertyGeneric  : public ObjectProperty
        {
            
        public:
            
            DEFINE_VISITABLE();
            
            ObjectPropertyGeneric()
            {}
            
            template
            <
                typename InGetType,
                typename InSetType
            >
            ObjectPropertyGeneric(const std::string& name,
                                  Functor<InGetType>* getter ,
                                  Functor<void,InSetType>* setter)
            : ObjectProperty(name),
            myImpl(new ObjectPropertyGeneric_impl
                   <GetType,SetType,InGetType,InSetType>(getter,setter))
            {}
            
            template
            <
                typename BaseClass,
                typename InGetType,
                typename InSetType
            >
            void setAccessors(BaseClass* ptr,
                              InGetType(BaseClass::*getter)(),
                              void(BaseClass::*setter)(InSetType))
            {
                if(myImpl)
                    delete myImpl;
                
                myImpl = new ObjectPropertyGeneric_impl
                <GetType,SetType,InGetType,InSetType>(ptr,getter,setter);
            }
            
            template
            <
                typename InGetType,
                typename InSetType
            >
            void setAccessors(Functor<InGetType>* getter,
                              Functor<void,InSetType>* setter)
            {
                if(myImpl)
                    delete myImpl;
                
                myImpl = new ObjectPropertyGeneric_impl
                <GetType,SetType,InGetType,InSetType>(getter,setter);
            }

            inline virtual std::string valueToString()
            {
                return boost::lexical_cast<std::string>( myImpl->getValue() );
            }
            
            inline virtual void valueFromString(const std::string& str)
            {
                myImpl->setValue(boost::lexical_cast
                                 <
                                    typename std::remove_const
                                    <
                                        typename std::remove_reference<SetType>::type
                                    >::type
                                 >(str));
            }
      
            inline virtual GetType value()
            {
                return myImpl->getValue();
            }
            
            inline virtual void setValue(SetType&& value)
            {
                myImpl->setValue(std::forward<SetType>(value));
            }
            
        private:
            
            IObjectPropertyGeneric_impl<GetType,SetType>* myImpl    = NULL;
            
        };
    }
}

#endif
