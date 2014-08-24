#ifndef __VBO_hpp__
#define __VBO_hpp__

#include "LinearMath/btTransform.h"
#include "external/gl/glew.h"
#include "external/gl/freeglut.h" 

#include "CKVector.hpp"

#include <vector>

#include "engine/render/renderer.h"

struct Color
{
    static const int size = 3 * sizeof(float);

    float r;
    float g;
    float b;

    Color()
    : r(0.f), g(0.f), b(0.f)
    {}

    Color(float r_,float g_, float b_)
    : r(r_), g(g_), b(b_)
    {}
};

struct Vertex
{
    static const int size = 3 * sizeof(float);

    float x;
    float y;
    float z;

     Vertex()
     :x(0.f), y(0.f), z(0.f)
     {}

    Vertex(float x_,float y_, float z_)
    : x(x_), y(y_), z(z_)
    {}  
};

typedef Vertex Normal;

template<typename _T>
struct Triangle
{
    static const int size = 3 * _T::size;

    _T v1;
    _T v2;
    _T v3;

    Triangle()
    {}

    Triangle(const _T& v_)
    : v1(v_), v2(v_), v3(v_)
    {}

    Triangle(const _T& v1_,const _T& v2_,const _T& v3_)
    : v1(v1_), v2(v2_), v3(v3_)
    {}
};

template<typename _T,int __size>
struct TriangleSet
{
    Triangle<_T> set[__size];
};

struct Mesh
{
    std::vector<Triangle<Vertex>> vertices;
    std::vector<Triangle<Color>>  colors;
    std::vector<Triangle<Normal>> normals;

    GLuint vboID;

    void toVbo(void)
	{
		if(vboID != 0)
			glDeleteBuffers(1, &vboID);

		glGenBuffers(1, &vboID);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		glBufferData(GL_ARRAY_BUFFER,          
			vertices.size() * Triangle<Vertex>::size + 
			colors.size()   * Triangle<Color> ::size + 
			normals.size()  * Triangle<Normal>::size , 
			NULL,                              
			GL_STREAM_DRAW); 

		GLenum error = glGetError();

		glBufferSubData(GL_ARRAY_BUFFER,
			0,                         
			vertices.size() * Triangle<Vertex>::size, 
			vertices.data());            

		error = glGetError();

		glBufferSubData(GL_ARRAY_BUFFER,
			vertices.size() * Triangle<Vertex>::size, 
			colors.size()   * Triangle<Color> ::size,
			colors.data());         

		error = glGetError();

		glBufferSubData(GL_ARRAY_BUFFER,
			vertices.size() * Triangle<Vertex>::size+ 
			colors.size()   * Triangle<Color> ::size, 
			normals.size()  * Triangle<Normal>::size,
			normals.data());         

		error = glGetError();

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void render(void)
	{
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		NYRenderer::checkGlError("glBindBuffer");

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, (void*)(0));
		glColorPointer( 3, GL_FLOAT, 0, (void*)(vertices.size() * Triangle<Vertex>::size));
		glNormalPointer(   GL_FLOAT, 0, (void*)(vertices.size() * Triangle<Vertex>::size + colors.size() * Triangle<Color>::size));

		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3 );

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
	}

    void addCube(const btVector3& loc_, const btVector3& size_ , const NYColor& color_)
    {
        Color c(color_.R,color_.V,color_.B);
      
        Vertex loc(loc_.x(), loc_.y(), loc_.z());
        Vertex size(size_.x(), size_.y(), size_.z());

        // Face - Z
        colors.emplace_back(c);
        colors.emplace_back(c);
        normals.emplace_back(Normal(0,0,-1));
        normals.emplace_back(Normal(0,0,-1));
        vertices.emplace_back(Vertex(loc.x - 0.5f * size.x , loc.y - 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z - 0.5f * size.z));
        
        vertices.emplace_back(Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z));

        // Face  Z
        colors.emplace_back(c);
        colors.emplace_back(c);
        normals.emplace_back(Normal(0,0,1));
        normals.emplace_back(Normal(0,0,1));
        vertices.emplace_back(Vertex(loc.x - 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z + 0.5f * size.z));
                                                                                                
        vertices.emplace_back(Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y + 0.5f * size.y , loc.z + 0.5f * size.z));

        // Face  -Y
        colors.emplace_back(c);
        colors.emplace_back(c);
        normals.emplace_back(Normal(0,-1,0));
        normals.emplace_back(Normal(0,-1,0));
        vertices.emplace_back(Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y - 0.5f * size.y , loc.z - 0.5f * size.z));
                                                                                                
        vertices.emplace_back(Vertex(loc.x - 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z));

        // Face  Y
        colors.emplace_back(c);
        colors.emplace_back(c);
        normals.emplace_back(Normal(0,1,0));
        normals.emplace_back(Normal(0,1,0));
        vertices.emplace_back(Vertex(loc.x + 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z + 0.5f * size.z));
                                                                                          
        vertices.emplace_back(Vertex(loc.x + 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y + 0.5f * size.y , loc.z + 0.5f * size.z));

    
        // Face  -X
        colors.emplace_back(c);
        colors.emplace_back(c);
        normals.emplace_back(Normal(-1,0,0));
        normals.emplace_back(Normal(-1,0,0));
        vertices.emplace_back(Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y - 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z));
                                                                                        
        vertices.emplace_back(Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x - 0.5f * size.x , loc.y + 0.5f * size.y , loc.z + 0.5f * size.z));

        // Face  X
        colors.emplace_back(c);
        colors.emplace_back(c);
        normals.emplace_back(Normal(1,0,0));
        normals.emplace_back(Normal(1,0,0));
        vertices.emplace_back(Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z));
                                                                                          
        vertices.emplace_back(Vertex(loc.x + 0.5f * size.x , loc.y - 0.5f * size.y , loc.z + 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y + 0.5f * size.y , loc.z - 0.5f * size.z),
                              Vertex(loc.x + 0.5f * size.x , loc.y + 0.5f * size.y , loc.z + 0.5f * size.z));

    }
};

