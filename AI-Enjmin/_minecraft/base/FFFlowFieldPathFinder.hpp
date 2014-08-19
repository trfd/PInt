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
/*
#include "ai/CKUSingleton.hpp"
#include "ai/CKFunctor.hpp"

#include "ai/ff//FFIntegrator.hpp"
#include "ai/ff/FFFlowTile.hpp"
#include "ai/ff/FFFlowTileCache.hpp"
*/

namespace ai
{
    namespace ff
    {
        template<typename _Config>
        class FlowFieldPathFinder : public ck::utils::Singleton<FlowFieldPathFinder<_Config>>
        {
            #pragma region Typedefs/Constants

            typedef Utils<_Config> Utils;

            static const int chunkWidth  = _Config::chunkWidth;
            static const int chunkHeight = _Config::chunkHeight;
            static const int chunkSize   = _Config::chunkWidth * _Config::chunkHeight;

            typedef typename _Config::FlowCell FlowCell;

            typedef Chunk<FlowCell,chunkWidth,chunkHeight> FlowChunk;

            typedef FlowTile<_Config> FlowTile;

            typedef ck::Functor<void,FlowFieldPath&,bool> PathCallback;

            #pragma endregion 

            struct FlowPath
            {

            };

            typedef std::shared_ptr<FlowPath> FlowPath_ptr;

            enum RequestState
            {
                INVALID     = 0,
                PENDING     = 1,
                RUNNING     = 2,
                FAILURE     = 3,
                SUCCESS     = 4

            };

            enum RequestPhase
            {
                INVALID         = 0,
                PORTAL_PATH     = 1,
                INTEGRATION     = 2
            };

            struct PathRequest
            {
                Cell from;
                std::vector<Cell> to;

                RequestState state = RequestState::INVALID;
                RequestPhase phase = RequestPhase::INVALID;

                FlowPath_ptr path;
                
                PathRequest(const Cell& from_, const Cell& to_, FlowPath_ptr path_)
                : from(from_), to(to_), path(path_)
                {}
            };

            struct PortalPathSearch
            {   
                PathRequest* pathRequest;

                ASPathFinder pathfinder;

                template<typename... Args_>
                PortalPathSearch(PathRequest* pathReq_,Args_&&... args_)
                : pathRequest(pathReq_)
                pathfinder(std::forward<Args_>(args_)...)
                {}
            };

            struct PathIntegrationRequest;

            struct TileIntegrationRequest
            {
                PathIntegrationRequest* pathIntegrationRequest;

                FlowTile::Data flowTileData;
            };

            struct PathIntegrationRequest
            {
                PathRequest* pathRequest;

                ASPath<Portal_ptr> portalPath;
                Integrator<_Config> integrator;

                std::queue<TileIntegrationRequest> tileRequests;

                PathIntegrationRequest(PathRequest* pathReq_,const ASPath<Portal_ptr>& portalPath_)
                : pathRequest(pathReq_)
                portalPath(portalPath_)
                {}
            };

            FlowPath_ptr path(Cell from_, Cell to_, PathCallback& callback_)
            {
                FlowPath_ptr newPath (new FlowPath());

                m_pendingPathRequests.emplace(from,to,newPath);

                return newPath;
            }

            void step()
            {
                
            }

            void terminate(const PathRequest& pathReq_)
            {
                //...
            }

            void stepPathRequest()
            {
                PathRequest& req = m_pendingPathRequests.front();

                ck_assert(req.state != RequestState::INVALID);

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

            void stepPortalPathSearch()
            {
                PortalPathSearch& search = m_pendingPortalPathSearches.front();

                switch(search.pathFinder.state())
                {
                case ASPathFinder::FAILURE:
                {
                    search.pathRequest.state = RequestState::FAILURE;
                    terminate(search.pathRequest);
                    break;
                }
                case ASPathFinder::TERMINATED:
                {
                    pushIntegrationRequest(search);
                    break;
                }
                case ASPathFinder::RUNNING:
                {
                    // Perform pathfinder step
                    search.pathFinder.step();
                    
                    // if pathfinding has failed or succeeded
                    // relaunch step to pass the new state on
                    if(search.pathFinder.state() != ASPathFinder::RUNNING)
                        stepPortalPathSearch();
                    break;
                }
                }
            }

            void stepIntegration()
            {
                PathIntegrationRequest& intr = m_pendingIntegrationRequests.front();

                if(intr.tileRequests.size() == 0)
                {
                    terminate(intr.pathRequest);
                    return;
                }

                TileIntegrationRequest& tileIntr = intr.tileRequests.front();

                switch(tileIntr.integrator.state())
                {
                case Intregrator<_Config>::FAILURE:
                {
                    tileIntr.pathIntegrationRequest.pathRequest.state = RequestState::FAILURE;
                    terminate(search.pathRequest);
                    break;
                }
                case Intregrator<_Config>::TERMINATED:
                {
                    completeTileIntegration(tileIntr);
                    intr.tileRequests.pop();
                    break;
                }
                case Intregrator<_Config>::RUNNING:
                {
                    // Perform Integrator step
                    tileIntr.integrator.stepCostIntegration();
                    
                    // if Integrator has failed or succeeded
                    // relaunch step to pass the new state on
                    if(tileIntr.integrator.state() != Intregrator<_Config>::RUNNING)
                        stepIntegration();
                    break;
                }
                }

            }

            inline void pushPortalPathSearch(const PathRequest& req_)
            {
                req_.state = RequestState::RUNNING;
                req_.phase = RequestPhase::PORTAL_PATH;

                Portal_ptr fromPortal = m_world->portalForCell(req_.from);

                Portal_ptr toPortal = m_world->portalForCells(req_.to);

                PortalGraph& graph = m_world->portalGraph();

                Index idxFrom = graph.indexOf(fromPortal);
                Index idxTo = graph.indexOf(toPortal);

                ck_assert(idxFrom != INT_MAX);
                ck_assert(idxTo   != INT_MAX);

                m_pendingPortalPathSearches.emplace(&req_, graph,idxFrom,idxTo);
                m_pendingPortalPathSearches.back().state = RequestState::RUNNING;
            }

            inline void pushIntegrationRequest(const PortalPathSearch& search_)
            {
                m_pendingIntegrationRequests.emplace(search_.pathRequest,
                    m_world.portalGraph().makePortalPath(search_.pathfinder.path()));

                PathIntegrationRequest& newIntr = m_pendingIntegrationRequests.back();

                // Go through path to create tileRequests
                for(auto it = newIntr.begin(); it != newIntr.end(); it++)
                {
                    TileRequest
                }

            }

            #pragma region Accessors

            World<_Config>& world() { return m_world; }

            #pragma endregion 

        private:

            FlowFieldCache<_Config> m_cache;

            World<_Config> m_world;

            std::queue<PathRequest> m_pendingPathRequests;

            std::queue<PortalPathSearch> m_pendingPortalPathSearches;

            std::queue<PathIntegrationRequest> m_pendingIntegrationRequests;

        };
    }
}

#endif //AI_FF_FlowFieldPathFinder_hpp