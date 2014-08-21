#ifndef __WorldMap_hpp__
#define __WorldMap_hpp__

#include "CKUSingleton.hpp"

#include "ff/FFTypes.hpp"

#include "../world.h"

#include "ff/FFWorld.hpp"

#include "VBO.hpp"

using namespace ck::utils;
using namespace ai::ff;

class WorldMap : public Singleton < WorldMap >
{
    friend Singleton<WorldMap>;

    WorldMap() 
    {
    }

public:

    inline static WorldMap* instance() { return Singleton<WorldMap>::instance(); }

    /// Extended cell type
    /// support conversion to and from NYCubeType
    enum class MapCell : uint8_t
    {
        GROUND      = 0, // CUBE_HERBE
        WATER       = 1, // CUBE_EAU
        DIRT        = 2, // CUBE_TERRE
        AIR         = 3, // CUBE_AIR
        TALL_GRASS  = 4,
        BUSH        = 5,
        MUSHROOM    = 6
    };

    static const int width  = MAT_SIZE_CUBES;
    static const int height = MAT_SIZE_CUBES;
    static const int count  = width * height;

    void createFrom(NYWorld* world_)
    {
        m_nyworld = world_;

        Cell c;

        for(int i = 0 ; i < count ; i++)
        {
            c.x = i % MAT_SIZE_CUBES;
            c.y = i / MAT_SIZE_CUBES;

            m_cells[i] = (MapCell) (m_nyworld->getCube(c.x,c.y, cellHeight(c)-1)->_Type);

            //ck_assert(m_cells[i] != MapCell::AIR);
        }
    }

    void render()
    {
        glPushMatrix();
       
        m_grassMesh.render();

	    glPopMatrix();
    }


    void generateBiome(int seed_ = 0)
    {
        srand(seed_);
        
        int areaCount = rand() % 10 + 10;

        for(int i = 0 ; i < areaCount ; i++)
        {
            int idx = rand() % count;

            int x = idx % MAT_SIZE_CUBES;
            int y = idx / MAT_SIZE_CUBES;

            generateTallGrassArea(x,y);
        }

        m_grassMesh.toVbo();
    }

    void generateTallGrassArea(int x_, int y_)
    {
        int count = 1;
        std::deque<Cell> gen = { Cell(x_,y_) };
        m_cells[x_ + y_ * MAT_SIZE_CUBES] = MapCell::TALL_GRASS;
        addTallGrassMesh(Cell(x_,y_));

        while(gen.size() > 0)
        {
            int size = gen.size();
            Cell c;
            for(int i=0 ; i<size ; i++)
            {
                c = gen.front();
                
                Cell neighbor;

                for(UniDirection udir = UniDirection::UP ; udir <= UniDirection::RIGHT ; udir++)
                {
                    neighbor = c+direction(udir);
                    
                    if(neighbor.x < 0 || neighbor.y < 0 || neighbor.x >= width || neighbor.y >= height )
                        continue;

                    MapCell mc = cellAt(neighbor);
    
                    if(mc == MapCell::GROUND || mc == MapCell::DIRT)
                    {
                        if(probaTallGrass(count))
                        {
                            setCellAt(neighbor, MapCell::TALL_GRASS);
                            gen.push_back(neighbor);
                            addTallGrassMesh(neighbor);
                            count++;
                        }
                    }
                }

                gen.pop_front();
            }
        }
    }

#define TALL_GRASS_THRESHOLD 300

    bool probaTallGrass(int count)
    {
        if(count < TALL_GRASS_THRESHOLD && rand()%3 >=1 )
            return true;
        return (rand()%TALL_GRASS_THRESHOLD >= 
                count - TALL_GRASS_THRESHOLD);
    }

    void generateMushroom(int x_, int y_)
    {

    }

    void generateBush(int x_, int y_)
    {

    }

    inline int static cellHeight(const Cell& cell_)
    {
        return cellHeight(cell_.x,cell_.y);
    }

    inline int static cellHeight(int x_, int y_)
    {
        if(!instance()->m_nyworld)
            throw std::exception();

        return instance()->m_nyworld->_MatriceHeights[x_][y_];
    }

    inline static float worldHeight(float x_, float y_)
    {
        return cellHeight((int)(x_ / NYCube::CUBE_SIZE), (int)(y_/NYCube::CUBE_SIZE))*NYCube::CUBE_SIZE;
    }

    inline static MapCell cellAt(const Cell& cell_)
    {
        return cellAt(cell_.x,cell_.y);
    }

    inline static MapCell cellAt(float x_,float y_)
    {
        return cellAt((int)(x_ / NYCube::CUBE_SIZE), (int)(y_/NYCube::CUBE_SIZE));
    }

