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
    
    template<typename ProductID>
    class FactoryException : public std::exception
    {
        ProductID myId;
        
    public:
        
        FactoryException(const ProductID& id)
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
		
        static void OnUnknownType(const ProductID& id)
        {
            throw FactoryException(id);
        }
        static void OnNullProduct(const ProductID& id)
        {
            throw FactoryException(id);
        }
    
	};
}

#endif
