#ifndef __GameObject_inl_hpp__
#define __GameObject_inl_hpp__

#include "WorldMap.hpp"

inline void __thiscall GameObject::registerOnGrid()
{ WorldMap::instance()->registerOnGrid(this); }

inline void __thiscall GameObject::unregisterFromGrid()
{ WorldMap::instance()->unregisterFromGrid(this); }

#endif //__GameObject_inl_hpp__