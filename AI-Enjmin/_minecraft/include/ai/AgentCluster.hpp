#ifndef _AgentCluster_hpp_
#define _AgentCluster_hpp_

#include <vector>

#include "LinearMath/btTransform.h"

#include "Agent.hpp"

#define __DRAW_CLUSTERS__

/// Clusters allow easy identification of agent groups
/// with complexity O(N).
/// Identification using only visual data would be of high order.
class AgentCluster
{
public:
    
    AgentCluster(Agent* ag1, Agent* ag2, float rad)
    : m_radius(rad)
    {
        m_agents.push_back(ag1);
        m_agents.push_back(ag2);

        m_center = 0.5f * ag1->gameObject()->position() +
                   0.5f * ag2->gameObject()->position();

        m_dirtyCenter = false;

#ifdef __DRAW_CLUSTERS__

        Debug::addToRender(ck::makeFunctor(this,&AgentCluster::draw));

#endif //__DRAW_CLUSTERS__

    }

    ~AgentCluster()
    {
        ck_assert(m_agents.size() == 0);
    }

    void draw()
    {
        glPushMatrix();
        glTranslatef(m_center.x(), m_center.y(), m_center.z());
        glBegin(GL_LINES);

        glColor3f(1.f,0.f,1.f);

        int n = 10;
        float angle = 2 * M_PI / n;
        for(int i = 0 ; i < 10 ; i++)
        {
            glVertex3f(m_radius * cos(angle*i) , m_radius * sin(angle*i) , 0);
            glVertex3f(m_radius * cos(angle*(i+1)) , m_radius * sin(angle*(i+1)) , 0);
        }

        glEnd();
        glPopMatrix();

    }

    inline btVector3 center()
    {
        if(m_dirtyCenter)
            update();

        return m_center; 
    }

    inline float radius() { return m_radius; }

    inline void setRadius(float rad_) 
    {
        m_radius = rad_; 
        m_dirtyCenter = true; 
    }
   
    void registerAgent(Agent* agent)
    {
        ck_assert(agent);

        // Check if already in
        std::vector<Agent*>::iterator it = std::find(m_agents.begin(), m_agents.end() , agent);

        if(it != m_agents.end())
            return;

        m_agents.push_back(agent);

        m_dirtyCenter = true;
    }

    void unregisterAgent(Agent* agent)
    {
        ck_assert(agent);

        std::vector<Agent*>::iterator it = 
            std::find(m_agents.begin(), m_agents.end() , agent);

        if(it == m_agents.end())
            return;

        m_agents.erase(it);

        m_dirtyCenter = true;
    }

    inline bool isIn(Agent* agent)
    {
        return (agent->gameObject()->position().distance(m_center) <= m_radius);
    }

    const std::vector<Agent*>& agents()
    {
        return m_agents;
    }

    void update()
    {
        if(!m_dirtyCenter)
            return;

        btVector3 newCenter(0.f,0.f,0.f);

        int count = 0;

        for(Agent* agent : m_agents)
        {
            float weight = 1;

            float dist = agent->gameObject()->position().distance(m_center);

            if(dist > m_radius)
                weight = max(0 , - dist / m_radius + 2);

            if(weight == 0.f)
                continue;
                
            newCenter += weight *  agent->gameObject()->position();
            count++;
        }

        if(count < 2)
        {
            return;
        }

        float av = 1.f / count;
        
        m_center = newCenter * av; 

        m_dirtyCenter = false;
    }
    
private:

    bool m_dirtyCenter = true;

    btVector3 m_center;

    float m_radius;

    std::vector<Agent*> m_agents;
};

#endif //_AgentCluster_hpp_