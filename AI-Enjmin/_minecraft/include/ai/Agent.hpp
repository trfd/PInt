#ifndef __Agent_hpp__
#define __Agent_hpp__

#include "Debug.hpp"

#include "GameComponent.hpp"

class Agent : public GameComponent
{
    virtual void init() override
    {
        m_body = _gameObject->findComponent<PhysicBody>();

        if(!m_body)
            throw std::exception();
    }

    void goTo(btVector3 const& targ_)
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

        btVector3 rel = m_targetPoint - _gameObject->transform().getOrigin();

        rel.setZ(0);

        rel.normalize();

        rel = 10*rel;

        rel.setZ(m_body->body().getLinearVelocity().z());

        m_body->body().setLinearVelocity(rel);

    }

private:

    float timer = 0; // Test

    btVector3 m_targetPoint;

    PhysicBody* m_body;
};

#endif //__Agent_hpp__