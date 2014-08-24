
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
 
    virtual void init() override;

    virtual void respawn() override;

    virtual void die() override;

    virtual void onRender() override;
    

    virtual void onAIUpdate(float dt) override;

    virtual void updateVision() override;

    #pragma region IClusterObject Interface

    inline const btVector3& position() { return _gameObject->position(); }

    inline void setCluster(const Cluster_ptr& cl_)
    {
        m_currCluster = cl_;
    }

    inline Cluster_ptr cluster() { return m_currCluster; }

    #pragma endregion

    void updateCluster();
    void checkGroupCreation();

    void checkLeaveGroup();

    void leaveGroup();

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
    };

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

        void newTarget();
    };

private:

    static const Cell s_badFoodTarget;

    float m_hunger = __HUNGER_FULL__; 

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