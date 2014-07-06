//
//  Object.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 04/07/2014.
//
//

#ifndef CYTOK_Object_hpp
#define CYTOK_Object_hpp

#include <string>

#include "NumericTypes.hpp"

#include "ID.hpp"

namespace ck
{
	class Object 
	{
    public:

        Object();
        
        virtual ~Object(){};
        
        ID id();
        
        virtual std::string toString();
        
    protected:
        
        template<typename This>
        void fetchID(This thisObj);
        
    private:
        
        ID myID = CK_INVALID_ID;
	};
}

#include "Object_impl.hpp"

#endif
