
#include "ai/GameObject.hpp"
#include "ai/PhysicBody.hpp"

#define GRAVITY 1

void PhysicBody::onUpdate(float dt)
{
    m_body->getMotionState()->getWorldTransform(_gameObject->transform());
}

void PhysicBody::setTransform()
{
    m_body->getMotionState()->setWorldTransform(_gameObject->transform());
}
