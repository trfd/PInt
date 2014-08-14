#ifndef __GameObject_hpp__
#define __GameObject_hpp__

#include "LinearMath/btTransform.h"

#include "CKComponentHolder.hpp"

#include "GameObjectTypes.hpp"

#include "GameComponent.hpp"

class GameObject 
: public ck::ComponentHolder<GameComponent>,
public std::enable_shared_from_this<GameObject>
{
public:

    enum
    {
        EARLY_UPDATE_PRIORITY   = 0,
        DEFAULT_UPDATE_PRIORITY = 100,
        PHYSIC_UPDATE_PRIORITY  = 200,
        LATE_UPDATE_PRIORITY    = 300
    };

    using ck::ComponentHolder<GameComponent>::PriorityPair;

    #pragma region Accessors

    inline btTransform& transform() { return m_transform; }

    #pragma endregion

    #pragma region Common Interface

    virtual void onInit()
    {
        for(const PriorityPair& gc : _components)
            gc.second->onInitGameObject();
    }

    virtual void update(float dt)
    {
        for(const PriorityPair& gc : _components)
            gc.second->onUpdate(dt);
    }

    virtual void render()
    {
        for(const PriorityPair& gc : _components)
            gc.second->onRender();
    }

    virtual void onDestroy()
    {
        for(const PriorityPair& gc : _components)
            gc.second->onDestroyGameObject();
    }

    #pragma endregion

protected:

    #pragma region Override ComponentHolder

    virtual void onAddComponent(GameComponent* comp_) override
    {
        comp_->setGameObject(shared_from_this());
        comp_->init();
    }

    virtual void onRemoveComponent(GameComponent* comp_) override
    {
        comp_->remove();
        comp_->setGameObject(GameObject_sptr(NULL));
    }

    #pragma endregion

private:

    btTransform m_transform;

};

#endif // __GameObject_hpp__