inline TriangleSet<Vertex,2> planeXY(float x, float y, float z,float dx, float dy)
{
    TriangleSet<Vertex,2> tset;

    tset.set[0].v1.x = x;      tset.set[0].v1.y = y;      tset.set[0].v1.z = z;
    tset.set[0].v2.x = x;      tset.set[0].v2.y = y + dy; tset.set[0].v2.z = z;
    tset.set[0].v3.x = x + dx; tset.set[0].v3.y = y;      tset.set[0].v3.z = z;
                           
    tset.set[1].v1.x = x + dx; tset.set[1].v1.y = y;      tset.set[1].v1.z = z;
    tset.set[1].v2.x = x;      tset.set[1].v2.y = y + dy; tset.set[1].v2.z = z;
    tset.set[1].v3.x = x + dx; tset.set[1].v3.y = y + dy; tset.set[1].v3.z = z;

    return tset;
}

inline TriangleSet<Vertex,2> planeXZ(float x, float y, float z,float dx, float dz)
{
    TriangleSet<Vertex,2> tset;

    tset.set[0].v1.x = x;      tset.set[0].v1.z = z;      tset.set[0].v1.y = y;
    tset.set[0].v2.x = x;      tset.set[0].v2.z = z + dz; tset.set[0].v2.y = y;
    tset.set[0].v3.x = x + dx; tset.set[0].v3.z = z;      tset.set[0].v3.y = y;
                 
    tset.set[1].v1.x = x + dx; tset.set[1].v1.z = z;      tset.set[1].v1.y = y;
    tset.set[1].v2.x = x;      tset.set[1].v2.z = z + dz; tset.set[1].v2.y = y;
    tset.set[1].v3.x = x + dx; tset.set[1].v3.z = z + dz; tset.set[1].v3.y = y;

    return tset;
}

inline TriangleSet<Vertex,2> planeYZ(float x, float y, float z,float dy, float dz)
{
    TriangleSet<Vertex,2> tset;

    tset.set[0].v1.y = y;      tset.set[0].v1.z = z;      tset.set[0].v1.x = x;
    tset.set[0].v2.y = y;      tset.set[0].v2.z = z + dz; tset.set[0].v2.x = x;
    tset.set[0].v3.y = y + dy; tset.set[0].v3.z = z;      tset.set[0].v3.x = x;
                                                                         
    tset.set[1].v1.y = y + dy; tset.set[1].v1.z = z;      tset.set[1].v1.x = x;
    tset.set[1].v2.y = y;      tset.set[1].v2.z = z + dz; tset.set[1].v2.x = x;
    tset.set[1].v3.y = y + dy; tset.set[1].v3.z = z + dz; tset.set[1].v3.x = x;

    return tset;
}

template<int __size>
inline TriangleSet<Normal, __size> fill(float x, float y, float z)
{
    TriangleSet<Normal,__size> tset;

    Normal n; n.x = x; n.y = y ; n.z = z;

    for(int i = 0 ; i < __size ; i++)
    {
        tset.set[0].v1 = n; tset.set[0].v2 = n; tset.set[0].v3 = n;
    }

    return tset;
}



#endif