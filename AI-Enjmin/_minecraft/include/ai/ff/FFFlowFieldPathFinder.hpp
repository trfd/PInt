///
///  FFFlowFieldPathFinder.hpp
///
///  Created by Baptiste Dupy on 18/08/2014.
///  Contact:
///          Mail:       <baptiste.dupy@gmail.com>
///          GitHub:     trfd <https://github.com/trfd>
///          Website:    <http://www.trf-d.com>
///
///  Copyright (C) 2014 Baptiste (trfd) Dupy
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
///
///

#ifndef AI_FF_FlowFieldPathFinder_hpp
#define AI_FF_FlowFieldPathFinder_hpp

#include <queue>

#include "ai/CKAssert.hpp"
#include "ai/CKUSingleton.hpp"
#include "ai/CKFunctor.hpp"

#include "FFFlowPath.hpp"
#include "FFFlowTile.hpp"
#include "FFFlowTileCache.hpp"
#include "FFIntegrator.hpp"

namespace ai
{
    namespace ff
    {
        template<typename _Config>
        class FlowFieldPathFinder : public ck::utils::Singleton<FlowFieldPathFinder<_Config>>
        {
            friend ck::utils::Singleton<FlowFieldPathFinder<_Config>>;

            FlowFieldPathFinder(){}

        public:
            
            #pragma region Typedefs/Constants

            typedef Utils<_Config>        Utils;
            typedef FlowTile<_Config>     FlowTile;
            typedef FlowTile_ptr<_Config> FlowTile_ptr;
            typedef FlowPath<_Config>     FlowPath;
            typedef FlowPath_ptr<_Config> FlowPath_ptr;
            typedef Integrator<_Config>   Integrator;

            static const int chunkWidth  = _Config::chunkWidth;
            static const int chunkHeight = _Config::chunkHeight;
            static const int chunkSize   = _Config::chunkWidth * _Config::chunkHeight;

            #pragma endregion 

            enum RequestState
            {
                INVALID_STATE = 0,
                PENDING       = 1,
                RUNNING       = 2,
                FAILURE       = 3,
                SUCCESS       = 4

            };

            enum RequestPhase
            {
                INVALID_PHASE   = 0,
                PORTAL_PATH     = 1,
                INTEGRATION     = 2
            };

            struct PathRequest
            {
                Cell from;
                std::vector<Cell> to;

                RequestState state = RequestState::INVALID_STATE;
                RequestPhase phase = RequestPhase::INVALID_PHASE;

                FlowPath_ptr path;
                
                template<typename _T>
                PathRequest(const Cell& from_, _T&& to_,const FlowPath_ptr& path_)
                : from(from_), to(std::forward<_T>(to_)), path(path_)
                {}
            };

            struct PortalPathSearch
            {   
                struct SingleSearch
                {
                    // PortalList index of 
                    Index fromIndex;
                    Index toIndex;
                };

                PathRequest* pathRequest;

                SingleSearch currSearch;
                
                typename Utils::PortalList fromPortals;
                typename Utils::PortalList toPortals;

                std::vector<ASPath<Index>> results;

                RequestState state;

                World<_Config>* world;
                ASPathFinder* pathfinder = nullptr;

                template<typename... Args_>
                PortalPathSearch(World<_Config>* world_,PathRequest* pathReq_,ChunkID from_,ChunkID to_)
                : world(world_), pathRequest(pathReq_)
                {
                    std::vector<FrontierID> fromFrontiers = Utils::allFrontiers(from_);
                    std::vector<FrontierID> toFrontiers = Utils::allFrontiers(to_);

                    for(FrontierID front : fromFrontiers)
                        fromPortals.insert(fromPortals.end(),world->portals(front).begin(), world->portals(front).end());

                    for(FrontierID front : toFrontiers)
                        toPortals.insert(toPortals.end(),world->portals(front).begin(), world->portals(front).end());
                }

                void startSearch()
                {
                    currSearch.fromIndex = 0;
                    currSearch.toIndex = 0;

                    if(fromPortals.size() == 0 || toPortals.size() == 0)
                    {
                        state = RequestState::FAILURE;
                        return;
                    }

                    pathfinder = new ASPathFinder(&world->portalGraph(), 
                        world->portalGraph().indexOfData(fromPortals[currSearch.fromIndex]),
                        world->portalGraph().indexOfData(toPortals  [currSearch.toIndex  ]));
                }

                void nextSearch()
                {
                    ck_assert(pathfinder->state() != ASPathFinder::RUNNING);

                    if(pathfinder->state() == ASPathFinder::TERMINATED)
                        results.push_back(pathfinder->path());

                    currSearch.toIndex++;

                    if(currSearch.toIndex >= toPortals.size())
                    {
                        currSearch.toIndex = 0;
                        currSearch.fromIndex++;

                        if(currSearch.fromIndex >= fromPortals.size())
                        {
                            state = RequestState::SUCCESS;
                            return;
                        }
                    }

                    pathfinder->restart(
                        world->portalGraph().indexOfData(fromPortals[currSearch.fromIndex]),
                        world->portalGraph().indexOfData(toPortals  [currSearch.toIndex  ]));
                    
                }

            };

