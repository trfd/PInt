//
//  ObjectProxy.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 09/07/2014.
//
//

#ifndef CYTOK_ObjectProxy_hpp
#define CYTOK_ObjectProxy_hpp

#include <string>
#include <vector>

#include "Cytok/Object.hpp"

namespace ck
{
    namespace proxy
    {
        class ObjectProperty;
        
        class ObjectProxy 
        {
        public:
            
            typedef std::vector<ObjectProperty*> PropertyList;
            typedef std::vector<ObjectProperty*>::iterator PropertyList_it;
            
            ObjectProxy(ck::Object* obj)
            {
                myObject = obj;
            }
            
            void addProperty(ObjectProperty* prop);
            
            std::vector<ObjectProperty*>& properties();
            
            ObjectProperty* property(int atIdx);
            
            ObjectProperty* property(std::string pName);
            
            ck::Object* object();
            
            virtual void buildPropertyList();
            
            virtual ~ObjectProxy(){};
            
        private:
            
            std::vector<ObjectProperty*> myProperties;
            
            ck::Object* myObject;
        };
    }
}

#endif
