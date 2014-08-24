
#include "ai/PreyAgent.hpp"
#include "ai/PredatorAgent.hpp"


const Cell PreyAgent::s_badFoodTarget =  Cell(-1,-1);


void PreyAgent::init() 
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

void PreyAgent::respawn() 
{
     Agent::respawn();

     _lifepoints = __PREY_DEFAULT_LP__;

     _velocity = __PREY_DEFAULT_VELOCITY__;

     m_size = 1.0f;

     m_foodTarget = s_badFoodTarget;

     m_currCluster.reset();
}

void PreyAgent::die() 
{
    Agent::die();

    if(m_currCluster.get())
        PreyClusterManager::instance()->removeFromCluster(this);
}

void PreyAgent::onRender() 
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


void PreyAgent::onAIUpdate(float dt) 
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

void PreyAgent::updateVision() 
{
    Agent::updateVision();

    ck::Vector2i offset = gridForward() * _visionForwardOffset;

    m_foodSpotSeen = WorldMap::instance()->cellsOfType(WorldMap::MapCell::TREE,_gameObject->position(),_visionZone,ck::Vector2i(0,0));
}

void PreyAgent::updateCluster()
{
    if(!m_currCluster.get())
        checkGroupCreation();
    else
        checkLeaveGroup();
}

void PreyAgent::checkGroupCreation()
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

void PreyAgent::checkLeaveGroup()
{
    if(!m_currCluster.get())
        return;

    if(m_currCluster->isIn(this))
        return;

    leaveGroup();
}

void PreyAgent::leaveGroup()
{
    if(!m_currCluster.get())
        return;

    PreyClusterManager::instance()->removeFromCluster(this);
}


#pragma region Condition

bool PreyAgent::isHungry()
{
   return (m_hunger <= __HUNGER_THRESHOLD__);
}

bool PreyAgent::isCloseToFood()
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

bool PreyAgent::isCloseToFoodSpot(const Cell& food)
{
    return (abs(Cell::distance2(food, WorldMap::toGridCoord(_gameObject->position()))) <= 2);
}

bool PreyAgent::seeFood()
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

bool PreyAgent::isInGroup()
{
    if(!m_currCluster.get())
        return false;

    return m_currCluster->isIn(this);
}

bool PreyAgent::isCloseToGroup()
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

bool PreyAgent::seeGroup()
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

bool PreyAgent::seeClosePredator()
{
    m_predatorTarget = findFirstObjectSeen<PredatorAgent>(
        [this](PredatorAgent* ag) -> bool
        {
            return(_gameObject->position().distance(ag->gameObject()->position()) < __PREY_PREDATOR_CLOSE__);
        });
    return (m_predatorTarget);
}

#pragma endregion


#pragma region Actions

#pragma region FleeAction

void PreyAgent::FleeAction::run()
{
    holder->runAction(this);

    if(!holder->m_predatorTarget)
    {
        std::cout << "Prey has no predator close\n";
        return;
    }

    holder->_currMovement = MovementType::MOVE_STEERING;
    holder->_targetDirection = holder->_gameObject->position() - holder->m_predatorTarget->gameObject()->position();
}

#pragma endregion

#pragma region EatAction

void PreyAgent::EatAction::run()
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

void PreyAgent::EatAction::onTerminate()
{
    holder->m_foodTarget = s_badFoodTarget;
}

#pragma endregion

#pragma region JoinGroupAction

void PreyAgent::JoinGroupAction::run()
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

#pragma endregion

#pragma region FlockAction

void PreyAgent::FlockAction::onStart()
{
    holder->_targetPoint = WorldMap::toWorldCoord(Cell(rand()%c_worldSize,rand()%c_worldSize));
}

void PreyAgent::FlockAction::run()
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
            btVector3 vec = holder->_gameObject->position() + 
                btVector3(__WANDER_RADIUS__ * cos(angle), __WANDER_RADIUS__ * sin(angle) , 0.f);
            WorldMap::applyMapBoundaries(vec);
            holder->_targetPoint = vec;
        }
    }
   
}

void PreyAgent::FlockAction::onTerminate()
{
    holder->_currMovement = MovementType::STAND_STILL;
}

btVector3 PreyAgent::FlockAction::flockingCohesion()
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

btVector3 PreyAgent::FlockAction::flockingAlignment()
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

btVector3 PreyAgent::FlockAction::flockingSeparation()
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

#pragma endregion

#pragma region GotoGroupAction

void PreyAgent::GotoFoodAction::onStart()
{
    holder->leaveGroup();
}


void PreyAgent::GotoFoodAction::run() 
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

void PreyAgent::GotoFoodAction::onTerminate() 
{
    holder->_currMovement = MovementType::STAND_STILL;
}

#pragma endregion

#pragma region GotoGroupAction

void PreyAgent::GotoGroupAction::run()
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

void PreyAgent::GotoGroupAction::onTerminate()
{
    holder->_currMovement = MovementType::STAND_STILL;
}

#pragma endregion

#pragma region WanderAction

void PreyAgent::WanderAction::onStart()
{
    newTarget();
}

void PreyAgent::WanderAction::run()
{
    holder->runAction(this);

    holder->_currMovement = MovementType::MOVE_STEERING;

    btVector3 sub = holder->_targetPoint - holder->_gameObject->position();

    Cell c = WorldMap::toGridCoord(holder->_gameObject->position());

    sub.setZ(0);

    if(sub.length() <= 2*NYCube::CUBE_SIZE || WorldMap::onEdge(c))
    { 
        newTarget();
    }
}

void PreyAgent::WanderAction::onTerminate()
{
    holder->_currMovement = MovementType::STAND_STILL;
}

void PreyAgent::WanderAction::newTarget()
{
    float angle = ((float)rand())/RAND_MAX * 2 * M_PI;
    holder->_targetDirection = btVector3(__WANDER_RADIUS__ * cos(angle), __WANDER_RADIUS__ * sin(angle) , 0.f);
    btVector3 vec = holder->_gameObject->position() + holder->_targetDirection;
    WorldMap::applyMapBoundaries(vec);
    holder->_targetPoint = vec;
}

#pragma endregion

#pragma endregion