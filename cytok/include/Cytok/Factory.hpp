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

#include "Cytok/FactoryMap.hpp"
#include "Cytok/FactoryException.hpp"

namespace ck
{
    template<
        typename AbstractProduct,
        typename ProductID,
        typename ProductCreator,
        template<typename>class ErrorPolicy = FactoryError
    >
	class Factory 
	{
    public:
        
        typedef FactoryMap<AbstractProduct, ProductID, ProductCreator, ErrorPolicy> DelegatedMap;
        
        Factory(DelegatedMap* pMap)
        : mMap(pMap)
        {}
        
        void setMap(DelegatedMap* pMap)
        {
            mMap = pMap;
        }
        
        inline virtual void registerCreator(ProductID id , ProductCreator creator)
        {
            if(mMap)
                mMap->registerCreator(id,creator);
        }
        
        inline void unregisterCreator(ProductID id)
        {
            if(mMap)
                mMap->unregisterCreator(id);
        }

        virtual AbstractProduct* createProduct(ProductID id)
        {
            if(!mMap)
                ErrorPolicy<ProductID>::OnNullMap();
            
            ProductCreator* creator = mMap->productCreator(id);
            
            if(!creator)
                return NULL; // Assume that map already call ErrorPolicy
            
            return (*creator)();
        }
        
    protected:
        
        DelegatedMap* mMap;
	};
}

#endif
