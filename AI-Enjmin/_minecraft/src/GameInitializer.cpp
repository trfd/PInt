#include "ai/GameManager.hpp"
#include "ai/GameObject.hpp"
#include "ai/CubeRenderer.hpp"
#include "ai/PhysicBody.hpp"

#include "ai/Entity.hpp"

#define __PREY_SPAWN_COUNT__ 40
#define __PREDATOR_SPAWN_COUNT__ 40

void GameInitializer::init()
{
    std::cout << "World size: " << c_worldSize << "x" << c_worldSize << "\n";

    for(int i = 0 ; i<__PREY_SPAWN_COUNT__ ; i++)
    { 
        Cell c(rand()%c_worldSize,rand()%c_worldSize);

        float height = ( WorldMap::cellHeight(c.x,c.y) + 1) * NYCube::CUBE_SIZE;

        createPrey(btVector3(c.x * NYCube::CUBE_SIZE,c.y * NYCube::CUBE_SIZE,height));
    }

    for(int i = 0 ; i<__PREDATOR_SPAWN_COUNT__ ; i++)
    { 
        Cell c(rand()%c_worldSize,rand()%c_worldSize);

        float height = ( WorldMap::cellHeight(c.x,c.y) + 1) * NYCube::CUBE_SIZE;

        createPredator(btVector3(c.x * NYCube::CUBE_SIZE,c.y * NYCube::CUBE_SIZE,height));
    }
}