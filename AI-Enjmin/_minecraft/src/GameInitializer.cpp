#include "ai/GameManager.hpp"
#include "ai/GameObject.hpp"
#include "ai/CubeRenderer.hpp"
#include "ai/PhysicBody.hpp"

#include "ai/Entity.hpp"

void GameInitializer::init()
{
    createEntity(btVector3(200,200,200));
}