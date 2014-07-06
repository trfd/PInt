//
//  Action.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 04/07/2014.
//
//

#ifndef CYTOK_Action_hpp
#define CYTOK_Action_hpp


namespace ck
{
	class IAction : public Object
	{
    public:
        
        Action();
        
        virtual void perform() = 0;
        
	};
}

#endif
