//
//  IAction.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 04/07/2014.
//
//

#ifndef CYTOK_IAction_hpp
#define CYTOK_IAction_hpp


namespace ck
{
	class IAction : public Object
	{
    public:
        
        virtual void perform() = 0;
        
	};
}

#endif
