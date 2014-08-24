#ifndef __Agent_hpp__
#define __Agent_hpp__

#include <functional>

#include "Debug.hpp"

#include "GameObject.hpp"
#include "GameComponent.hpp"

#include "WorldMap.hpp"

#include "bt/BehaviourTree.hpp"

#include "ff/FFFlowPath.hpp"
#include "PhysicBody.hpp"

using namespace ai::bt;

#define __DEBUG_VISION__
//#define __DEBUG_ACTION__
#define _AI_FRAME_ 1.f/20.f 

class Agent : public GameComponent
{
public:

    enum MovementType
    {
        STAND_STILL,
        MOVE_LINE_OF_SIGHT,
        MOVE_FOLLOW_FLOW_PATH
    };

    virtual void init() override
    {
        m_body = _gameObject->findComponent<PhysicBody>();

        if(!m_body)
            throw std::exception();
    }

    void goTo(const btVector3& targ_)
    {
        _targetPoint = targ_;

        // should check line of sight in world map
    }

    virtual void onUpdate(float dt) override
    {
        // Check if need to update AI
        clock_t time = clock();

        if(((float)(time - lastUpdate)) / CLOCKS_PER_SEC >= _AI_FRAME_)
        {
            onAIUpdate((time - lastUpdate) / CLOCKS_PER_SEC);

            lastUpdate = clock();
        }

        // Update movement

        moveTowardTarget();
    }

    virtual void onAIUpdate(float dt)
    {
        updateVision();

        m_behaviours.step();
    }

    virtual void updateVision()
    {
        _objectSeen = WorldMap::instance()->registeredObjectAt(_gameObject->position(),_visionZone,ck::Vector2i(0,0));

#ifdef __DEBUG_VISION__
        for(ObjectList* list : _objectSeen)
        {
            for(GameObject* go : *list)
            {
                Debug::drawLine(_gameObject->position(), go->position(), NYColor(1,1,1,1) , 0.1f);
            }
        }
#endif
    }


    void moveTowardTarget()
    {
        btVector3 rel(0,0,0);

        // Move straightly toward target
        if(_currMovement == MovementType::MOVE_LINE_OF_SIGHT)
        {
            WorldMap::applyMapBoundaries(_targetPoint);

            rel = _targetPoint - _gameObject->transform().getOrigin();

            rel.normalize();

            rel = _velocity*rel;
        }

        rel.setZ(-m_mass);

        // Jump

        if(obstacleAhead() && feetHeight() < (groundHeight()+1.5f*NYCube::CUBE_SIZE) )
        {
            rel.setZ(m_jumpForce);
        }

        m_body->body().setLinearVelocity(rel);

        face(_targetPoint);
    }

    bool obstacleAhead()
    {
        btVector3 pos = _gameObject->transform().getOrigin();

        btVector3 forw = forward();

        float angle = roundf(atan2(forw.y(),forw.x()) * 4.f / M_PI) * M_PI_4;

        int x = pos.x() / NYCube::CUBE_SIZE;
        int y = pos.y() / NYCube::CUBE_SIZE;
        int z = WorldMap::cellHeight(x,y);

        x += round(cos(angle));
        y += round(sin(angle));

        NYCubeType cube = WorldMap::instance()->nyworld()->getCube(x,y,z)->_Type;

        return (cube != NYCubeType::CUBE_AIR);
    }

    inline ck::Vector2i gridForward()
    {
        btVector3 forw = forward();

        float angle = roundf(atan2(forw.y(),forw.x()) * 4.f / M_PI) * M_PI_4;

        return ck::Vector2i((int)round(cos(angle)) ,(int)round(sin(angle)));
    }

    Direction gridForwardDirection()
    {
        return direction(gridForward());
    }
    
    inline float groundHeight()
    {
        btVector3 pos = _gameObject->transform().getOrigin();

        int x = pos.x() / NYCube::CUBE_SIZE;
        int y = pos.y() / NYCube::CUBE_SIZE;
        return WorldMap::cellHeight(x,y) * NYCube::CUBE_SIZE;
    }

    inline float feetHeight()
    {
        return _gameObject->transform().getOrigin().z() - m_body->boxSize().z() * 0.5f;
    }

