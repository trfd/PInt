//
//  ObjectPropertyHolder.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#ifndef CYTOK_ObjectPropertyHolder_hpp
#define CYTOK_ObjectPropertyHolder_hpp

#include <vector>

#include <boost/any.hpp>


namespace ck
{
    namespace proxy
    {
        class ObjectProperty;
        
        class IObjectPropertyHolder
        {
        public:
            
            typedef std::vector<ObjectProperty*> PropertyList;
            typedef std::vector<ObjectProperty*>::iterator PropertyList_it;
            
            virtual size_t propertyCount() = 0;
            
            virtual PropertyList* properties() = 0;
            
            virtual ObjectProperty* property(int atIdx) = 0;
            
            virtual ObjectProperty* property(std::string pName) = 0;
            
        };
    }
}

#endif