    inline static MapCell cellAt(int x_, int y_)
    {
        return instance()->m_cells[x_ + y_ * MAT_SIZE_CUBES];
    }

    inline static void setCellAt(const Cell& cell_,const MapCell& mc_)
    {
        setCellAt(cell_.x,cell_.y,mc_);
    }

    inline static void setCellAt(float x_,float y_,const MapCell& mc_)
    {
        setCellAt((int)(x_ / NYCube::CUBE_SIZE), (int)(y_/NYCube::CUBE_SIZE),mc_);
    }

    inline static void setCellAt(int x_, int y_,const MapCell& mc_)
    {
        instance()->m_cells[x_ + y_ * MAT_SIZE_CUBES] = mc_;
    }

    template<typename _Config>
    void fillPathfinderWorld(World<_Config>* pfWorld_)
    {
        pfWorld_->beginChanges();

        for(int i = 0 ; i < count ; i++)
        {
            pfWorld_->setCostAt(cost(m_cells[i]), i % MAT_SIZE_CUBES, i / MAT_SIZE_CUBES);
        }

        pfWorld_->endChanges();
    }


    Cost cost(const MapCell& cell_)
    {
        switch(cell_)
        {
        case MapCell::DIRT:
        case MapCell::GROUND:
            return 0x00;

        case MapCell::BUSH:
        case MapCell::MUSHROOM:
            return 0x01;

        case MapCell::WATER:
            return 0xFF;

        case MapCell::TALL_GRASS:
            return 0x0F;

        default:
            return 0x00;
        }
    }

#define TALL_GRASS_OFFSET 1.f
#define TALL_GRASS_HEIGHT 4.f

    void addTallGrassMesh(const Cell& cell_)
    {
        float x0 = cell_.x * NYCube::CUBE_SIZE + TALL_GRASS_OFFSET;
        float y0 = cell_.y * NYCube::CUBE_SIZE + TALL_GRASS_OFFSET;
        float z0 = worldHeight(x0,y0);

        float x1 = (cell_.x + 1)* NYCube::CUBE_SIZE - TALL_GRASS_OFFSET;
        float y1 = (cell_.y + 1)* NYCube::CUBE_SIZE - TALL_GRASS_OFFSET;
        float z1 = z0 + TALL_GRASS_HEIGHT;

        TriangleSet<Vertex, 2> face1 = planeYZ(x0,y0,z0,   NYCube::CUBE_SIZE - 2 * TALL_GRASS_OFFSET ,TALL_GRASS_HEIGHT);
        TriangleSet<Vertex, 2> face2 = planeYZ(x1,y1,z0, - NYCube::CUBE_SIZE + 2 * TALL_GRASS_OFFSET ,TALL_GRASS_HEIGHT);
        TriangleSet<Vertex, 2> face3 = planeXZ(x1,y0,z0, - NYCube::CUBE_SIZE + 2 * TALL_GRASS_OFFSET ,TALL_GRASS_HEIGHT);
        TriangleSet<Vertex, 2> face4 = planeXZ(x0,y1,z0,   NYCube::CUBE_SIZE - 2 * TALL_GRASS_OFFSET ,TALL_GRASS_HEIGHT);

        m_grassMesh.vertices.push_back(face1.set[0]);
        m_grassMesh.vertices.push_back(face1.set[1]);
        m_grassMesh.vertices.push_back(face2.set[0]);
        m_grassMesh.vertices.push_back(face2.set[1]);
        m_grassMesh.vertices.push_back(face3.set[0]);
        m_grassMesh.vertices.push_back(face3.set[1]);
        m_grassMesh.vertices.push_back(face4.set[0]);
        m_grassMesh.vertices.push_back(face4.set[1]);

        Triangle<Color> color;
        color.v1.r = 0.9f;  color.v2.r = 0.9f;  color.v3.r = 0.9f; 
        color.v1.g = 0.17f; color.v2.g = 0.17f; color.v3.g = 0.17f;
        color.v1.b = 0.18f; color.v2.b = 0.18f; color.v3.b = 0.18f;

        m_grassMesh.colors.push_back(color);
        m_grassMesh.colors.push_back(color);
        m_grassMesh.colors.push_back(color);
        m_grassMesh.colors.push_back(color);
        m_grassMesh.colors.push_back(color);
        m_grassMesh.colors.push_back(color);
        m_grassMesh.colors.push_back(color);
        m_grassMesh.colors.push_back(color);

    }

private:

    MapCell m_cells[MAT_SIZE_CUBES*MAT_SIZE_CUBES];

    Mesh m_grassMesh;

    Mesh m_testMesh;

    NYWorld* m_nyworld;
};


#endif //__WorldMap_hpp__