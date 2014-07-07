//
//  Action.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 04/07/2014.
//
//

#ifndef CYTOK_Action_hpp
#define CYTOK_Action_hpp

#include "Object.hpp"

namespace ck
{
    
	class Action : public Object
	{
    public:
        
        enum State
        {
            INVALID = 0,
            SUCCESS = 1,
            FAILURE = 2
        };
        
        virtual State perform() = 0;
        
	};
}

#endif
