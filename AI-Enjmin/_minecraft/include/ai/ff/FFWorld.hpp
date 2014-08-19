///
///  FFWorld.hpp
///
///  Created by Baptiste Dupy on 15/08/2014.
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

#ifndef AI_FF_World_hpp
#define AI_FF_World_hpp

#include <map>

#include "ai/DtAdjacencyList.hpp"

#include "ASPathFinder.hpp"
#include "FFTypes.hpp"
#include "FFChunk.hpp"
#include "FFGrid.hpp"
#include "FFPortal.hpp"
#include "FFCostChunkGraph.hpp"
#include "FFPortalGraph.hpp"

#define __USE_SAFE_PORTAL_INSERT__
#define __ENABLE_DEBUG_DRAW__

namespace ai
{
    namespace ff
    {
        template<typename _Config>
        class Utils;

        template<typename _Config>
        class Integrator;

        template<int __width, int __height>
        struct DefaultWorldConfig
        {
            typedef BufferCell  BufferCell;
            typedef FlowCell    FlowCell;

            static const int chunkWidth  = 16;
            static const int chunkHeight = 16;
            
            static const int gridWidth   = __width;
            static const int gridHeight  = __height; 
        };

        //TODO: Change the way portal are created. In supporting actions
        // on portals such as Merge/Split/Expand/Reduce/Add/Remove, dynamic
        // mutation of costGrid can be achieved and alter only paths
        // linked to these portals
        template<typename _Config>
        class World
        {
        public:

            #pragma region Typedefs/Constants

            typedef Utils<_Config> Utils;

            static const int chunkWidth  = _Config::chunkWidth;
            static const int chunkHeight = _Config::chunkHeight;

            static const int gridWidth   = _Config::gridWidth;
            static const int gridHeight  = _Config::gridHeight;

            typedef typename _Config::BufferCell BufferCell;
            typedef typename _Config::FlowCell   FlowCell;

            typedef Chunk<Cost      ,chunkWidth,chunkHeight> CostChunk;
            typedef Chunk<BufferCell,chunkWidth,chunkHeight> BufferChunk;
            typedef Chunk<FlowCell  ,chunkWidth,chunkHeight> FlowChunk;

            typedef std::shared_ptr<CostChunk>               CostChunk_ptr;
            typedef CostChunkGraph<chunkWidth,chunkHeight>   CostChunkGraph;

            typedef Grid<CostChunk  ,gridWidth,gridHeight>   CostGrid;
            typedef Grid<BufferChunk,gridWidth,gridHeight>   BufferGrid;
            typedef Grid<FlowChunk  ,gridWidth,gridHeight>   FlowGrid;

            typedef std::shared_ptr<Portal> Portal_ptr;

            typedef std::vector<Portal_ptr>                  PortalList;
            typedef typename PortalList::iterator            PortalList_it;
            //typedef ck::gen::data::AdjacencyList<Portal_ptr> PortalGraph;
            typedef std::vector<Portal::Entrance*>           EntranceList;

            static const int chunkRowCount    = CostGrid::chunkRowCount;
            static const int chunkColumnCount = CostGrid::chunkColumnCount;
            static const int chunkCount       = CostGrid::chunkCount;

            static const int frontierCount    = (2 * chunkRowCount - 1) * chunkColumnCount  - chunkRowCount ;

            #pragma endregion 
       
            World();

            #pragma region Debug

            void stepIntegrator();

            void drawGrid();

            #pragma endregion 

            #pragma region Accessors

            inline Cost costAt(int x_, int y_);
            inline Cost costAt(const Cell& cell_);

            inline void setCostAt(const Cost& cost_,const Cell& cell_);

            inline void setCostAt(const Cost& cost_, int x_, int y_);

            inline bool isPathable(Cell const& cell_);

            inline PortalGraph& portalGraph();
           
            #pragma endregion

            inline void beginChanges(){}
            inline void endChanges(){ computePortals(); }
        
        private:

            #pragma region Portals

            /// Use the current state of the cost grid to
            /// find and create portals
            /// This method is far from being optimal.
            /// This uses a brute-force by removing
            /// every portal linked to the chunk
            /// and recreating them.
            void computePortals();

            void computePortalGraph();

            /// Create portals for a given frontier
            /// This creation of portal removes every existing portals
            /// even if untouched. 
            //TODO: Improve efficiency of portal mutation by 
            // implementating Merge/Split/Extend/Reduce/Add/Remove mutation
            // on portals. This is tightly linked to agent's path recomputing.
            void createAllPortalsOn(FrontierID frontier_);

            /// Create a new portal shared pointer.
            Portal_ptr createNewPortal(FrontierID frontier_,ChunkID chunk1_, ChunkID chunk2_,
                                       const Cell& entr1Origin_, const Cell& entr2Origin_,
                                       const CellSize& size_);

            /// Adds a portal to the list of portal, its entrances 
            /// to the map of entrances
            /// and add the corresponding node to the portal graph.
            void addPortal(Portal_ptr portal_);

            #pragma endregion

            /// Grid containing cost of cells
            CostGrid m_costGrid;

            CostChunkGraph m_costChunkGraph[chunkCount];
                
            PortalGraph m_portalGraph;

            std::vector<ASPath<Index>> m_portalPaths[chunkCount];

            PortalList m_portals[frontierCount];

            EntranceList m_entrances[chunkCount];

            ASPath<Portal_ptr> portalPathTest;

            Integrator<_Config>* testIntegrator;
            
        };
    }
}

#include "FFWorld_impl.hpp"

#endif //AI_FF_World_hpp