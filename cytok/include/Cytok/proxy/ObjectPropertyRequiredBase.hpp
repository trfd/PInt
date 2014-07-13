//
//  ObjectPropertyRequiredBase.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 13/07/2014.
//
//

#ifndef CYTOK_ObjectPropertyRequiredBase_hpp
#define CYTOK_ObjectPropertyRequiredBase_hpp

#include "ObjectPropertyGeneric.hpp"

namespace ck
{
	namespace proxy
    {
        typedef ObjectPropertyGeneric<int> IntPropertyBase;
        typedef ObjectPropertyGeneric<float> FloatPropertyBase;
        typedef ObjectPropertyGeneric<std::string,const std::string&> StringPropertyBase;
        
    }
}

#endif
