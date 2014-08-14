#ifndef __CubeRenderer_hpp__
#define __CubeRenderer_hpp__

#include "GameComponent.hpp"

#include "external/gl/glew.h"
#include "external/gl/freeglut.h"

#include "LinearMath/btTransform.h"

class CubeRenderer : public GameComponent
{
public:

    #pragma region Constructors

    CubeRenderer()
    : m_offset(btVector3(0,0,0)),
    m_scale(btVector3(1,1,1))
    {}

    CubeRenderer(const btVector3& scale, 
                 const btVector3& offset)
    : m_offset(offset),
    m_scale(scale)
    {}

    #pragma endregion

    #pragma region Accessors

    inline btVector3& offset() { return m_offset; }
    inline btVector3& scale() { return m_scale; }

    void setOffset(const btVector3& off_) { m_offset = off_; }
    void setScale(float scl_) { m_scale = btVector3(scl_,scl_,scl_); }
    void setScale(const btVector3& scl_) { m_scale = scl_; }

    #pragma endregion

    #pragma region Component Override

    virtual void onRender() override;

    #pragma endregion

protected:

    void drawCube();

private:

    btVector3 m_offset;

    btVector3 m_scale;

};

#endif //__CubeRenderer_hpp__