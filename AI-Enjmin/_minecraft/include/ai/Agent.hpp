#ifndef __Agent_hpp__
#define __Agent_hpp__

#include "Debug.hpp"

#include "GameComponent.hpp"

#include "WorldMap.hpp"

class Agent : public GameComponent
{
    virtual void init() override
    {
        m_body = _gameObject->findComponent<PhysicBody>();

        if(!m_body)
            throw std::exception();
    }

    void goTo(const btVector3& targ_)
    {
        Log::log(Log::ENGINE_INFO, ("Goto: "+Debug::toString(targ_)).c_str());
        Debug::drawLine(_gameObject->transform().getOrigin(), targ_,NYColor(1.f,0.f,1.f,1.f),2.f);
        m_targetPoint = targ_;

    }

    virtual void onUpdate(float dt) override
    {
        if(timer <= 0 )
        { 
            goTo(btVector3(rand() % 500, rand() % 500, rand()%500));
            timer = 10.f;
        }

        timer -= dt;

        moveTowardTarget();

    }

    void moveTowardTarget()
    {
        btVector3 rel = m_targetPoint - _gameObject->transform().getOrigin();

        rel.normalize();

        rel = m_velocity*rel;

        rel.setZ(-m_mass);

        // Jump

        if(obstacleAhead() && feetHeight() < (groundHeight()+1.5f*NYCube::CUBE_SIZE) )
        {
            rel.setZ(m_jumpForce);
        }

        m_body->body().setLinearVelocity(rel);

        face(m_targetPoint);
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
        
        Debug::drawLine(pos,btVector3(x+0.5f,y+0.5f,z+0.5f) * NYCube::CUBE_SIZE);

        NYCubeType cube = WorldMap::instance()->nyworld()->getCube(x,y,z)->_Type;

        return (cube != NYCubeType::CUBE_AIR);
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
        btVector3 forw = m_targetPoint - _gameObject->transform().getOrigin();
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

private:

    float timer = 0; // Test

    btVector3 m_targetPoint;

    PhysicBody* m_body;

    float m_velocity = 50.f;

    float m_mass = 50.f;

    float m_jumpForce = 20.f;
};

#endif //__Agent_hpp__