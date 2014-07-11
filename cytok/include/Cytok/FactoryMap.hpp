//
//  FactoryMap.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_FactoryMap_hpp
#define CYTOK_FactoryMap_hpp

#include "FactoryMap.hpp"
#include "FactoryException.hpp"

namespace ck
{
	template<
        typename AbstractProduct,
        typename ProductID,
        typename ProductCreator,
        template<typename>class ErrorPolicy = FactoryError
    >
	class FactoryMap
	{
    public:
        
        typedef std::map<ProductID,ProductCreator> CreatorMap;
        typedef std::pair<ProductID,ProductCreator> CreatorPair;
        typedef typename std::map<ProductID,ProductCreator>::iterator CreatorMap_it;
        
        virtual void registerCreator(ProductID id , ProductCreator creator)
        {
            myAssociations.insert(CreatorPair(id , creator));
        }
        
        virtual void unregisterCreator(ProductID id)
        {
            myAssociations.erase(id);
        }
        
        virtual ProductCreator* productCreator(ProductID id)
        {
            CreatorMap_it it = myAssociations.find(id);
            
            if(it == myAssociations.end())
            {
                ErrorPolicy<ProductID>::OnUnknownType(id);
                return NULL;
            }
            
            return &(it->second);
        }
        
    private:
        
        CreatorMap myAssociations;
        
	};
}

#endif
