
#include "ai/WorldMap.hpp"
#include "ai/GameObject.hpp"
#include "ai/PhysicBody.hpp"

#define GRAVITY 1

void PhysicBody::onUpdate(float dt)
{
    m_body->getMotionState()->getWorldTransform(_gameObject->transform());

    btVector3 pos = _gameObject->transform().getOrigin();

    if(pos.z() - m_size.Z*0.5f < WorldMap::worldHeight(pos.x(), pos.y()))
    {
        pos.setZ(WorldMap::worldHeight(pos.x(), pos.y())+m_size.Z*0.5f);
        
        _gameObject->transform().setOrigin(pos);
    }
}

void PhysicBody::setTransform()
{
    m_body->getMotionState()->setWorldTransform(_gameObject->transform());
}
