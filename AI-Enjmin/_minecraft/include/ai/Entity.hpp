#ifndef __ENTITY__
#define __ENTITY__

#include "ai/GameManager.hpp"
#include "ai/GameObject.hpp"
#include "ai/MeshRenderer.hpp"
#include "ai/PhysicBody.hpp"
#include "ai/Agent.hpp"

#include "ai/PreyAgent.hpp"
#include "ai/PredatorAgent.hpp"

using namespace ai::bt;


void createPreyMesh(MeshRenderer* rd)
{
    NYColor c =  NYColor(123.f/255.f,79.f/255.f,247.f/255.f,1.f);

    // Body
    rd->mesh().addCube(btVector3(0, 0, -5), btVector3(8,7,10) ,c);

    // Head
    rd->mesh().addCube(btVector3(0, -3, 1), btVector3(5,5,5) ,c);

    // Arms
    rd->mesh().addCube(btVector3(0, -1, -3.5f), btVector3(9.5,2,5) ,c);

    // Feet
    rd->mesh().addCube(btVector3(-2, -1, -10), btVector3(3,3,2) ,c);
    rd->mesh().addCube(btVector3( 2, -1, -10), btVector3(3,3,2) ,c);
    
    // Tail
    rd->mesh().addCube(btVector3( 0, 3, -8), btVector3(3,3,2) ,c);
    rd->mesh().toVbo();
}

void createPrey(btVector3 const& loc_)
{
    GameObject_sptr obj = GameManager::instance()->create<GameObject>();

    PhysicBody* body = obj->emplaceComponent<PhysicBody>(GameObject::PHYSIC_UPDATE_PRIORITY,NYVert3Df(10,10,10));

    MeshRenderer* renderer = obj->emplaceComponent<MeshRenderer>(GameObject::DEFAULT_UPDATE_PRIORITY);

    createPreyMesh(renderer);

    obj->setPosition(loc_);

    body->setTransform();

    PreyAgent* preyAgent = obj->emplaceComponent<PreyAgent>(GameObject::DEFAULT_UPDATE_PRIORITY);

    preyAgent->setBehaviours(BehaviourTreeBuilder()
    .add<Selector>()
        .add<Sequence>()
            .add<Condition>(ck::makeFunctor(preyAgent, &PreyAgent::seeClosePredator))
            .add<Action>(preyAgent, new PreyAgent::FleeAction())
        .parent()
        .add<Sequence>()
            .add<Condition>(ck::makeFunctor(preyAgent,&PreyAgent::isHungry))
            .add<Selector>()
            /**/.add<Sequence>()
            /**/    .add<Condition>(ck::makeFunctor(preyAgent,&PreyAgent::isCloseToFood))
            /**/    .add<Action>(preyAgent, new PreyAgent::EatAction())
            /**/.parent()
            /**/.add<Sequence>()
            /**/    .add<Condition>(ck::makeFunctor(preyAgent,&PreyAgent::seeFood))
            /**/    .add<Action>(preyAgent, new PreyAgent::GotoFoodAction())
            /**/.parent()
            /**/.add<Action>(preyAgent, new PreyAgent::WanderAction())
           .parent()
       .parent()
       .add<Sequence>()
            .add<Condition>(ck::makeFunctor(preyAgent,&PreyAgent::isInGroup))
            .add<Action>(preyAgent, new PreyAgent::FlockAction())
        .parent()
        .add<Sequence>()
            .add<Condition>(ck::makeFunctor(preyAgent,&PreyAgent::isCloseToGroup))
            .add<Action>(preyAgent, new PreyAgent::JoinGroupAction())
        .parent()
        .add<Sequence>()
            .add<Condition>(ck::makeFunctor(preyAgent,&PreyAgent::seeGroup))
            .add<Action>(preyAgent, new PreyAgent::GotoGroupAction())
        .parent()
        .add<Action>(preyAgent, new PreyAgent::WanderAction())
    .end());
}


void createPredatorMesh(MeshRenderer* rd)
{
    NYColor c =  NYColor(243.f/255.f,166.f/255.f,1.f/255.f,1.f);

    // Body
    rd->mesh().addCube(btVector3(0, 0, -5), btVector3(8,7,10) ,c);

    // Head
    rd->mesh().addCube(btVector3(0, -3, 1), btVector3(5,5,5) ,c);

    // Arms
    rd->mesh().addCube(btVector3(0, -1, -3.5f), btVector3(9.5,2,5) ,c);

    // Feet
    rd->mesh().addCube(btVector3(-2, -1, -10), btVector3(3,3,2) ,c);
    rd->mesh().addCube(btVector3( 2, -1, -10), btVector3(3,3,2) ,c);
    
    // Tail
    rd->mesh().addCube(btVector3( 0, 3, -8), btVector3(3,3,2) ,c);
    rd->mesh().toVbo();
}

void createPredator(btVector3 const& loc_)
{
    GameObject_sptr obj = GameManager::instance()->create<GameObject>();

    PhysicBody* body = obj->emplaceComponent<PhysicBody>(GameObject::PHYSIC_UPDATE_PRIORITY,NYVert3Df(10,10,10));

    MeshRenderer* renderer = obj->emplaceComponent<MeshRenderer>(GameObject::DEFAULT_UPDATE_PRIORITY);

    createPredatorMesh(renderer);

    obj->setPosition(loc_);

    body->setTransform();

    PredatorAgent* predatorAgent = obj->emplaceComponent<PredatorAgent>(GameObject::DEFAULT_UPDATE_PRIORITY);

    predatorAgent->setBehaviours(BehaviourTreeBuilder()
    .add<Selector>()
        .add<Sequence>()
        /**/.add<Condition>(ck::makeFunctor(predatorAgent, &PredatorAgent::isHungry))                            // If is hungry
        /**/.add<Selector>()
        /**/    .add<Sequence>()
        /**/        .add<Condition>(ck::makeFunctor(predatorAgent, &PredatorAgent::isCloseToPrey))              // If close to prey AND
        /**/        .add<Selector>()
        /**/            .add<Condition>(ck::makeFunctor(predatorAgent,&PredatorAgent::targetPreyIsAlone))       // If prey is alone
        /**/            .add<Condition>(ck::makeFunctor(predatorAgent,&PredatorAgent::isVeryHungry))            // OR is very hungry
        /**/        .parent()
        /**/        .add<Action>(predatorAgent,new PredatorAgent::AttackPreyAction())                           // Then attack
        /**/    .parent()
        /**/    .add<Sequence>()
        /**/        .add<Condition>(ck::makeFunctor(predatorAgent, &PredatorAgent::seePrey))                    // If see to prey AND
        /**/        .add<Sequence>()
        /**/            .add<Selector>()
        /**/                .add<Condition>(ck::makeFunctor(predatorAgent,&PredatorAgent::targetPreyIsAlone))     // If prey is alone
        /**/                .add<Condition>(ck::makeFunctor(predatorAgent,&PredatorAgent::isVeryHungry))         // OR is very hungry
        /**/            .parent()
        /**/            .add<Action>(predatorAgent,new PredatorAgent::GotoPreyAction())                      // Then goto prey
        /**/        .parent()
        /**/        .add<Action>(predatorAgent, new PredatorAgent::FollowPreyGroupAction())                     //Else Follow group
        /**/    .parent()
        /**/.parent()
        .parent()
        .add<Action>(predatorAgent, new PredatorAgent::WanderAction())
    .end());
}

#endif // __ENTITY__