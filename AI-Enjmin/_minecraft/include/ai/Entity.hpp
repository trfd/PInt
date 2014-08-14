#ifndef __ENTITY__
#define __ENTITY__

#include "ai/GameManager.hpp"
#include "ai/GameObject.hpp"
#include "ai/CubeRenderer.hpp"
#include "ai/PhysicBody.hpp"
#include "ai/Agent.hpp"

void createEntity(btVector3 const& loc_)
{
    GameObject_sptr test = GameManager::instance()->create<GameObject>();

    test->emplaceComponent<PhysicBody>(GameObject::PHYSIC_UPDATE_PRIORITY);

    test->emplaceComponent<CubeRenderer>(GameObject::DEFAULT_UPDATE_PRIORITY,
                                         btVector3(5.f, 5.f, 2.5f),
                                         btVector3(0.f,0.f,0.f));
    test->emplaceComponent<CubeRenderer>(GameObject::DEFAULT_UPDATE_PRIORITY,
                                    btVector3(0.5f, 0.5f, 1.f), 
                                    btVector3(0.f, 5.5f, 5.5f));
    test->transform().setOrigin(loc_);

    test->emplaceComponent<Agent>(GameObject::DEFAULT_UPDATE_PRIORITY);
}

#endif // __ENTITY__