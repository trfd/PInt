
#include "ai/WorldMap.hpp"

#include "ai/GameObject.hpp"

typedef GameObject::GridRegistrationInterface<WorldMap> __GRInt;

void WorldMap::registerOnGrid(GameObject* go_)
{
    unregisterFromGrid(go_);

    int idx = indexForLocation(go_->transform().getOrigin());

    m_gridObjects[idx].push_front(go_);

    __GRInt::setGridRegistrationIterator(go_,m_gridObjects[idx].begin());
}

void WorldMap::unregisterFromGrid(GameObject* go_)
{
    if(!__GRInt::gridRegistration(go_).isValid)
        return;

    int idx = indexForLocation(go_->transform().getOrigin());
    
    // Insure is valid (not end of list or incorrect pointer)
    ck_assert(*(__GRInt::gridRegistration(go_).object_it) == go_); 

    // The nifty thing is that even if the ObjectList_it (aka list iterator)
    // stored does NOT belong to this list (m_gridObjects[idx])
    // the correct list node is remove from the correct list (the one to which it belongs)
    m_gridObjects[idx].erase(__GRInt::gridRegistration(go_).object_it);

    __GRInt::gridRegistration(go_).isValid = false;
}

std::list<GameObject*> WorldMap::registeredObjectAt(const btVector3& loc_, int rad_ = 0)
{
    return registeredObjectAt(loc_,CellSize(2*rad_+1,2*rad_+1));
}


std::list<GameObject*> registeredObjectAt(const btVector3& loc_,const CellSize& size_ , const ck::Vector2i& offset_);
{
    if(size_.width == 0 || size_.height == 0)
        return std::list<GameObject*>();

    Cell c(loc_.x() / NYCube::CUBE_SIZE , loc_.y() / NYCube::CUBE_SIZE);

    c += offset_;

    c.x -= size_.width >> 1;
    c.y -= size_.height >> 1;

    return registeredObjectAt(CellRect(c,size_));
}


std::list<GameObject*> WorldMap::registeredObjectAt(const CellRect& rect)
{
    std::list<GameObject*> objectList;

    for(int x = rect.left ; x <= rect.right ; x++)
    {
        for(int y = rect.top ; y <= rect.bottom ; y++)
        {
            if(x < 0 || y < 0 || x >= c_worldSize || y >= c_worldSize )
                continue;

            objectList.insert(objectList.end(),
                            m_gridObjects[ x + y * c_worldSize ].begin(),
                            m_gridObjects[ x + y * c_worldSize ].end());
        }
    }

    return objectList;
}