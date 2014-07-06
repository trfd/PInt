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

        /// Static
        
        template<typename ObjectType , typename... Args>
        static ObjectType* create(Args... args);
        
        /// Constructor/Destructor
        
        Object();
        
        virtual ~Object(){};
        
        /// Accessor
        
        ID id();
        
        virtual std::string toString();
        
        
    protected:
        
        /// Flow
        
        virtual void init(){}
        
    private:
     
        ID myID = CK_INVALID_ID;
	};
}

#include "Object_impl.hpp"

#endif
