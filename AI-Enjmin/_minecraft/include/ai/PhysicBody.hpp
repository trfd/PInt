#ifndef __PhysicBody_hpp__
#define __PhysicBody_hpp__

#include "GameComponent.hpp"

#include "LinearMath/btTransform.h"

#include "basic_physic_engine.h"

class  PhysicBody : public GameComponent
{
public:

    virtual void init() override
    {
        
        m_body = NYBasicPhysicEngine::getInstance()
            ->addBoxObject(true, NYVert3Df(10, 10, 10), NYVert3Df(200, 200, 200), 10);

        m_body->setDamping(.0f,0.0f);
        m_body->setFriction(0.5f);
        setTransform();
        
    }

    /// Sets rigid body transform to _gameObject's transform 
    virtual void setTransform();

    virtual void onUpdate(float dt) override;

    btRigidBody& body(){ return *m_body; }

private:

    btRigidBody* m_body;

    
    //btVector3 m_force;
    //btVector3 m_acceleration;
    //btVector3 m_velocity;

    float m_mass;

};


#endif // __PhysicBody_hpp__