            struct PathIntegrationRequest;

            struct TileIntegrationRequest
            {
                PathIntegrationRequest* pathIntegrationRequest;
                RequestState state;
                typename FlowTile::Data tileData;

                TileIntegrationRequest(PathIntegrationRequest* pIR_, const typename FlowTile::Data& data_)
                : pathIntegrationRequest(pIR_), tileData(data_)
                {}

            };

            struct PathIntegrationRequest
            {
                PathRequest* pathRequest;

                PortalPath portalPath;

                Integrator integrator;

                std::queue<TileIntegrationRequest> tileRequests;

                PathIntegrationRequest(World<_Config>* world, PathRequest* pathReq_,const PortalPath& ppath_)
                : pathRequest(pathReq_),
                portalPath(ppath_),
                integrator(world)
                {}
                
                /// Push a tile request to the queue if tile
                /// doesn't exist in the cache
                inline void pushTileRequest(FlowTileCache<_Config>* cache_ ,const typename FlowTile::Data& data_)
                {
                    // Check if data is already in cache
                    if(cache_->canAccess(data_.id()))
                    {
                        FlowTile_ptr& nTile = cache_->tileForID(data_.id());

                        ck_assert(data_ == nTile->data());
                        pathRequest->path->addTile(nTile);

                        return;
                    }
                    
                    tileRequests.emplace(this,data_);
                }

            };
            
            static FlowFieldPathFinder* instance(){ return  ck::utils::Singleton<FlowFieldPathFinder<_Config>>::instance(); }

            FlowPath_ptr path(Cell from_, Cell to_)
            {
                FlowPath_ptr newPath (new FlowPath());

                m_pendingPathRequests.emplace(from_,std::vector<Cell>({to_}),newPath);
                m_pendingPathRequests.back().state = RequestState::PENDING;

                return newPath;
            }

            void step(float dt)
            {
                // should use a timer
                stepPathRequest();
                stepPortalPathSearch();
                stepIntegration();
            }

            void terminate(const PathRequest& pathReq_)
            {
                //...
            }

            void stepPathRequest()
            {
                if(m_pendingPathRequests.size() == 0)
                    return;

                PathRequest& req = m_pendingPathRequests.front();

                ck_assert(req.state != RequestState::INVALID_STATE);

                switch(req.state)
                {
                case RequestState::PENDING:
                {
                    req.state = RequestState::RUNNING;
                    pushPortalPathSearch(req);
                    break;
                }
                case RequestState::SUCCESS:
                case RequestState::FAILURE:
                {
                    terminate(req);
                    m_pendingPathRequests.pop();
                    break;
                }
                }
            }

            void stepPortalPathSearch()
            {
                if(m_pendingPortalPathSearches.size() == 0)
                    return;

                PortalPathSearch& search = m_pendingPortalPathSearches.front();

                switch(search.state)
                {
                case RequestState::RUNNING:
                    break;
                case RequestState::PENDING:
                {
                    search.state = RequestState::RUNNING;
                    search.startSearch();
                    break;
                }
                case RequestState::SUCCESS:
                {
                    pushIntegrationRequest(search);
                    m_pendingPortalPathSearches.pop();
                    return;
                }
                case RequestState::FAILURE:
                {
                    search.pathRequest->state = RequestState::FAILURE;
                    terminate(*search.pathRequest);
                    m_pendingPortalPathSearches.pop();
                    return;
                }
                }
                
                switch(search.pathfinder->state())
                {
                case ASPathFinder::TERMINATED:
                case ASPathFinder::FAILURE:
                {
                    // Single Portal path search ended
                    search.nextSearch();

                    // If last search has been reached state would change
                    // then relaunch step to pass the new state on
                    if(search.state != RequestState::RUNNING)
                        stepPortalPathSearch();
                    break;
                }
                case ASPathFinder::RUNNING:
                {
                    // Perform pathfinder step
                    search.pathfinder->step();
                    
                    // if pathfinding has failed or succeeded
                    // relaunch step to pass the new state on
                    if(search.pathfinder->state() != ASPathFinder::RUNNING)
                        stepPortalPathSearch();
                    break;
                }
                }     
            }

