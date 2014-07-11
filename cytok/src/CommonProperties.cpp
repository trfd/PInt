//
//  CommonProperties.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#include "Cytok/proxy/ProxyMacros.hpp"
#include "Cytok/proxy/ObjectPropertyFactory.hpp"
#include "Cytok/proxy/CommonProperties.hpp"

namespace ck
{
    namespace proxy
    {
        REGISTER_PROPERTY(IntProperty);

        REGISTER_PROPERTY(FloatProperty);
        
        REGISTER_PROPERTY(StringProperty);
    }
}