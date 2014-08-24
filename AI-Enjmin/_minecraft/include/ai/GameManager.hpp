#ifndef __GameManager_hpp__
#define __GameManager_hpp__

#include <vector>

#include "CKUSingleton.hpp"
#include "CKFunctor.hpp"

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

    

    struct ScheduleTask
    {
        typedef ck::Functor<void,float> Callback;
        // Time between each step
        float stepTime;
        float lastStepTime;
        Callback callback; 

        ScheduleTask(const Callback& c_,float t_)
        : stepTime(t_), lastStepTime(0.f), callback(c_)
        {}
    };

    typedef std::vector<ScheduleTask>       ScheduleTaskList;
    typedef std::vector<GameObject_sptr>    ObjectList;
    typedef ObjectList::iterator            ObjectList_it;

    inline static GameManager* instance() { return ck::utils::Singleton<GameManager>::instance(); }

    // Initialization callback from singleton
    inline virtual void init() override
    {
       
    }

    static void startGame()
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

    /// Schedule a functor to be called at regular time interval
    /// Time is the target time in second between two calls
    // Note that task can not be update faster than game loop rate
    // then scheduling with time between steps = 0 insure to
    // update every frame
    inline void scheduleUpdate(const ScheduleTask::Callback& call_, float time_ = 0.f)
    {
        m_registeredUpdates.emplace_back(call_,time_);
    }

    /// Schedule a functor to be called at regular time interval
    /// Time is the target time in second between two calls
    // Note that task can not be update faster than game loop rate
    // then scheduling with time between steps = 0 insure to
    // update every frame
    template<typename... Args_>
    inline void scheduleUpdate(float time_,Args_&&... args_)
    {
        m_registeredUpdates.emplace_back(ck::makeFunctor(std::forward<Args_>(args_)...),time_);
    }

    inline void unscheduleUpdate(const ScheduleTask::Callback& call_)
    {
        for(ScheduleTaskList::iterator it = m_registeredUpdates.begin() ;
            it != m_registeredUpdates.end() ; )
        {
            if(it->callback == call_)
                it = m_registeredUpdates.erase(it);
            else
                ++it;
        }
    }

    void update(float dt)
    {
        for(ScheduleTask& task : m_registeredUpdates)
        {
            task.lastStepTime += dt;
            
            if(task.lastStepTime >= task.stepTime)
            {
                task.callback(task.lastStepTime);
                task.lastStepTime = 0.f;
            }
        }

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

    ScheduleTaskList m_registeredUpdates;
};

#pragma endregion

#endif // __GameManager_hpp__