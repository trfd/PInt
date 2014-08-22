#ifndef __MeshRenderer_hpp__
#define __MeshRenderer_hpp__

#include "VBO.hpp"

#include "GameObject.hpp"
#include "GameComponent.hpp"

#include "external/gl/glew.h"
#include "external/gl/freeglut.h"

#include "LinearMath/btTransform.h"

class MeshRenderer : public GameComponent
{
public:

    #pragma region Constructors

    MeshRenderer()
    : m_offset(btVector3(0,0,0)),
    m_scale(btVector3(1,1,1))
    {}

    MeshRenderer(const btVector3& scale, 
                 const btVector3& offset)
    : m_offset(offset),
    m_scale(scale)
    {}

    #pragma endregion

    #pragma region Accessors

    inline Mesh& mesh(){ return m_mesh; }

    inline btVector3& offset() { return m_offset; }
    inline btVector3& scale() { return m_scale; }

    void setOffset(const btVector3& off_) { m_offset = off_; }
    void setScale(float scl_) { m_scale = btVector3(scl_,scl_,scl_); }
    void setScale(const btVector3& scl_) { m_scale = scl_; }

    #pragma endregion

    #pragma region Component Override

    virtual void onRender() override
    {
        btVector3 pos = _gameObject->transform().getOrigin() + m_offset;

        btVector3& rotAxis = _gameObject->transform().getRotation().getAxis();

        glPushMatrix();
        glTranslatef(pos.x(),pos.y(),pos.z());
        glRotatef(_gameObject->transform().getRotation().getAngle() * 180.f/3.14f,
                  rotAxis.x(), rotAxis.y(),rotAxis.z());
        glScalef(m_scale.x(), m_scale.y(), m_scale.z());
        m_mesh.render();
        glPopMatrix();
    }

    #pragma endregion

protected:

    void drawCube();

private:
    
    Mesh m_mesh;

    btVector3 m_offset;

    btVector3 m_scale;


};

#endif //__MeshRenderer_hpp__