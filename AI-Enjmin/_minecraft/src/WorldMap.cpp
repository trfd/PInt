
#include "ai/WorldMap.hpp"

#include "ai/GameObject.hpp"

#include "ai/Debug.hpp"

typedef GameObject::GridRegistrationInterface<WorldMap> __GRInt;

std::vector<Cell> WorldMap::cellsOfType(const MapCell& mc_, const btVector3& loc_, const CellSize& size_, const ck::Vector2i& offset_)
{
    if(size_.width == 0 || size_.height == 0)
        return std::vector<Cell>();

    Cell c = toGridCoord(loc_);

    c += offset_;

    c.x -= size_.width >> 1;
    c.y -= size_.height >> 1;

    return cellsOfType(mc_,CellRect(c,size_));
}

std::vector<Cell> WorldMap::cellsOfType(const MapCell& mc_, const CellRect& rect)
{
    std::vector<Cell> cells;

    for(int x = rect.left() ; x <= rect.right() ; x++)
    {
        for(int y = rect.top() ; y <= rect.bottom() ; y++)
        {
            if(x < 0 || y < 0 || x >= c_worldSize || y >= c_worldSize )
                continue;

            if(m_cells[ x + y * c_worldSize] == mc_)
                cells.emplace_back(x,y);
        }
    }

    return cells;
}

void WorldMap::registerOnGrid(GameObject* go_)
{
    int idx = indexForLocation(go_->position());

    if(__GRInt::gridRegistration(go_).isValid && __GRInt::gridRegistration(go_).cellIndex == idx)
        return;

    unregisterFromGrid(go_);

    if(idx < 0)
    {
        Log::log(Log::USER_ERROR, "GameObject out of grid");
        go_->setPosition(toWorldCoord(Cell(c_worldSize>>1,c_worldSize>>1)));
        return;
    }

    ck_assert(idx >= 0 && idx < c_worldSize*c_worldSize);

    m_gridObjects[idx].push_front(go_);

    __GRInt::setGridRegistrationIterator(go_,m_gridObjects[idx].begin(),idx);
}

void WorldMap::unregisterFromGrid(GameObject* go_)
{
    ck_assert(go_);

    if(!__GRInt::gridRegistration(go_).isValid)
        return;
    
    // Insure is valid (not end of list or incorrect pointer)
    ck_assert(*(__GRInt::gridRegistration(go_).object_it) == go_); 

    ck_assert(__GRInt::gridRegistration(go_).cellIndex >= 0 && __GRInt::gridRegistration(go_).cellIndex < c_worldSize*c_worldSize);

    m_gridObjects[__GRInt::gridRegistration(go_).cellIndex]
        .erase(__GRInt::gridRegistration(go_).object_it);

    __GRInt::gridRegistration(go_).isValid = false;
}


std::vector<std::list<GameObject*>*> WorldMap::registeredObjectAt(const CellRect& rect)
{
    std::vector<std::list<GameObject*>*> objectList;

    for(int x = rect.left() ; x <= rect.right() ; x++)
    {
        for(int y = rect.top() ; y <= rect.bottom() ; y++)
        {
            if(x < 0 || y < 0 || x >= c_worldSize || y >= c_worldSize )
                continue;

            if(m_gridObjects[x + y * c_worldSize].size() > 0)
            {
                for(GameObject* obj : m_gridObjects[x + y * c_worldSize])
                {   ck_assert(Cell(x, y) == toGridCoord(obj->position())); }
                
                objectList.push_back(& m_gridObjects[ x + y * c_worldSize ]);
            }
                
        }
    }

    return objectList;
}

std::vector<std::list<GameObject*>*> WorldMap::registeredObjectAt(const btVector3& loc_, int rad_ )
{
    return registeredObjectAt(loc_,CellSize(2*rad_+1,2*rad_+1));
}


std::vector<std::list<GameObject*>*> WorldMap::registeredObjectAt(const btVector3& loc_,const CellSize& size_ , const ck::Vector2i& offset_)
{
    if(size_.width == 0 || size_.height == 0)
        return std::vector<std::list<GameObject*>*>();

    Cell c = toGridCoord(loc_);

    c += offset_;

    c.x -= size_.width >> 1;
    c.y -= size_.height >> 1;

    return registeredObjectAt(CellRect(c,size_));
}

