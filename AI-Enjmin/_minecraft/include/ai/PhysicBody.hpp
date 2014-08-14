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
            ->addBoxObject(true, NYVert3Df(1, 1, 1), NYVert3Df(200, 200, 200), 100);

        m_body->setDamping(.3f,0.1f);
        setTransform();
    }

    /// Sets rigid body transform to _gameObject's transform 
    virtual void setTransform();

    virtual void onUpdate(float dt) override;

    btRigidBody& body(){ return *m_body; }

private:

    btRigidBody* m_body;

};


#endif // __PhysicBody_hpp__