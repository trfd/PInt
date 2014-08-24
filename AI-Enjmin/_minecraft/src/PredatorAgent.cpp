
#include "ai/PredatorAgent.hpp"

#include "ai/PreyAgent.hpp"

#pragma region Condition

bool PredatorAgent::isCloseToPrey()
{
    if(m_preyTarget && isCloseTo(m_preyTarget))
        return true;

    PreyAgent* prey = findFirstObjectSeen<PreyAgent>(
        [this](PreyAgent* ag) -> bool
        {
            return isCloseTo(ag);
        });

    if(!prey)
        return false;

    m_preyTarget = prey;

    return true;
}

bool PredatorAgent::isCloseTo(PreyAgent* ag)
{
    return(ag->gameObject()->position().distance(_gameObject->position()) < 1.5*NYCube::CUBE_SIZE);
}

bool PredatorAgent::targetPreyIsAlone()
{
    return (m_preyTarget->cluster().get() == NULL);
}

bool PredatorAgent::seePrey()
{
    std::vector<PreyAgent*> preys = findAllObjectsSeen<PreyAgent>();

    if(preys.size() == 0)
        return false;

    // Select the closest prey

    float minDist = FLT_MAX;
    float tmp;
    for(PreyAgent* ag : preys)
    {
        if((tmp = ag->position().distance(_gameObject->position())) < minDist)
        {
            minDist = tmp;
            m_preyTarget = ag;
        }
    }

    return true;
}

bool PredatorAgent::isHidden()
{
    return (WorldMap::cellAt(_gameObject->position().x(),
                          _gameObject->position().y()) == 
           WorldMap::MapCell::TALL_GRASS);
}

#pragma endregion

#pragma region Actions

#pragma region AttackPreyAction

void  PredatorAgent::AttackPreyAction::run()
{
    holder->runAction(this);   

    if(!holder->m_preyTarget || !holder->isCloseTo(holder->m_preyTarget))
    {
        std::cout << "Predator has no prey close\n";
        return;
    }

    holder->m_preyTarget->hit(__PREDATOR_DAMAGES__);

    holder->m_hunger += __PREDATOR_FOOD_POWER__;

    holder->m_hunger = min(holder->m_hunger ,__PREDATOR_HUNGER_FULL__);

    holder->_lifepoints += __PREDATOR_ATTACK_HEAL__;
}

#pragma endregion

#pragma region FollowPreyGroupAction

void  PredatorAgent::FollowPreyGroupAction::run()
{
    holder->runAction(this);

    holder->_currMovement = MovementType::MOVE_LINE_OF_SIGHT;

    if(!holder->m_preyTarget->cluster().get())
        return;

    btVector3 rel = holder->gameObject()->position() - holder->m_preyTarget->cluster()->center();

    rel.normalize();

    holder->_targetPoint = __PREY_PREDATOR_CLOSE__ * rel +  holder->m_preyTarget->cluster()->center();
}

#pragma endregion


#pragma region GotoPreyAction

void PredatorAgent::GotoPreyAction::run()
{
    holder->runAction(this);
    
    holder->_currMovement = MovementType::MOVE_LINE_OF_SIGHT;
        
    holder->_targetPoint = holder->m_preyTarget->gameObject()->position();
}

#pragma endregion


#pragma region WanderAction


void PredatorAgent::WanderAction::onStart()
{
    //holder->_path = FFPathFinder::instance()->path(WorldMap::toGridCoord(holder->_gameObject->position()),
    //                                         Cell(rand()%c_worldSize,rand()%c_worldSize));
}


void  PredatorAgent::WanderAction::run()
{
    holder->runAction(this);

    holder->_currMovement = MovementType::MOVE_LINE_OF_SIGHT;
    
    btVector3 sub = holder->_targetPoint - holder->_gameObject->position();
    
    sub.setZ(0);
    
    if(sub.length() <= 2*NYCube::CUBE_SIZE)
    { 
        float angle = avoidGroups();
        btVector3 vec = holder->_gameObject->position() + 
            btVector3(__WANDER_RADIUS__ * cos(angle), __WANDER_RADIUS__ * sin(angle) , 0.f);
        WorldMap::applyMapBoundaries(vec);
        holder->_targetPoint = vec;
    }
}

float PredatorAgent::WanderAction::avoidGroups()
{
    std::vector<PreyAgent*> agents = holder->findAllObjectsSeen<PreyAgent>();

    std::vector<Cluster_ptr> clusters;

    // Find all clusters
    for(PreyAgent* ag : agents)
    {
        if(!ag->cluster().get())
            continue;

        auto it = std::find(clusters.begin(), clusters.end(), ag->cluster());

        // distance to cluster center
        float dist = ag->cluster()->center().distance(holder->gameObject()->position());
        
        if(it == clusters.end() && dist < __PREY_PREDATOR_CLOSE__ * 1.5)
            clusters.push_back(ag->cluster());
    }

    // If no group around return random
    if(clusters.size() == 0)
    {
        return ((float)rand())/RAND_MAX * M_PI * 2.f;
    }

    btVector3 sum(0,0,0);

    for(Cluster_ptr cl : clusters)
    {
        sum += cl->center();
    }

    sum *= 1.f / clusters.size();

    btVector3 rel = holder->gameObject()->position() - sum;

    return atan2(rel.y(),rel.x());
}

#pragma endregion


#pragma endregion