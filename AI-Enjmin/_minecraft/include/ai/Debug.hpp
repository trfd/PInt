#ifndef __Debug_Utils_hpp__
#define __Debug_Utils_hpp__

#include <sstream>

#include "external/gl/glew.h"
#include "external/gl/freeglut.h"

#include "CKUSingleton.hpp"
#include "CKFunctor.hpp"

#include "LinearMath/btTransform.h"
#include "engine/utils/types_3d.h"

#define ENABLE_DEBUG

struct Line
{
    NYColor color;
    btVector3 start;
    btVector3 end;

    float lifetime;
};

class Debug : public ck::utils::Singleton<Debug>
{
    Debug(){}

    friend ck::utils::Singleton<Debug>;

public:

    inline static Debug* instance()
    {
        return ck::utils::Singleton<Debug>::instance();
    }

    inline static std::string toString(btVector3 const& vect)
    {
        std::string str = "{";
        str += std::to_string(vect.x()) + "," + std::to_string(vect.y()) + "," + std::to_string(vect.z()) + "}";
        return str;
    }

    static void addToRender(ck::Functor<void> const& func_)
    {
        instance()->m_renderCallback.push_back(func_);
    }

    static void drawLine(btVector3 const& start_,
                         btVector3 const& end_,
                         NYColor const&  color_ = NYColor(1.f, 1.f, 1.f, 1.f),
                         float duration_        = 1.f )
    {
#ifdef ENABLE_DEBUG
        instance()->m_lines.emplace_back();

        Line& line = instance()->m_lines.back();

        line.start    = start_;
        line.end      = end_;
        line.lifetime = duration_;
        line.color    = color_;
#endif
    }

    void update(float dt)
    {
        for(auto it = m_lines.begin() ; it != m_lines.end() ;)
        {
            if(it->lifetime > 0)
                it->lifetime -= dt , ++it;
            else
                it = m_lines.erase(it);
        }
    }

    void render()
    {
        for(ck::Functor<void>& func : m_renderCallback)
            func();

        if(m_lines.size() == 0)
            return;

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        //glLoadIdentity();

        glBegin(GL_LINES);

        for(Line& line : m_lines)
        {
            glColor3f(line.color.R,line.color.V,line.color.B);
            glVertex3f(line.start.x(), line.start.y(), line.start.z());
            glVertex3f(line.end.x(), line.end.y(), line.end.z());
        }

        glEnd();

        glPopMatrix();

      
    }

private:
    std::vector<ck::Functor<void>> m_renderCallback;
    std::vector<Line> m_lines;
};

#endif // __Debug_Utils_hpp__