    void face(const btVector3& point_)
    {
        btVector3 rel = point_ - _gameObject->transform().getOrigin();

        btTransform tr;
        m_body->body().getMotionState()->getWorldTransform(tr);
       
        btQuaternion quat(btVector3(0, 0, 1), atan2(rel.y(),rel.x())+ M_PI_2);
        tr.setRotation(quat);

        m_body->body().getMotionState()->setWorldTransform(tr);
    }

    inline btVector3 forward()
    {
        btVector3 forw = _targetPoint - _gameObject->transform().getOrigin();
        forw.setZ(0);
        forw.normalize();
        return forw;
    }

    inline btVector3 right()
    {
        btVector3 right = forward().cross(btVector3(0,0,1));
        return right;
    }

    inline btVector3 up()
    {
        btVector3 up(0,0,1);
        return up;
    }

    inline BehaviourTree& behaviours(){ return m_behaviours; }
    inline void setBehaviours(const BehaviourTree& bt_){ m_behaviours = bt_; }

    /// Find and return the first object seen with component of type _Type
    template<typename _Type>
    _Type* findFirstObjectSeen()
    {
        _Type* comp;

        for(ObjectList* list : _objectSeen)
        {
            for(GameObject* go : *list)
            {
                if((comp = go->findComponent<_Type>()))
                    return comp;
            }
        }

        return nullptr;
    }

    /// Find and return the first object seen 
    /// with component of type _Type that satisfies condition_
    template<typename _Type>
    _Type* findFirstObjectSeen(std::function<bool(_Type*)> condition_)
    {
        _Type* comp;

        for(ObjectList* list : _objectSeen)
        {
            for(GameObject* go : *list)
            {
                if( (comp = go->findComponent<_Type>()) && condition_(comp) )
                    return comp;
            }
        }

        return nullptr;
    }

    /// Find and return the all objects seen with component of type _Type
    template<typename _Type>
    std::vector<_Type*> findAllObjectsSeen()
    {
        _Type* comp;
        std::vector<_Type*> gObjs;

        for(ObjectList* list : _objectSeen)
        {
            for(GameObject* go : *list)
            {
                if( (comp = go->findComponent<_Type>()) )
                    gObjs.push_back(comp);
            }
        }

        return gObjs;
    }

    /// Find and return the all objects seen 
    /// with component of type _Type that satisfies condition_
    template<typename _Type>
    std::vector<_Type*> findAllObjectsSeen(std::function<bool(_Type*)> condition_)
    {
        _Type* comp;
        std::vector<_Type*> gObjs;

        for(ObjectList* list : _objectSeen)
        {
            for(GameObject* go : *list)
            {
                if( (comp = go->findComponent<_Type>()) &&  condition_(comp) )
                    gObjs.push_back(comp);
            }
        }

        return gObjs;
    }

    virtual void hit(float damages)
    {
        _lifepoints = max(0.f, _lifepoints-damages);
    }

protected:

   #pragma region Action Management

    void runAction(IBehaviourAction* action_)
    {
        if(action_ != _currentAction)
        { 
            if(_currentAction)
                _currentAction->onTerminate();

            if(action_)
                action_->onStart();

#ifdef __DEBUG_ACTION__
            std::cout << "Agent: 0x"<<std::hex<<this<<" Start action: " << typeid(*action_).name() << "\n";
#endif //__DEBUG_ACTION__

            _currentAction = action_;
        }
        else
        {
#ifdef __DEBUG_ACTION__
            std::cout << "Agent: 0x"<<std::hex<<this<<" Run action: " << typeid(*action_).name() << "\n";
#endif //__DEBUG_ACTION__
        }
    }

    #pragma endregion

    typedef std::vector<std::list<GameObject*>*> ObjectListArray;
    typedef std::list<GameObject*> ObjectList;

    ObjectListArray _objectSeen;

    MovementType _currMovement;

    btVector3 _targetPoint = btVector3(100,100,300);

    FFPath_ptr _path;

    /// Vision rectangle
    CellSize _visionZone;
    
    /// Offset of vision rectangle 
    int _visionForwardOffset; 

    float _velocity = 50.f;

    float _lifepoints;

    IBehaviourAction* _currentAction = nullptr;

private:

    PhysicBody* m_body;

    BehaviourTree m_behaviours;

    float m_mass = 50.f;

    float m_jumpForce = 20.f;

    clock_t lastUpdate = 0.f;

};

#endif //__Agent_hpp__