#ifndef _Cluster_hpp_
#define _Cluster_hpp_

#include <memory>
#include <vector>

#include "LinearMath/btTransform.h"
#include "GameManager.hpp"
#include "CKAssert.hpp"
#include "Debug.hpp"

#define __DRAW_CLUSTERS__

class Cluster;

typedef std::shared_ptr<Cluster> Cluster_ptr;

class IClusterObject
{
public:
    virtual const btVector3& position()        = 0;
    virtual void setCluster(const Cluster_ptr&) = 0;
    virtual Cluster_ptr cluster()        = 0;
};

/// Clusters allow easy identification of object groups
/// with complexity O(N).
/// Identification using only visual data would be of high order.
class Cluster : public std::enable_shared_from_this<Cluster>
{
public:
    
    Cluster(float rad = 1.f)
    : m_radius(rad)
    {
        init();
    }

    void init()
    {
        m_dirtyCenter = false;

        GameManager::instance()->scheduleUpdate(0.05f,this,&Cluster::update);

#ifdef __DRAW_CLUSTERS__

        Debug::addToRender(ck::makeFunctor(this,&Cluster::draw));

#endif //__DRAW_CLUSTERS__
    }

    ~Cluster()
    {
#ifdef __DRAW_CLUSTERS__
        Debug::removeFromRender(ck::makeFunctor(this,&Cluster::draw));
#endif
        ck_assert(m_objects.size() == 0);
        GameManager::instance()->unscheduleUpdate(ck::makeFunctor(this,&Cluster::update));
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

        glVertex3f(0,0,0);
        glVertex3f(m_radius,0,0);

        glVertex3f(0,0,0);
        glVertex3f(0,m_radius,0);

        glEnd();
        glPopMatrix();

    }

    inline btVector3 center()
    {
        if(m_dirtyCenter)
            update(0.f);

        return m_center; 
    }

    inline float radius() { return m_radius; }

    inline void setRadius(float rad_) 
    {
        m_radius = rad_; 
        m_dirtyCenter = true; 
    }
   
    void registerObject(IClusterObject* object)
    {
        ck_assert(object);

        // Check if already in
        std::vector<IClusterObject*>::iterator it = std::find(m_objects.begin(), m_objects.end() , object);

        if(it != m_objects.end())
            return;

        m_objects.push_back(object);

        object->setCluster(shared_from_this());

        m_dirtyCenter = true;
    }

    void unregisterObject(IClusterObject* object)
    {
        ck_assert(object);

        std::vector<IClusterObject*>::iterator it = 
            std::find(m_objects.begin(), m_objects.end() , object);

        object->setCluster(nullptr);

        if(it == m_objects.end())
            return;

        m_objects.erase(it);

        m_dirtyCenter = true;
    }

    inline bool isIn(IClusterObject* object)
    {
        return (object->position().distance(m_center) <= m_radius);
    }

    const std::vector<IClusterObject*>& objects()
    {
        return m_objects;
    }

    void update(float dt)
    {
        btVector3 newCenter(0.f,0.f,0.f);

        int count = 0;

        for(IClusterObject* object : m_objects)
        {
            float weight = 1;

            float dist = object->position().distance(m_center);

            if(dist > m_radius)
                weight = max(0 , - dist / m_radius + 2);

            if(weight == 0.f)
                continue;
                
            newCenter += weight *  object->position();
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

    std::vector<IClusterObject*> m_objects;
};

#endif //_Cluster_hpp_