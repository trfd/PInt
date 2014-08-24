#ifndef __WorldMap_hpp__
#define __WorldMap_hpp__

#include <list>

#include "Pathfinding.hpp"

#include "CKUSingleton.hpp"

#include "../world.h"
#include "VBO.hpp"

using namespace ck::utils;
using namespace ai::ff;

template<typename _Ctx>
struct GridRegistrator
{
    typedef _Ctx Type;
};

class GameObject;

class WorldMap : public Singleton < WorldMap > , GridRegistrator<WorldMap>
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
        TREE        = 5,
        MUSHROOM    = 6
    };

    static const int width  = c_worldSize;
    static const int height = c_worldSize;
    static const int count  = width * height;

    #pragma region Initialization

    void createFrom(NYWorld* world_)
    {
        m_nyworld = world_;

        Cell c;

        for(int i = 0 ; i < count ; i++)
        {
            c.x = i % c_worldSize;
            c.y = i / c_worldSize;

            m_cells[i] = (MapCell) (m_nyworld->getCube(c.x,c.y, cellHeight(c)-1)->_Type);

            //ck_assert(m_cells[i] != MapCell::AIR);
        }
    }

    template<typename _Config>
    void fillPathfinderWorld(World<_Config>* pfWorld_)
    {
        pfWorld_->beginChanges();

        for(int i = 0 ; i < count ; i++)
        {
            pfWorld_->setCostAt(cost(m_cells[i]), i % c_worldSize, i / c_worldSize);
        }

        pfWorld_->endChanges();
    }

    #pragma endregion

    inline NYWorld* nyworld(){ return m_nyworld; }


    void render()
    {
        glPushMatrix();
       
        m_grassMesh.render();

        m_mushroomMesh.render();

        m_bushMesh.render();

	    glPopMatrix();
    }

    #pragma region Generation

    void generateBiome(int seed_ = 0)
    {
        srand(seed_);
        
        int areaCount = rand() % 10 + 10;
        int mushroomCount = rand() % 25 + 25;

        // Tall Grass

        for(int i = 0 ; i < areaCount ; i++)
        {
            int idx = rand() % count;

            int x = idx % c_worldSize;
            int y = idx / c_worldSize;

            generateTallGrassArea(x,y);
        }

        // Mushrooms

        for(int i = 0 ; i < mushroomCount ; i++)
        {
            int idx = rand() % count;

            int x = idx % c_worldSize;
            int y = idx / c_worldSize;

            MapCell mc = cellAt(x,y);

            if(mc == MapCell::GROUND || mc == MapCell::DIRT)
            {
                generateMushroom(x,y);
            }
        }

        for(int i = 0 ; i < mushroomCount ; i++)
        {
            int idx = rand() % count;

            int x = idx % c_worldSize;
            int y = idx / c_worldSize;

            generateBush(x,y);
        }

        m_grassMesh.toVbo();
        m_mushroomMesh.toVbo();
        m_bushMesh.toVbo();
    }

    void generateTallGrassArea(int x_, int y_)
    {
        int count = 1;
        std::deque<Cell> gen = { Cell(x_,y_) };
        m_cells[x_ + y_ * c_worldSize] = MapCell::TALL_GRASS;
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
        int count = rand() % 5 + 1;

        for(int i = 0 ; i < count ; i++)
        {
            addMushroomMesh(x_,y_);
        }
        
        m_cells[x_+y_*c_worldSize] = MapCell::MUSHROOM;
    }

    void generateBush(int x_, int y_)
    {
        int radius = rand() % 20 + 10;
        int count = rand() % 20 + 5;
        for(int i = 0 ; i < count ; i++)
        {
            int nx = x_ + rand()%(radius>>2) - radius;
            int ny = y_ + rand()%(radius>>2) - radius;

            if(nx < 0 || ny < 0 || nx >= width || ny >= height )
                continue;

            MapCell mc = cellAt(nx,ny);

            if(mc != MapCell::GROUND && mc != MapCell::DIRT)
                continue;

            addBushMesh(nx,ny);

            m_cells[nx+ny*c_worldSize] = MapCell::TREE;
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

    void addMushroomMesh(int x_, int y_)
    {
        float x0 = (x_+ ((float)rand())/ RAND_MAX) * NYCube::CUBE_SIZE;
        float y0 = (y_+ ((float)rand())/ RAND_MAX) * NYCube::CUBE_SIZE;
        float z0 = worldHeight(x0,y0);

        float trunkSize = ((float)rand()) / RAND_MAX * 13 + 4;
        float trunkWidth = ((float)rand()) / RAND_MAX * 3 + 1;

        float hatWidth = ((float)rand()) / RAND_MAX * trunkWidth + 5;
        float hatSize = ((float)rand()) / RAND_MAX;

        m_mushroomMesh.addCube(btVector3(x0, y0, z0 + trunkSize*0.5f), btVector3(trunkWidth, trunkWidth, trunkSize), NYColor(0.96f,0.91f,0.71f,1.f));

        m_mushroomMesh.addCube(btVector3(x0, y0, z0 + trunkSize), btVector3(hatWidth, hatWidth, hatSize), NYColor(0.13f,0.91f,0.67f,1.f));
    }

    void addBushMesh(int x_, int y_)
    {
        float x0 = (x_+ 0.5f) * NYCube::CUBE_SIZE;
        float y0 = (y_+ 0.5f) * NYCube::CUBE_SIZE;
        float z0 = worldHeight(x0,y0);

        float trunkSize = ((float)rand()) / RAND_MAX * 30 + 4;
        float trunkWidth = ((float)rand()) / RAND_MAX * 0.1f * NYCube::CUBE_SIZE +  0.4f * NYCube::CUBE_SIZE;

        float leafMaxWidth = ((float)rand()) / RAND_MAX * 20.f + 10.f;
        float leafHeight   =  ((float)rand()) / RAND_MAX * 5 + 2;

        int leafCount = rand() % 4;
        float reduceCoef = ((float)rand()) / RAND_MAX * 0.5f;

        m_bushMesh.addCube(btVector3(x0, y0, z0 + trunkSize*0.5f), btVector3(trunkWidth, trunkWidth, trunkSize), NYColor(0.41f,0.07f,0.07f,1.f));

        float leafWidth = 0.5f * leafMaxWidth;
        m_bushMesh.addCube(btVector3(x0, y0, z0 + trunkSize + leafHeight*0.5f), btVector3(leafWidth, leafWidth, leafHeight), NYColor(0.84f,0.25f,0.13f,1.f));

        leafWidth = 0.8f * leafMaxWidth;
        m_bushMesh.addCube(btVector3(x0, y0, z0 + trunkSize + leafHeight*1.5f), btVector3(leafWidth, leafWidth, leafHeight), NYColor(0.84f,0.25f,0.13f,1.f));

        leafWidth = leafMaxWidth;
        for(int i = 0 ; i < leafCount ; i++)
        {
            m_bushMesh.addCube(btVector3(x0, y0, z0 + trunkSize+leafHeight*(i+2.5f)), btVector3(leafWidth, leafWidth, leafHeight), NYColor(0.84f,0.25f,0.13f,1.f));
            leafWidth = reduceCoef * leafWidth;
        }
   }
        
    #pragma endregion

    #pragma region Heigths

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
        return cellHeight(x_ / NYCube::CUBE_SIZE, y_/NYCube::CUBE_SIZE ) * NYCube::CUBE_SIZE;
    }

    #pragma endregion

    #pragma region MapCells

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
        return instance()->m_cells[x_ + y_ * c_worldSize];
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
        instance()->m_cells[x_ + y_ * c_worldSize] = mc_;
    }

    Cost cost(const MapCell& cell_)
    {
        switch(cell_)
        {
        case MapCell::DIRT:
        case MapCell::GROUND:
            return 0x00;

        case MapCell::TREE:
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

    std::vector<Cell> cellsOfType(const MapCell& mc_, const btVector3& loc, const CellSize& size, const ck::Vector2i& offset);
    std::vector<Cell> cellsOfType(const MapCell& mc_, const CellRect& rect);

    #pragma endregion

    #pragma region Utils
    
    inline static btVector3 toWorldCoord(const Cell& cell_)
    {
        return btVector3(cell_.x * NYCube::CUBE_SIZE , cell_.y * NYCube::CUBE_SIZE, 0.f);
    }

    inline static Cell toGridCoord(const btVector3& vec_)
    {
        return Cell(vec_.x() / NYCube::CUBE_SIZE, vec_.y() / NYCube::CUBE_SIZE);
    }

    /// Returns the index of grid's cell of world coordinates location
    inline static int indexForLocation(const btVector3& vec_)
    {
        if(vec_.x() < 0.f || vec_.y() < 0.f || 
           vec_.x() >= c_worldSize*NYCube::CUBE_SIZE || 
           vec_.y() >= c_worldSize*NYCube::CUBE_SIZE)
           return -1;

        return (vec_.x() / NYCube::CUBE_SIZE) + (vec_.y() / NYCube::CUBE_SIZE) * c_worldSize;
    }

    /// Grid registration reduces complexity of spatial tests
    /// between game objects from O(N^2) to O(N)
    #pragma region GridRegistration

    /// Associates an object to its location on grid
    /// In order to make unregistering efficient ( O(1) )
    /// and safe, a list iterator copy is stored in the game object
    /// during registration. 
    void registerOnGrid(GameObject* go_);
   
    /// De-associates an object from its location on grid
    void unregisterFromGrid(GameObject* go_);

    /// Return the list of object registered on the grid
    /// around location in world coordinate
    /// rad_ is the number of cell to look around
    /// the lookup zone is actually a square
    std::vector<std::list<GameObject*>*> registeredObjectAt(const btVector3& loc_,int rad_ = 1);

    /// Return the list of object registered on the grid
    /// around location in world coordinate
    /// size_ defines the size of lookup zone around loc_ (loc_ is the center of rectangle)
    /// offset_ is the offset between location and the actual center of lookup zone
    std::vector<std::list<GameObject*>*> registeredObjectAt(const btVector3& loc_,const CellSize& size_ , const ck::Vector2i& offset_ = Cell(0,0));

    /// Return the list of object registered on the grid
    /// for cell in the (top-left) rectangle (grid coordinates)
    std::vector<std::list<GameObject*>*> registeredObjectAt(const CellRect& rect);

    #pragma endregion

private:

    std::list<GameObject*> m_gridObjects[c_worldSize*c_worldSize];

    MapCell m_cells[c_worldSize*c_worldSize];

    Mesh m_grassMesh;

    Mesh m_mushroomMesh;

    Mesh m_bushMesh;

    NYWorld* m_nyworld;
};



#endif //__WorldMap_hpp__