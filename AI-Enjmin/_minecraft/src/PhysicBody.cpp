
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

    pos.setX(max(0,min(c_worldSize * NYCube::CUBE_SIZE, pos.x())));
    pos.setY(max(0,min(c_worldSize * NYCube::CUBE_SIZE, pos.y())));
    pos.setZ(min(MAT_HEIGHT_CUBES * NYCube::CUBE_SIZE, pos.z()));

    tr.setOrigin(pos);

    // Update game object transform;

    _gameObject->setTransform(tr);
}

void PhysicBody::setTransform()
{
    m_body->getMotionState()->setWorldTransform(_gameObject->transform());
}
