#ifndef __GameComponent_hpp__
#define __GameComponent_hpp__

#include "GameObjectTypes.hpp"

class GameComponent
{
public:

    #pragma region Constructor/Destructor

    virtual ~GameComponent(){} 

    #pragma endregion

    #pragma region Accessors

    inline void setGameObject(GameObject_sptr go_)
    {
        _gameObject = go_;
    }

    inline GameObject_sptr& gameObject()
    {
        return _gameObject;
    }

    #pragma endregion

    #pragma region Override Interface

    /// Method called right after adding component to GameObject
    virtual void init(){}

    /// Called just before removing component from GameObject
    virtual void remove(){}

    virtual void onInitGameObject(){}
    virtual void onUpdate(float dt){}
    virtual void onRender(){}
    virtual void onDestroyGameObject(){}

    #pragma endregion

protected:

    GameObject_sptr _gameObject;
};

#endif // __GameComponent_hpp__