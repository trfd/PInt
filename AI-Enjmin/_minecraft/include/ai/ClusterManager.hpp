#ifndef __ClusterManager_hpp__
#define __ClusterManager_hpp__

#include "CKUSingleton.hpp"

#include "Cluster.hpp"

class PreyAgent;

#define __CLUSTER_SPLIT_SIZE__ 5
#define __CLUSTER_RADIUS__   100.f

class PreyClusterManager : ck::utils::Singleton<PreyClusterManager>
{
    friend ck::utils::Singleton<PreyClusterManager>;

    PreyClusterManager()
    {
        GameManager::instance()->scheduleUpdate(0.05f,this,&PreyClusterManager::update);
    }

public:

    inline static PreyClusterManager* instance(){ return ck::utils::Singleton<PreyClusterManager>::instance(); }

    void createCluster(PreyAgent* ag1, PreyAgent* ag2);

    void addToCluster(PreyAgent*,const Cluster_ptr&);
    void removeFromCluster(PreyAgent*);

    void update(float dt);

    void mergeClusters(const Cluster_ptr&,const Cluster_ptr&);

    void splitClusters(const Cluster_ptr&);

private:

    std::vector<Cluster_ptr> m_clusters;
};

#endif // __ClusterManager_hpp__