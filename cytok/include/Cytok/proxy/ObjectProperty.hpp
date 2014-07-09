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

namespace ck
{
    namespace proxy
    {
        class ObjectProperty
        {
        public:
            
            ObjectProperty(const std::string& name);
            
            std::string& name();
            
            virtual ~ObjectProperty(){};
            
            virtual std::string valueToString() = 0;
            
            virtual void valueFromString(const std::string&) = 0;
            
        private:
            
            std::string myName;
        };
    }
}

#endif
