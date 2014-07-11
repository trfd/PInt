//
//  UniFactory.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_UniFactory_hpp
#define CYTOK_UniFactory_hpp

namespace ck
{
    template<
        typename AbstractProduct,
        typename ProductID,
        typename ProductCreator,
        template<typename>class ErrorPolicy = FactoryError
    >
	class UniFactory : Factory
	{
    public:
        
        UniFactory()
        : Factory(new DelegatedMap())
        {}
        
    private:
        
        virtual void setMap(DelegatedMap* pMap)
        {
            mMap = pMap;
        }
	};
}

#endif
