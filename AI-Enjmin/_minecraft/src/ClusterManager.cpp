
#include "ai/ClusterManager.hpp"

#include "ai/PreyAgent.hpp"

void PreyClusterManager::createCluster(PreyAgent* ag1, PreyAgent* ag2)
{
    m_clusters.push_back(std::make_shared<Cluster>(__CLUSTER_RADIUS__));
    m_clusters.back()->registerObject(ag1);
    m_clusters.back()->registerObject(ag2);

    m_clusters.back()->resetCenter();
}

void PreyClusterManager::addToCluster(PreyAgent* ag, const Cluster_ptr& cl)
{
    cl->registerObject(ag);

    if(cl->objects().size() >= __CLUSTER_SPLIT_SIZE__)
        splitClusters(cl);
}

void PreyClusterManager::removeFromCluster(PreyAgent* ag)
{
    if(!ag->cluster().get())
        return;

    Cluster_ptr ptr = ag->cluster();

    ptr->unregisterObject(ag);

    if(ptr->objects().size() == 1)
        ptr->unregisterObject(ptr->objects()[0]);
}

void PreyClusterManager::update(float dt)
{
    std::vector<std::pair<Cluster_ptr,Cluster_ptr>> mergeList;

    for(Cluster_ptr& c1 : m_clusters)
    {
        for(Cluster_ptr& c2 : m_clusters)
        {
            if(c1 == c2)
                continue;

            if(c1->center().distance(c2->center()) < c1->radius() + c2->radius() &&
               c1->objects().size() + c2->objects().size() < __CLUSTER_SPLIT_SIZE__)
               mergeList.push_back(std::make_pair(c1,c2));

        }
    }

    for(auto it = mergeList.begin() ; it != mergeList.end() ; ++it)
    {
        if(it->first.get() && it->second.get())
            mergeClusters(it->first,it->second);
    }

    for(auto cluster : m_clusters)
        cluster->update(dt);
}

void PreyClusterManager::mergeClusters(const Cluster_ptr& cl1, const Cluster_ptr& cl2)
{
    if(cl1->objects().size() + cl2->objects().size() >= __CLUSTER_SPLIT_SIZE__)
        return;

    // Copy since array will change during merge
    std::vector<IClusterObject*> objs = cl2->objects();

    for(auto obj : objs)
    {
        cl2->unregisterObject(obj);

        cl1->registerObject(obj);
    }

    auto it = std::find(m_clusters.begin(), m_clusters.end(),cl2);

    if(it != m_clusters.end())
        m_clusters.erase(it);

    cl1->resetCenter();
}

void PreyClusterManager::splitClusters(const Cluster_ptr& cl)
{
    Cluster_ptr nCluster = std::make_shared<Cluster>(__CLUSTER_RADIUS__);

     // Copy since array will change during merge
    std::vector<IClusterObject*> objs = cl->objects();

    ck_assert(objs.size() >= __CLUSTER_SPLIT_SIZE__);

    int size = objs.size() >> 1;

    for(int i = 0 ; i<size ; i++)
    {
        cl->unregisterObject(objs[i]);

        nCluster->registerObject(objs[i]);
    }

    m_clusters.push_back(nCluster);

    nCluster->resetCenter();

    cl->resetCenter();
}