
#ifndef _AI_PreyAgent_hpp_
#define _AI_PreyAgent_hpp_

#include "Agent.hpp"
#include "AgentCluster.hpp"
#include "TreeFood.hpp"

#define __HUNGER_FULL__      100.f
#define __HUNGER_THRESHOLD__ 40.f
#define __FOOD_POWER__       20.f
#define __HUNGER_RATE__      0.05f
#define __HUNGER_KILL_RATE__ 0.1f
#define __CLUSTER_RADIUS__   100.f
#define __WANDER_RADIUS__   200.f


typedef std::shared_ptr<AgentCluster> AgentCluster_ptr;

class PreyAgent : public Agent
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

        m_currCluster = std::make_shared<AgentCluster>(this,agent,__CLUSTER_RADIUS__);
        agent->m_currCluster = m_currCluster;
    }

    void checkLeaveGroup()
    {
        if(!m_currCluster.get())
            return;

        if(m_currCluster->isIn(this))
            return;

        m_currCluster->unregisterAgent(this);

        // Remove the last agent is alone
        if(m_currCluster->agents().size() == 1)
        {
            PreyAgent* agent = static_cast<PreyAgent*>(m_currCluster->agents()[0]);
            m_currCluster->unregisterAgent(agent);
            agent->m_currCluster.reset();
        }
 
        m_currCluster.reset();
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
                    holder->m_targetCluster->registerAgent(holder);
                    holder->m_currCluster = holder->m_targetCluster;
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

            btVector3 sub = holder->_targetPoint - holder->_gameObject->position();

            sub.setZ(0);

            if(sub.length() <= 2*NYCube::CUBE_SIZE)
            { 
                holder->_targetPoint = WorldMap::toWorldCoord(Cell(rand()%c_worldSize,rand()%c_worldSize));
            }
        }

        virtual void onTerminate() override
        {
            holder->_currMovement = MovementType::STAND_STILL;
        }
    };

    struct GotoFoodAction : public BehaviourAction<PreyAgent>
    {
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
    AgentCluster_ptr m_targetCluster;

    std::vector<Cell> m_knownFoodSpot;

    std::vector<Cell> m_foodSpotSeen;

    AgentCluster_ptr m_currCluster;

};

const Cell PreyAgent::s_badFoodTarget =  Cell(-1,-1);

#endif //_AI_PreyAgent_hpp_