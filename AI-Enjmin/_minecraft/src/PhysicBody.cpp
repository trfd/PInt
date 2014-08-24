
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

    pos.setX(max(5,min(c_worldSize * NYCube::CUBE_SIZE-5, pos.x())));
    pos.setY(max(5,min(c_worldSize * NYCube::CUBE_SIZE-5, pos.y())));
    pos.setZ(WorldMap::worldHeight(pos.x(), pos.y())+m_size.Z);

    tr.setOrigin(pos);

    // Update game object transform;

    _gameObject->setTransform(tr);
}

void PhysicBody::setTransform()
{
    m_body->getMotionState()->setWorldTransform(_gameObject->transform());
}
