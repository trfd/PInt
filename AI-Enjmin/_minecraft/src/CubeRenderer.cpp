
#include "ai/GameObject.hpp"
#include "ai/CubeRenderer.hpp"

void CubeRenderer::onRender()
{
    btVector3 pos = _gameObject->transform().getOrigin() + m_offset;

    btVector3& rotAxis = _gameObject->transform().getRotation().getAxis();

    glPushMatrix();
    glTranslatef(pos.x(), pos.y(),pos.z());
    glRotatef(_gameObject->transform().getRotation().getAngle(),
              rotAxis.x(),rotAxis.y(),rotAxis.z());
    glScalef(m_scale.x(), m_scale.y(), m_scale.z());
    drawCube();
    glPopMatrix();
}

void CubeRenderer::drawCube()
{
    glBegin(GL_POLYGON);
   
    glColor3f(1.0, 1.0, 1.0);
    glNormal3f( 0.f,  0.f, 1.f);
  
    glVertex3f( 0.5, -0.5, 0.5);
    glVertex3f( 0.5,  0.5, 0.5);
    glVertex3f(-0.5,  0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(1.f, 0.f, 0.f);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5,  0.5, -0.5);
    glVertex3f(0.5,  0.5,  0.5);
    glVertex3f(0.5, -0.5,  0.5);
    glEnd();


    glBegin(GL_POLYGON);
    glNormal3f(-1.f,  0.f,  0.f);
    glVertex3f(-0.5, -0.5,  0.5);
    glVertex3f(-0.5,  0.5,  0.5);
    glVertex3f(-0.5,  0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glEnd();


    glBegin(GL_POLYGON);
    glNormal3f( 0.f, 1.f, 0.f);
    glVertex3f( 0.5, 0.5,  0.5);
    glVertex3f( 0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5,  0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f( 0.f, -1.f, 0.f);
    glVertex3f( 0.5, -0.5, -0.5);
    glVertex3f( 0.5, -0.5,  0.5);
    glVertex3f(-0.5, -0.5,  0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f( 0.f, 0.f,  -1.f);
    glVertex3f( 0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5,  0.5, -0.5);
    glVertex3f( 0.5,  0.5, -0.5);

    glEnd();
}