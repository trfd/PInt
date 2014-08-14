#ifndef __GameInitializer_hpp__
#define __GameInitializer_hpp__

#include "GameObject.hpp"


static void init()
{
     GameManager::instance()->create<GameObject>();
}

#endif __GameInitializer_hpp__