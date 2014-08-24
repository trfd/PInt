
#ifndef _AI_PreyAgent_hpp_
#define _AI_PreyAgent_hpp_


#include "Agent.hpp"

#include "Cluster.hpp"
#include "ClusterManager.hpp"

#define __HUNGER_FULL__      100.f
#define __HUNGER_THRESHOLD__ 40.f
#define __FOOD_POWER__       20.f
#define __HUNGER_RATE__      0.05f
#define __HUNGER_KILL_RATE__ 0.1f
#define __WANDER_RADIUS__   200.f

#define __FLOCK_COHESION_COEF__     1.0f
#define __FLOCK_ALIGNMENT_COEF__    1.0f
#define __FLOCK_SEPARATION_COEF__   1.5f
#define __FLOCK_SEPARATION_RADIUS__ 10.f

#define __PREY_PREDATOR_CLOSE__     7.f



class PreyAgent : public Agent, public IClusterObject
{
public:
 
    virtual void init() override
    {
        Agent::init();

        _visionZone = CellSize(20,20);

        _visionForwardOffset = 15;

        m_foodTarget = s_badFoodTarget;
    }

    virtual void onRender() override
    {
        btVector3 pos = _gameObject->transform().getOrigin() + btVector3(0,0,15);

        float a = 5.f;

        glPushMatrix();
        glTranslatef(pos.x(),pos.y(),pos.z());
        glBegin(GL_QUADS);

        float g = (isHungry() ? 1.f : 0.f);
        float b = (isInGroup() ? 1.f : 0.f);

        glColor3f(0.f,g,b);

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

        if(m_hunger == 0.f)
        {
            m_lifepoints -= __HUNGER_KILL_RATE__;

            if(m_lifepoints <= 0)
            {
               // std::cout << "Prey is died \n";
            }
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

    inline bool isHungry()
    {
       return (m_hunger <= __HUNGER_THRESHOLD__);
    }

    inline bool isCloseToFood()
    {
        Cell food = Cell(-1,-1);

        for(Cell& c : m_foodSpotSeen)
        {
            if(isCloseToFoodSpot(c))
                food = c;       
        }

        if(food.x < 0 || food.y < 0 ) 
            return false;

        m_foodTarget = food;

        return true;
    }

    inline bool isCloseToFoodSpot(const Cell& food)
    {
        return (abs(Cell::distance2(food, WorldMap::toGridCoord(_gameObject->position()))) <= 2);
    }

    inline bool seeFood()
    {
         if(m_foodTarget != s_badFoodTarget)
            return true;

         if(m_foodSpotSeen.size() == 0)
            return false;

         // Select the closest tree around

         int minDist = INT_MAX;
         int tmpDist;
         for(Cell& fd : m_foodSpotSeen)
         {
             if((tmpDist = abs(Cell::distance2(fd, WorldMap::toGridCoord(_gameObject->position())))) < minDist)
             {
                minDist = tmpDist;
                m_foodTarget = fd;
             }
         }

         return true;
    }

    inline bool isInGroup()
    {
        if(!m_currCluster.get())
            return false;

        return m_currCluster->isIn(this);
    }

    inline bool isCloseToGroup()
    {
        PreyAgent* agent = findFirstObjectSeen<PreyAgent>(
            [this](PreyAgent* agent) -> bool
            { 
                if(agent == this)
                    return false;

                if(!agent->isInGroup())
                   return false;
                return agent->m_currCluster->isIn(this);
            });

        if(!agent)
            return false;

        m_targetCluster = agent->m_currCluster;

        return (agent != NULL);
    }

    inline bool seeGroup()
    {
        PreyAgent* agent = findFirstObjectSeen<PreyAgent>(
            [this](PreyAgent* agent) -> bool
            { 
                 if(agent == this)
                    return false;

                return agent->isInGroup();
            });

        if(!agent)
            return false;

        m_targetCluster = agent->m_currCluster;
        
        return true;
    }

    #pragma endregion

    struct EatAction : public BehaviourAction<PreyAgent>
    {
        virtual void run() override
        {
            holder->runAction(this);

            if(holder->m_foodTarget == s_badFoodTarget || 
               !holder->isCloseToFoodSpot(holder->m_foodTarget))
            {
                std::cout << "Prey has no food tree close\n";
                return;
            }

            holder->m_hunger += __FOOD_POWER__;

            holder->m_hunger = min(holder->m_hunger ,__HUNGER_FULL__);
        }

        virtual void onTerminate() override
        {
            holder->m_foodTarget = s_badFoodTarget;
        }
    };

    struct JoinGroupAction : public BehaviourAction<PreyAgent>
    {
        virtual void run() override
        {
            holder->runAction(this);

            if(holder->m_targetCluster.get())
            {
                if(holder->m_targetCluster->isIn(holder))
                {   
                    PreyClusterManager::instance()->addToCluster(holder,holder->m_targetCluster);

                    holder->m_targetCluster.reset();

                }
            }
        }
    };

    struct FlockAction : public BehaviourAction<PreyAgent>
    {
        virtual void onStart() override
        {
            holder->_targetPoint = WorldMap::toWorldCoord(Cell(rand()%c_worldSize,rand()%c_worldSize));
        }

        virtual void run() override
        {
            holder->runAction(this);

            holder->_currMovement = MovementType::MOVE_LINE_OF_SIGHT;
            
            // If not leader of flock
            if(holder != holder->m_currCluster->objects()[0])
            { 
                btVector3 steer = __FLOCK_ALIGNMENT_COEF__  *  flockingAlignment()  + 
                                  __FLOCK_COHESION_COEF__   *  flockingCohesion()   +
                                  __FLOCK_SEPARATION_COEF__ *  flockingSeparation();

                holder->_targetPoint =  holder->_gameObject->position() + steer;
            }
            else // If leader: wander
            {
                btVector3 sub = holder->_targetPoint - holder->_gameObject->position();

                sub.setZ(0);

                if(sub.length() <= 2*NYCube::CUBE_SIZE)
                { 
                    float angle = ((float)rand())/RAND_MAX * 2 * M_PI;
                    btVector3 vec = holder->_gameObject->position() + btVector3(__WANDER_RADIUS__ * cos(angle), __WANDER_RADIUS__ * sin(angle) , 0.f);
                    WorldMap::applyMapBoundaries(vec);
                    holder->_targetPoint = vec;
                }
            }
           
        }

        virtual void onTerminate() override
        {
            holder->_currMovement = MovementType::STAND_STILL;
        }

        btVector3 flockingCohesion()
        {
            ck_assert(holder->m_currCluster.get());

            btVector3 sum(0,0,0);

            for(IClusterObject* obj : holder->m_currCluster->objects())
            {
                Agent* agent = static_cast<PreyAgent*>(obj);
                sum += agent->gameObject()->position();
            }
            
            sum = sum * (1.f / holder->m_currCluster->objects().size());

            return (sum - holder->gameObject()->position());
        }

        btVector3 flockingAlignment()
        {
            ck_assert(holder->m_currCluster.get());

            btVector3 sum(0,0,0);

            for(IClusterObject* obj : holder->m_currCluster->objects())
            {
                Agent* agent = static_cast<PreyAgent*>(obj);
                 sum += agent->forward();
            }

            return sum * (1.f / holder->m_currCluster->objects().size());
        }

        btVector3 flockingSeparation()
        {
            ck_assert(holder->m_currCluster.get());

            btVector3 sum(0,0,0);

            int count = 0;

            for(IClusterObject* obj : holder->m_currCluster->objects())
            {
                Agent* agent = static_cast<PreyAgent*>(obj);
                if(agent->gameObject()->position().distance(holder->gameObject()->position()) < __FLOCK_SEPARATION_RADIUS__)
                {
                    sum += agent->gameObject()->position();
                    count++;
                }
            }

            sum = sum * (1.f /count);

            return (holder->gameObject()->position() - sum);
        }
    };

    struct GotoFoodAction : public BehaviourAction<PreyAgent>
    {
        virtual void onStart() override
        {
            holder->leaveGroup();
        }
        

        virtual void run() override
        {
            holder->runAction(this);

            if(holder->m_foodTarget == s_badFoodTarget)
            {
                std::cout << "Prey has no food target\n";
                return;
            }

            holder->_currMovement = MovementType::MOVE_LINE_OF_SIGHT;
            holder->_targetPoint = WorldMap::toWorldCoord(holder->m_foodTarget);
        }

        virtual void onTerminate() override
        {
            holder->_currMovement = MovementType::STAND_STILL;
        }
    };

    struct GotoGroupAction : public BehaviourAction<PreyAgent>
    {
        virtual void run() override
        {
            holder->runAction(this);

            if(!holder->m_targetCluster.get())
            {
                std::cout << "Prey has no cluster target\n";
                return;
            }

            holder->_currMovement = MovementType::MOVE_LINE_OF_SIGHT;
            holder->_targetPoint = holder->m_targetCluster->center();
        }

        virtual void onTerminate() override
        {
            holder->_currMovement = MovementType::STAND_STILL;
        }
    };

    struct WanderAction : public BehaviourAction<PreyAgent>
    {
        virtual void onStart() override
        {
            holder->_targetPoint = WorldMap::toWorldCoord(Cell(rand()%c_worldSize,rand()%c_worldSize));
        }

        virtual void run() override
        {
            holder->runAction(this);

            holder->_currMovement = MovementType::MOVE_LINE_OF_SIGHT;

            btVector3 sub = holder->_targetPoint - holder->_gameObject->position();

            sub.setZ(0);

            if(sub.length() <= 2*NYCube::CUBE_SIZE)
            { 
                float angle = ((float)rand())/RAND_MAX * 2 * M_PI;
                btVector3 vec = holder->_gameObject->position() + btVector3(__WANDER_RADIUS__ * cos(angle), __WANDER_RADIUS__ * sin(angle) , 0.f);
                WorldMap::applyMapBoundaries(vec);
                holder->_targetPoint = vec;
            }
        }

        virtual void onTerminate() override
        {
            holder->_currMovement = MovementType::STAND_STILL;
        }
    };

private:

    static const Cell s_badFoodTarget;

    float m_hunger      = __HUNGER_FULL__; 
    float m_lifepoints  = 10.f; 

    Cell m_foodTarget;
    Cluster_ptr m_targetCluster;

    std::vector<Cell> m_knownFoodSpot;

    std::vector<Cell> m_foodSpotSeen;

    Cluster_ptr m_currCluster;

};


#endif //_AI_PreyAgent_hpp_