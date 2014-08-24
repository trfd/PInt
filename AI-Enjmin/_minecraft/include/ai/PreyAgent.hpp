
#ifndef _AI_PreyAgent_hpp_
#define _AI_PreyAgent_hpp_

#include "Agent.hpp"

#include "Cluster.hpp"
#include "ClusterManager.hpp"

#include "MeshRenderer.hpp"

#define __HUNGER_FULL__      100.f
#define __HUNGER_THRESHOLD__ 40.f
#define __FOOD_POWER__       20.f
#define __HUNGER_RATE__      0.05f
#define __HUNGER_KILL_RATE__ 0.1f
#define __WANDER_RADIUS__    200.f

#define __FLOCK_COHESION_COEF__     1.0f
#define __FLOCK_ALIGNMENT_COEF__    1.0f
#define __FLOCK_SEPARATION_COEF__   1.5f
#define __FLOCK_SEPARATION_RADIUS__ 10.f

#define __PREY_DEFAULT_VELOCITY__   50.f
#define __PREY_MAX_VELOCITY__       70.f
#define __PREY_DEFAULT_LP__         50.f

#define __PREY_PREDATOR_CLOSE__     7.f

class PredatorAgent;

class PreyAgent : public Agent, public IClusterObject
{
public:
 
    virtual void init() override
    {
        Agent::init();

        _visionZone = CellSize(20,20);

        _visionForwardOffset = 15;

        _lifepoints = 50.f;

        _velocity = __PREY_DEFAULT_VELOCITY__;

        m_foodTarget = s_badFoodTarget;

        m_mesh = _gameObject->findComponent<MeshRenderer>();

        m_body = _gameObject->findComponent<PhysicBody>();
    }

    virtual void respawn() override
    {
         Agent::respawn();

         _lifepoints = __PREY_DEFAULT_LP__;

         _velocity = __PREY_DEFAULT_VELOCITY__;

         m_size = 1.0f;

         m_foodTarget = s_badFoodTarget;

         m_currCluster.reset();
    }

    virtual void die() override
    {
        Agent::die();

        if(m_currCluster.get())
            PreyClusterManager::instance()->removeFromCluster(this);
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
            float g = (isHungry() ? 1.f : 0.f);
            float b = (isInGroup() ? 1.f : 0.f);

            glColor3f(0.f,g,b);
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
        updateCluster();

        Agent::onAIUpdate(dt);

        m_hunger -= __HUNGER_RATE__;

        m_hunger = max(0.f, m_hunger);

        if(_lifepoints <= 0)
        {
            die();
        }
        else
        {
            float velCoef = __PREY_MAX_VELOCITY__- __PREY_DEFAULT_VELOCITY__;

            _velocity = min(__PREY_MAX_VELOCITY__,-velCoef*(_lifepoints/__PREY_DEFAULT_LP__)+__PREY_DEFAULT_VELOCITY__);

            m_size = max(0.5f,_lifepoints/__PREY_DEFAULT_LP__);

            m_mesh->setScale(m_size);

            m_body->setBoxSize(btVector3(10,10, 10 * m_size));
        }
    }

    virtual void updateVision() override
    {
        Agent::updateVision();

        ck::Vector2i offset = gridForward() * _visionForwardOffset;

        m_foodSpotSeen = WorldMap::instance()->cellsOfType(WorldMap::MapCell::TREE,_gameObject->position(),_visionZone,ck::Vector2i(0,0));
    }

    #pragma region IClusterObject Interface

    inline const btVector3& position() { return _gameObject->position(); }

    inline void setCluster(const Cluster_ptr& cl_)
    {
        m_currCluster = cl_;
    }

    inline Cluster_ptr cluster() { return m_currCluster; }

    #pragma endregion

    void updateCluster()
    {
        if(!m_currCluster.get())
            checkGroupCreation();
        else
            checkLeaveGroup();
    }

    void checkGroupCreation()
    {
        PreyAgent* agent = findFirstObjectSeen<PreyAgent>(
            [this](PreyAgent* agent) -> bool
            { 
                if(agent == this)
                   return false;

                if(agent->isInGroup())
                   return false;
                
                btVector3 sub = agent->gameObject()->position() - _gameObject->position();
                sub.setZ(0);

                return (sub.length() <= __CLUSTER_RADIUS__);
            });

        if(!agent)
            return;


        PreyClusterManager::instance()->createCluster(this,agent);
    }

    void checkLeaveGroup()
    {
        if(!m_currCluster.get())
            return;

        if(m_currCluster->isIn(this))
            return;

        leaveGroup();
    }

    void leaveGroup()
    {
        if(!m_currCluster.get())
            return;

        PreyClusterManager::instance()->removeFromCluster(this);
    }

    #pragma region Condition

    bool isHungry();

    bool isCloseToFood();

    bool isCloseToFoodSpot(const Cell& food);

    bool seeFood();

    bool isInGroup();

    bool isCloseToGroup();

    bool seeGroup();

    bool seeClosePredator();

    #pragma endregion

    struct FleeAction : public BehaviourAction<PreyAgent>
    {
        virtual void run() override;
    };

    struct EatAction : public BehaviourAction<PreyAgent>
    {
        virtual void run() override;

        virtual void onTerminate() override;
    };

    struct JoinGroupAction : public BehaviourAction<PreyAgent>
    {
        virtual void run() override;
    };

    struct FlockAction : public BehaviourAction<PreyAgent>
    {
        virtual void onStart() override;

        virtual void run() override;
       
        virtual void onTerminate() override;

        btVector3 flockingCohesion();

        btVector3 flockingAlignment();

        btVector3 flockingSeparation();
    };

    struct GotoFoodAction : public BehaviourAction<PreyAgent>
    {
        virtual void onStart() override;

        virtual void run() override;

        virtual void onTerminate() override;

    struct GotoGroupAction : public BehaviourAction<PreyAgent>
    {
        virtual void run() override;
        virtual void onTerminate() override;
    };

    struct WanderAction : public BehaviourAction<PreyAgent>
    {
        virtual void onStart() override;

        virtual void run() override;

        virtual void onTerminate() override;
    };

private:

    static const Cell s_badFoodTarget;

    float m_hunger      = __HUNGER_FULL__; 

    float m_size = 1.0f;

    Cell m_foodTarget;
    Cluster_ptr m_targetCluster;

    std::vector<Cell> m_knownFoodSpot;

    std::vector<Cell> m_foodSpotSeen;

    Cluster_ptr m_currCluster;

    PredatorAgent* m_predatorTarget;

    MeshRenderer* m_mesh;

    PhysicBody* m_body;
};


#endif //_AI_PreyAgent_hpp_