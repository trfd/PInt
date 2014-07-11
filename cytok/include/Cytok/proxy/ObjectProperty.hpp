//
//  ObjectProperty.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 09/07/2014.
//
//

#ifndef CYTOK_ObjectProperty_hpp
#define CYTOK_ObjectProperty_hpp

#include <string>
#include <vector>

#include "ObjectPropertyHolder.hpp"

namespace ck
{
    namespace proxy
    {
        class ObjectProperty : public IObjectPropertyHolder
        {
            
        public:
            
            enum PropertyType
            {
                NONE        = 0,
                
                INTEGER     = 1,
                FLOAT       = 2,
                STRING      = 3,
                ENUM        = 4,
                OBJECT      = 5,
                POINTER     = 6,
                REFERENCE   = 7,
                ARRAY       = 8,
                MAP         = 9
            };

            
            ObjectProperty(const std::string& name,PropertyType type = NONE);
            
            std::string& name();
            
            PropertyType type();
            
            virtual ~ObjectProperty(){};
            
            virtual std::string valueToString() = 0;
            
            virtual void valueFromString(const std::string&) = 0;
        
            virtual size_t propertyCount();
            
            virtual PropertyList* properties();
            
            virtual ObjectProperty* property(int atIdx);
            
            virtual ObjectProperty* property(std::string pName);
            
        private:
            
            std::string myName;
            
            PropertyType myType;
        };
    }
}

#endif
