#ifndef __GameObjectTypes_hpp__
#define __GameObjectTypes_hpp__

#include <memory>

class GameObject;

typedef std::shared_ptr<GameObject> GameObject_sptr;
typedef std::weak_ptr<GameObject> GameObject_wptr;

#endif //__GameObjectTypes_hpp__