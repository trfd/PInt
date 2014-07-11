//
//  FactoryException.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#ifndef CYTOK_FactoryException_hpp
#define CYTOK_FactoryException_hpp

#include "boost/lexical_cast.hpp"

namespace ck
{
    class FactoryException : std::exception
    {
    };
    
    class FactoryNullMapException : FactoryException
    {
    };
    
    template<typename ProductID>
    class FactoryProductIDException : FactoryException
    {
        ProductID myId;
        
    public:
        
        FactoryProductIDException(const ProductID& id)
        {
            myId = id;
        }
        
        virtual const char* what() const throw()
        {
            try
            {
                std::string idstr = boost::lexical_cast<std::string>(myId);
                
                return (std::string("Unknow ID passed to factory: ")+idstr).c_str();
            }
            catch(boost::bad_lexical_cast &)
            {
                return "Unknow ID passed to factory";
            }
        }
        
        ProductID id()
        {
            return myId;
        }
		
    };
    
    template<typename ProductID>
    class FactoryError
    {
    public:
        
        static void OnUnknownType(const ProductID& id)
        {
            throw FactoryProductIDException<ProductID>(id);
        }
        
        static void OnNullProduct(const ProductID& id)
        {
            throw FactoryProductIDException<ProductID>(id);
        }
        
        static void OnNullMap()
        {
            throw FactoryNullMapException();
        }
	};
}

#endif
