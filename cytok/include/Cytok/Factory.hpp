//
//  Factory.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#ifndef CYTOK_Factory_hpp
#define CYTOK_Factory_hpp

#include <map>

#include "Cytok/FactoryException.hpp"

namespace ck
{
    template<
        typename AbstractProduct,
        typename ProductID,
        typename ProductCreator,
        template<typename>class ErrorPolicy = FactoryException
    >
	class Factory 
	{
    public:
        
        typedef std::map<ProductID,ProductCreator> FactoryMap;
        typedef std::pair<ProductID,ProductCreator> FactoryPair;
        typedef typename std::map<ProductID,ProductCreator>::iterator FactoryMap_it;
        
        virtual void registerProduct(ProductID id , ProductCreator creator)
        {
            myAssociations.insert(FactoryPair(id , creator));
        }
        
        virtual void unregisterProduct(ProductID id)
        {
            myAssociations.erase(id);
        }
        
        virtual AbstractProduct* createProduct(ProductID id)
        {
            FactoryMap_it it = myAssociations.find(id);
            
            if(it == myAssociations.end())
            {
                ErrorPolicy<ProductID>::OnUnknownType(id);
                return NULL;
            }
            
            return (it->second)();
        }
        
    private:
        
        FactoryMap myAssociations;
        
	};
}

#endif
