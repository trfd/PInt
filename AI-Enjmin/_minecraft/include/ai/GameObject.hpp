#ifndef __GameObject_hpp__
#define __GameObject_hpp__

#include <list>

#include "LinearMath/btTransform.h"

#include "CKComponentHolder.hpp"

#include "GameObjectTypes.hpp"

#include "GameComponent.hpp"

template<typename _Ctx>
struct GridRegistrator;

class GameObject 
: public ck::ComponentHolder<GameComponent>,
public std::enable_shared_from_this<GameObject>
{
public:
    
    typedef std::list<GameObject*>::iterator ObjectList_it;

    struct GridRegistration
    {
        bool isValid = false;
        ObjectList_it object_it;
    };

    enum
    {
        EARLY_UPDATE_PRIORITY   = 0,
        DEFAULT_UPDATE_PRIORITY = 100,
        PHYSIC_UPDATE_PRIORITY  = 200,
        LATE_UPDATE_PRIORITY    = 300
    };

    using ck::ComponentHolder<GameComponent>::PriorityPair;

    #pragma region Accessors

    inline const btTransform& transform() { return m_transform; }

    inline const btVector3& position() { return m_transform.getOrigin(); }

    inline const btQuaternion& rotation() { return m_transform.getRotation(); }

    /// Change the overall transform of the game object
    /// and pass this change on to the grid registration
    inline void setTransform(const btTransform& trans_)
    {
        m_transform = trans_;

        registerOnGrid();
    }

    /// Changes the position of the game object
    /// and pass this change on to grid registration
    inline void setPosition(const btVector3& vec_)
    {
        m_transform.setOrigin(vec_);

        registerOnGrid();
    }

    /// Changes the orientation of the game object
    /// Doesn't affect grid registration (ponctual registration)
    inline void setRotation(const btQuaternion& qu_)
    {
        m_transform.setRotation(qu_);
    }

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

    #pragma region GridRegistration

    inline void __thiscall registerOnGrid();
    inline void __thiscall unregisterFromGrid();

    /// Implementation of PInt for GridRegistration Interface
    /// see https://github.com/trfd/PInt for details
    /// on PInt
    template<typename _Ctx>
    struct GridRegistrationInterface
    {
        friend GridRegistrator<_Ctx>;
        friend typename GridRegistrator<_Ctx>::Type;
        
        inline static void setGridRegistration(GameObject* go,const GridRegistration& grd) 
        { go->setGridRegistration(grd); }

        inline static void setGridRegistrationIterator(GameObject* go,const ObjectList_it& it) 
        { go->setGridRegistrationIterator(it); }

        inline static void unsetGridRegistrationIterator(GameObject* go) 
        { go->unsetGridRegistrationIterator(); }

        inline static GridRegistration& gridRegistration(GameObject* go) 
        { return go->gridRegistration(); }
    };

private:

    /// Stores new value of grid registration
    /// see WorldMap for details about grid registration
    inline void setGridRegistration(const GridRegistration& grd_)
    {
        m_gridReg = grd_;
    }

    /// Stores new value of grid registration
    /// see WorldMap for details about grid registration
    inline void setGridRegistrationIterator(const ObjectList_it& it_)
    {
        m_gridReg.isValid = true;
        m_gridReg.object_it = it_;
    }

    /// Remove value of grid registration
    /// see WorldMap for details about grid registration
    inline void unsetGridRegistrationIterator()
    {
        m_gridReg.isValid = false;
        m_gridReg.object_it = ObjectList_it();
    }

    /// Returns current stored grid registration
    /// see WorldMap for details about grid registration
    inline GridRegistration& gridRegistration()
    {
        return m_gridReg;
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

    GridRegistration m_gridReg;

};

#include "GameObject_inl.hpp"

#endif // __GameObject_hpp__