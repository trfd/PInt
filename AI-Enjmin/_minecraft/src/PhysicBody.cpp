
#include "ai/WorldMap.hpp"
#include "ai/GameObject.hpp"
#include "ai/PhysicBody.hpp"

#define GRAVITY 1

void PhysicBody::onUpdate(float dt)
{
    btTransform tr;
    m_body->getMotionState()->getWorldTransform(tr);

    // Check if not under the map

    btVector3 pos = tr.getOrigin();

    if(pos.z() - m_size.Z*0.5f < WorldMap::worldHeight(pos.x(), pos.y()))
    {
        pos.setZ(WorldMap::worldHeight(pos.x(), pos.y())+m_size.Z*0.5f);
        
        tr.setOrigin(pos);
    }

    // Update game object transform;

    _gameObject->setTransform(tr);
}

void PhysicBody::setTransform()
{
    m_body->getMotionState()->setWorldTransform(_gameObject->transform());
}
