#ifndef _PredatorAgent_hpp_
#define _PredatorAgent_hpp_

#include "Agent.hpp"

#define __PREDATOR_DAMAGES__             5.f
#define __PREDATOR_HUNGER_LOW_THRESHOLD__  50.f
#define __PREDATOR_HUNGER_HIGH_THRESHOLD__ 20.f
#define __PREDATOR_HUNGER_RATE__          0.05f
#define __PREDATOR_HUNGER_FULL__          100.f
#define __PREDATOR_FOOD_POWER__           20.f
#define __PREDATOR_HUNGER_KILL_RATE__      0.1f
#define __PREDATOR_WANDER_RADIUS__         200.f  

class PreyAgent;

class PredatorAgent : public Agent
{
public:
 
    virtual void init() override
    {
        Agent::init();

        _visionZone = CellSize(30,30);

        _visionForwardOffset = 15;

        _lifepoints = 100.f;

        _velocity = 60.f;

        m_preyTarget = nullptr;
    }

    virtual void onRender() override
    {
        btVector3 pos = _gameObject->transform().getOrigin() + btVector3(0,0,15);

        float a = 5.f;

        glPushMatrix();
        glTranslatef(pos.x(),pos.y(),pos.z());
        glBegin(GL_QUADS);

        float g;

        if(!isHungry()) g = 0.f;
        else if(isVeryHungry()) g = 1.f;
        else g = 0.5f;

        

        glColor3f(0.f,g,0.f);

        glVertex3f( -a , 0 , -a);
        glVertex3f(  a , 0 , -a);
        glVertex3f(  a , 0 ,  a);
        glVertex3f( -a , 0 ,  a);

        glVertex3f( -a , 0 , -a);
        glVertex3f( -a , 0 ,  a);
        glVertex3f(  a , 0 ,  a);
        glVertex3f(  a , 0 , -a);

        glEnd();
        glPopMatrix();
    }
    

    virtual void onAIUpdate(float dt) override
    {
        Agent::onAIUpdate(dt);

        m_hunger -= __PREDATOR_HUNGER_RATE__;

        m_hunger = max(0.f, m_hunger);

        if(m_hunger == 0.f)
        {
            m_lifepoints -= __PREDATOR_HUNGER_KILL_RATE__;

            if(m_lifepoints <= 0)
            {
               // std::cout << "Prey is died \n";
            }
        }
    }

    #pragma region Condition

    inline bool isHungry()
    {
       return (m_hunger <= __PREDATOR_HUNGER_LOW_THRESHOLD__);
    }

    inline bool isVeryHungry()
    {
       return (m_hunger <= __PREDATOR_HUNGER_HIGH_THRESHOLD__);
    }

    bool isCloseToPrey();

    bool isCloseTo(PreyAgent* ag);

    bool targetPreyIsAlone();

    bool seePrey();

    bool isHidden();

    #pragma endregion

    struct AttackPreyAction : public BehaviourAction<PredatorAgent>
    {
        virtual void run() override;
    };

    struct FollowPreyGroupAction : public BehaviourAction<PredatorAgent>
    {
        virtual void run() override;
    };

    struct GotoPreyAction : public BehaviourAction<PredatorAgent>
    {
        virtual void run() override;
    };

    struct WanderAction : public BehaviourAction<PredatorAgent>
    {
        virtual void run() override;

        /// Returns the angle opposite to the average of groups seen
        /// if any, otherwise returns a random angle
        float avoidGroups();
    };

private:

    float m_hunger      = __PREDATOR_HUNGER_FULL__; 
    float m_lifepoints  = 100.f; 

    PreyAgent* m_preyTarget;

};

#endif //_PredatorAgent_hpp_