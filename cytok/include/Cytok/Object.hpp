//
//  Object.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 04/07/2014.
//
//

#ifndef CYTOK_Object_hpp
#define CYTOK_Object_hpp

#include <sstream>

#include "Cytok/Logs.hpp"

#include "Cytok/NumericTypes.hpp"

#include "Cytok/ID.hpp"

namespace ck
{
	class Object 
	{

        friend std::ostream& operator <<(std::ostream& stream, const Object& obj);
        
    public:

        /// Static
        
        template<typename ObjectType , typename... Args>
        static ObjectType* create(Args... args);
        
        /// Constructor/Destructor
        
        Object();
        
        virtual ~Object(){};
        
        /// Accessor
        
        ID id();
        
        virtual void description(std::ostringstream& stream);
        
        std::string toString();
        
        virtual void describe(Log::ContextInfo ctx = Log::ENGINE , Log::Type logType = Log::INFO);
    
    protected:
        
        /// Flow
        
        virtual void init(){}
        
    private:
     
        ID myID = CK_INVALID_ID;
	};
}

#include "Object_impl.hpp"

#endif
