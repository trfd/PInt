#ifndef __Agent_hpp__
#define __Agent_hpp__

#include "Debug.hpp"

#include "GameComponent.hpp"

#include "WorldMap.hpp"

#include "bt/BehaviourTree.hpp"

#include "ff/FFFlowPath.hpp"

using namespace ai::bt;

class Agent : public GameComponent
{
public:

    virtual void init() override
    {
        m_body = _gameObject->findComponent<PhysicBody>();

        if(!m_body)
            throw std::exception();
    }

    void goTo(const btVector3& targ_)
    {
        _targetPoint = targ_;
    }

    virtual void onUpdate(float dt) override
    {
        updateVision();

        m_behaviours.step();
    }

    virtual void updateVision()
    {
        ck::Vector2i offset = gridForward() * _visionForwardOffset;
        
        _objectSeen = WorldMap::instance()->registeredObjectAt(_gameObject->position(),_visionZone,offset);
    }


    void moveTowardTarget()
    {
        btVector3 rel = _targetPoint - _gameObject->transform().getOrigin();

        rel.normalize();

        rel = _velocity*rel;

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
        
        Debug::drawLine(pos,btVector3(x+0.5f,y+0.5f,z+1.f) * NYCube::CUBE_SIZE);

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


protected:

    std::list<GameObject*> _objectSeen;

    btVector3 _targetPoint;

    FFPath_ptr _path;

    /// Vision rectangle
    CellSize _visionZone;
    
    /// Offset of vision rectangle 
    int _visionForwardOffset; 

    
    float _velocity = 50.f;

private:

    PhysicBody* m_body;

    BehaviourTree m_behaviours;

    float m_mass = 50.f;

    float m_jumpForce = 20.f;

};

#endif //__Agent_hpp__