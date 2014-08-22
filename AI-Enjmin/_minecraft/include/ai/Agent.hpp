#ifndef __Agent_hpp__
#define __Agent_hpp__

#include "Debug.hpp"

#include "GameComponent.hpp"

//#include "WorldMap.hpp"

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

        if(obstacleAhead())
        {
            rel.setZ(rel.z() + m_jumpForce);
        }

        m_body->body().setLinearVelocity(rel);

        face(m_targetPoint);
    }

    bool obstacleAhead()
    {
        btVector3 aheadLook(0,5,-3);

        btVector3 worldPos = _gameObject->transform().getOrigin() + aheadLook;
        return false;
      /*  NYCubeType cube = WorldMap::instance()->nyworld()->getCube( worldPos.x() / MAT_SIZE_CUBES,
                                                            worldPos.y() / MAT_SIZE_CUBES,
                                                            worldPos.z() / MAT_SIZE_CUBES)->_Type;

        return (cube != NYCubeType::CUBE_AIR || cube != NYCubeType::CUBE_EAU);*/
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
        btVector3 forw(1,0,0);
        btQuaternion q = _gameObject->transform().getRotation();
        forw.rotate(q.getAxis(),q.getAngle());
        return forw;
    }

private:

    float timer = 0; // Test

    btVector3 m_targetPoint;

    PhysicBody* m_body;

    float m_velocity = 50.f;

    float m_mass = 50.f;

    float m_jumpForce = 70.f;
};

#endif //__Agent_hpp__