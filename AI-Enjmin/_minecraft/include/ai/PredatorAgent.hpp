#ifndef _PredatorAgent_hpp_
#define _PredatorAgent_hpp_

#include "MeshRenderer.hpp"
#include "Agent.hpp"

#define __PREDATOR_DAMAGES__             5.f
#define __PREDATOR_HUNGER_LOW_THRESHOLD__  50.f
#define __PREDATOR_HUNGER_HIGH_THRESHOLD__ 20.f
#define __PREDATOR_HUNGER_RATE__          0.05f
#define __PREDATOR_HUNGER_FULL__          100.f
#define __PREDATOR_FOOD_POWER__           20.f
#define __PREDATOR_HUNGER_KILL_RATE__      0.1f
#define __PREDATOR_WANDER_RADIUS__        200.f  
#define __PREDATOR_ATTACK_HEAL__          10.f
#define __PREDATOR_LIFE_CAP__            200.f
#define __PREDATOR_DEFAULT_VELOCITY__     55.f
#define __PREDATOR_DEFAULT_LP__          100.f

class PreyAgent;

class PredatorAgent : public Agent
{
public:
 
    virtual void init() override
    {
        Agent::init();

        _visionZone = CellSize(30,30);

        _visionForwardOffset = 15;

        _lifepoints = __PREDATOR_DEFAULT_LP__;

        _velocity = __PREDATOR_DEFAULT_VELOCITY__;

        m_preyTarget = nullptr;

        m_mesh = _gameObject->findComponent<MeshRenderer>();

        m_body = _gameObject->findComponent<PhysicBody>();
    }

    virtual void respawn() override
    {
         Agent::respawn();

         _lifepoints = __PREDATOR_DEFAULT_LP__;

         _velocity = __PREDATOR_DEFAULT_VELOCITY__;

         m_size = 1.0f;

         m_preyTarget = nullptr;
    }

    virtual void onRender() override
    {
        btVector3 pos = _gameObject->transform().getOrigin() + btVector3(0,0,15);

        float a = 5.f;

        glPushMatrix();
        glTranslatef(pos.x(),pos.y(),pos.z());
        glBegin(GL_QUADS);
        
        if(_isDead)
            glColor3f(1.f,0.f,0.f);
        else
        {
            float g;

            if(!isHungry()) g = 0.f;
            else if(isVeryHungry()) g = 1.f;
            else g = 0.5f;
            glColor3f(0.f,g,0.f);
        }
        

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

        if(_lifepoints <= 0)
        {
            die();
            return;
        }
        else
        {
            _velocity = __PREDATOR_DEFAULT_VELOCITY__ * (__PREDATOR_DEFAULT_LP__/_lifepoints);

            m_size = min(2.f,_lifepoints/__PREDATOR_DEFAULT_LP__);

            m_mesh->setScale(m_size);

            m_body->setBoxSize(btVector3(10,10, 10 * m_size));
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
        virtual void onStart() override;

        virtual void run() override;

        /// Returns the angle opposite to the average of groups seen
        /// if any, otherwise returns a random angle
        float avoidGroups();

        void newTarget();
    };

private:

    float m_hunger      = __PREDATOR_HUNGER_FULL__; 
    float m_size;
    
    PreyAgent* m_preyTarget;

    MeshRenderer* m_mesh;

    PhysicBody* m_body;

};

#endif //_PredatorAgent_hpp_