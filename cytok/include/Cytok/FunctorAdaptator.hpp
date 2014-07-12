//
//  FunctorAdaptator.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 12/07/2014.
//
//

#ifndef CYTOK_FunctorAdaptator_hpp
#define CYTOK_FunctorAdaptator_hpp

#include "Functor.hpp"

namespace ck
{

    
    template
    <
        typename OutReturnType,
        typename... OutArgs
    >
	class FunctorAdaptator 
	{
    public:
        template
        <
            typename InReturnType,
            typename... InArgs
        >
        OutReturnType operator()(Functor<InReturnType,InArgs...> funct, OutArgs... args)
        {
            return (OutReturnType) funct( (InArgs) args...);
        }
        
	};
}

#endif
