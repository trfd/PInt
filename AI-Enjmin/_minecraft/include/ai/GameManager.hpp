#ifndef __GameManager_hpp__
#define __GameManager_hpp__

#include <vector>

#include "CKUSingleton.hpp"

#include "GameObject.hpp"

#pragma region GameInitializer

class GameInitializer
{
public:

    static void init();

};

#pragma endregion


#pragma region GameManager

class GameManager 
: public ck::utils::Singleton<GameManager>
{
private:

    friend ck::utils::Singleton<GameManager>;

    GameManager()
    {}

public:

    typedef std::vector<GameObject_sptr> ObjectList;
    typedef ObjectList::iterator ObjectList_it;

    inline static GameManager* instance() { return ck::utils::Singleton<GameManager>::instance(); }

    // Initialization callback from singleton
    inline virtual void init() override
    {
        GameInitializer::init();
    }

    template<typename _T,typename... _Args>
    inline GameObject_sptr create(_Args&&... args)
    {
        GameObject_sptr go(new _T(std::forward<_Args>(args)...));
        m_objects.push_back(go);
        return go;
    }

    inline bool destroy(GameObject_sptr obj)
    {
        ObjectList_it it = std::find(m_objects.begin(), m_objects.end(),obj);
        
        if(it == m_objects.end())
            return false;

        it->reset();

        m_objects.erase(it);
    }

    void update(float dt)
    {
        for(GameObject_sptr ptr : m_objects)
        {
            ptr->update(dt);
        }
    }

    void render()
    {
        for(GameObject_sptr ptr : m_objects)
        {
            ptr->render();
        }
    }

private:

    ObjectList m_objects;
};

#pragma endregion

#endif // __GameManager_hpp__