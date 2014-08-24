#ifndef __PhysicBody_hpp__
#define __PhysicBody_hpp__

#include "GameComponent.hpp"

#include "LinearMath/btTransform.h"

#include "basic_physic_engine.h"


class  PhysicBody : public GameComponent
{
public:
    
    PhysicBody(const NYVert3Df& size_)
    : m_size(size_)
    {}

    virtual void init() override
    {
        m_body = NYBasicPhysicEngine::getInstance()
            ->addBoxObject(true, m_size, NYVert3Df(200, 200, 200), 10);

        m_body->setDamping(.0f,0.0f);
        m_body->setFriction(0.5f);
        setTransform();
    }

    /// Sets rigid body transform to _gameObject's transform 
    virtual void setTransform();

    virtual void onUpdate(float dt) override;

    inline btRigidBody& body(){ return *m_body; }

    inline btVector3 boxSize(){ return btVector3(m_size.X,m_size.Y,m_size.Z); }

    inline void setBoxSize(const btVector3& box)
    {
        m_size = NYVert3Df(box.x(), box.y(), box.z());
    }

private:

    btRigidBody* m_body;

    float m_mass;

    NYVert3Df m_size;

};


#endif // __PhysicBody_hpp__