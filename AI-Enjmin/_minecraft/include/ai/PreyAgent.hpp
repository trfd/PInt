
#ifndef _AI_PreyAgent_hpp_
#define _AI_PreyAgent_hpp_

#include "Agent.hpp"

class PreyAgent : public Agent
{
public:

    virtual void onUpdate(float dt) override
    {

    }

private:

    float m_hunger;
    float m_lifepoints;

};

#endif //_AI_PreyAgent_hpp_