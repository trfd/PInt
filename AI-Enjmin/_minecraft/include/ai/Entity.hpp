#ifndef __ENTITY__
#define __ENTITY__

#include "ai/GameManager.hpp"
#include "ai/GameObject.hpp"
#include "ai/MeshRenderer.hpp"
#include "ai/PhysicBody.hpp"
#include "ai/Agent.hpp"

using namespace ai::bt;

int hungryGauge = 1000;
bool isHungry()
{
    --hungryGauge;
    std::cout << "Is Hungry: (" << hungryGauge << "); " << std::boolalpha <<  (hungryGauge<0)<<"\n";
    return (hungryGauge<0);
}

void moveToFood()
{
    std::cout << "Move to food: \n";
    hungryGauge = 1000;
}

void createMesh(MeshRenderer* rd)
{
    // Body
    rd->mesh().addCube(btVector3(0, 0, -5), btVector3(8,7,10) , NYColor(123.f/255.f,79.f/255.f,247.f/255.f,1.f));

    // Head
    rd->mesh().addCube(btVector3(0, -3, 1), btVector3(5,5,5) , NYColor(123.f/255.f,79.f/255.f,247.f/255.f,1.f));

    // Arms
    rd->mesh().addCube(btVector3(0, -1, -3.5f), btVector3(9.5,2,5) , NYColor(123.f/255.f,79.f/255.f,247.f/255.f,1.f));

    // Feet
    rd->mesh().addCube(btVector3(-2, -1, -10), btVector3(3,3,2) , NYColor(123.f/255.f,79.f/255.f,247.f/255.f,1.f));
    rd->mesh().addCube(btVector3( 2, -1, -10), btVector3(3,3,2) , NYColor(123.f/255.f,79.f/255.f,247.f/255.f,1.f));
    
    // Tail
    rd->mesh().addCube(btVector3( 0, 3, -8), btVector3(3,3,2) , NYColor(123.f/255.f,79.f/255.f,247.f/255.f,1.f));
    rd->mesh().toVbo();
}

void createEntity(btVector3 const& loc_)
{
    GameObject_sptr test = GameManager::instance()->create<GameObject>();

    test->emplaceComponent<PhysicBody>(GameObject::PHYSIC_UPDATE_PRIORITY,NYVert3Df(10,10,10));

    MeshRenderer* renderer = test->emplaceComponent<MeshRenderer>(GameObject::DEFAULT_UPDATE_PRIORITY);

    createMesh(renderer);

    test->transform().setOrigin(loc_);

    Agent* testAgent = test->emplaceComponent<Agent>(GameObject::DEFAULT_UPDATE_PRIORITY);

    /*
    Sequence* seq = new Sequence();

    seq->addChild(new Condition(ck::makeFunctor(&isHungry)));
    seq->addChild(new Action(ck::makeFunctor(&moveToFood)));


    testAgent->setBehaviours(BehaviourTree(seq));
    */

    BehaviourTreeBuilder btb;
    btb = btb.add<Sequence>()
                .add<Condition>(ck::makeFunctor(&isHungry))
                .add<Action>(ck::makeFunctor(&moveToFood));
        

    testAgent->setBehaviours(BehaviourTreeBuilder()
    .add<Sequence>()
        .add<Condition>(ck::makeFunctor(&isHungry))
        .add<Action>(ck::makeFunctor(&moveToFood))
    .end());
}


#endif // __ENTITY__