            void stepIntegration()
            {
                if(m_pendingIntegrationRequests.size() == 0)
                    return;

                PathIntegrationRequest& intr = m_pendingIntegrationRequests.front();

                if(intr.tileRequests.size() == 0)
                {
                    terminate(*intr.pathRequest);
                    return;
                }

                TileIntegrationRequest& tileIntr = intr.tileRequests.front();

                ck_assert(tileIntr.pathIntegrationRequest == &intr);

                switch(tileIntr.state)
                {
                case RequestState::RUNNING:
                    break;
                case RequestState::PENDING:
                    startTileIntegration(tileIntr);
                    break;
                case RequestState::FAILURE:
                {
                    intr.pathRequest->state = RequestState::FAILURE;
                    terminate(*intr.pathRequest);
                    return;
                }
                case RequestState::SUCCESS:
                {
                    completeTileIntegration(tileIntr);
                    intr.tileRequests.pop();
                    return;
                }
                }

                switch(intr.integrator.state())
                {
                case Integrator::State::FAILURE:
                {
                    intr.pathRequest->state = RequestState::FAILURE;
                    terminate(*intr.pathRequest);
                    break;
                }
                case Integrator::State::TERMINATED:
                {
                    completeTileIntegration(tileIntr);
                    intr.tileRequests.pop();
                    break;
                }
                case Integrator::RUNNING:
                {
                    // Perform Integrator step
                    intr.integrator.stepCostIntegration();
                    
                    // if Integrator has failed or succeeded
                    // relaunch step to pass the new state on
                    if(intr.integrator.state() != Integrator::RUNNING)
                        stepIntegration();
                    break;
                }
                }
            }
            
            inline void pushPortalPathSearch(PathRequest& req_)
            {
                req_.state = RequestState::RUNNING;
                req_.phase = RequestPhase::PORTAL_PATH;

                ChunkID idFrom = Utils::chunkOfCell(req_.from);
                ChunkID idTo   = Utils::chunkOfCell(req_.to[0]);

                // Emplace new portal path search from fromPortal to toPortal
                // (using their indices in the graph)
                m_pendingPortalPathSearches.emplace(m_world,&req_,idFrom,idTo);
            }

            inline void pushIntegrationRequest(PortalPathSearch& search_)
            {
                // Select best portal search result

                int minSize = INT_MAX;
                ASPath<Index>* path_addr = nullptr;

                for(ASPath<Index>& path : search_.results)
                {
                    if(path.nodeList().size() < minSize)
                    {
                        path_addr = &path;
                        minSize = path.nodeList().size();
                    }
                }

                // Emplace a new integration request

                m_pendingIntegrationRequests.emplace(m_world,search_.pathRequest,
                   m_world->portalGraph().makePortalPath(*path_addr));

                PathIntegrationRequest& newIntr = m_pendingIntegrationRequests.back();

                ChunkID currChunk = Utils::chunkOfCell(newIntr.pathRequest->from);

                // Create Tile Request for each Portal Path step
                // each step request a flow going to the next portal entrance
                // that lies in current chunk
                for(auto it = newIntr.portalPath.begin() ; it != newIntr.portalPath.end() ; ++it)
                {
                    // Build data using next entrance and chunk
                    FlowTile::Data fData = FlowTile::Data(currChunk , {(*it)->entrance(currChunk)});

                    // Check if tile is not in cache 
                    // if already cached add tile to path
                    // otherwise push request
                    newIntr.pushTileRequest(&m_cache,fData);
                    
                    currChunk = (*it)->otherChunk(currChunk);
                }

                // Convert path goals to local cells

                std::vector<LocalCell> lcells;

                for(Cell cell : newIntr.pathRequest->to)
                {
                    lcells.push_back(Utils::toLocal(cell).second);
                }

                // Then create last tile request 

                // Check if tile is not in cache 
                // if already cached add tile to path
                // otherwise push request
                newIntr.pushTileRequest(&m_cache,FlowTile::Data(currChunk ,lcells));
                
            }

            void startTileIntegration(TileIntegrationRequest& tileIntr_)
            {
                Integrator& integrator = tileIntr_.pathIntegrationRequest->integrator;

                if(integrator.state() == Integrator::State::RUNNING)
                    throw std::exception();

                integrator.run(FlowTile_ptr(new FlowTile(m_world,tileIntr_.tileData)));

                tileIntr_.state = RequestState::RUNNING;
            }

            void completeTileIntegration(const TileIntegrationRequest& tileIntr_)
            {
                FlowTile_ptr tile = tileIntr_.pathIntegrationRequest->integrator.tile();
                
                tileIntr_.pathIntegrationRequest->pathRequest->path->addTile(tile);

                m_cache.addTile(tile);
            }

            #pragma region Accessors

            World<_Config>* world() { return m_world; }
            void setWorld(World<_Config>* world_) { m_world = world_; }

            #pragma endregion 
            
        private:

            FlowTileCache<_Config> m_cache;

            World<_Config>* m_world;
            
            std::queue<PathRequest> m_pendingPathRequests;

            std::queue<PortalPathSearch> m_pendingPortalPathSearches;

            std::queue<PathIntegrationRequest> m_pendingIntegrationRequests;
            

        };
    }
}

#endif //AI_FF_FlowFieldPathFinder_hpp