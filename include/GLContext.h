/*
Copyright (c) 2013-2017 Jinrong Xie (jrxie at ucdavis dot edu)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _GL_CONTEXT_H_
#define _GL_CONTEXT_H_
#include "mat4.h"
#include <vector>
#include <stack>

namespace davinci{

struct GLLights{
    vec4f m_pos;
    vec4f m_color;
    GLLights(const vec4f& pos  = vec4f(0.0f,0.0f,0.0f,1.0f),
             const vec4f& color= vec4f(1.0f,1.0f,1.0f,1.0f) )

        :m_pos(pos), m_color(color){}
};
class GLContext
{
public:
    typedef enum{GL_ModelViewMatrix, GL_ProjectionMatrix, GL_NormalMatrix} MatrixMode;

    GLContext(void);
    ~GLContext(void);
public:
    static mat4 g_MVM;//model view matrix
    static mat4 g_PjM;//projection matrix
    static mat3 g_NM;//normal matrix
    static std::vector<GLLights> g_lights;


    static void glMatrixMode(MatrixMode mm){g_matrixMode = mm;}
    static void glPushMatrix();
    static void glPopMatrix();
    
protected:
    static MatrixMode g_matrixMode;
    static std::stack<mat4> g_MVMstack;
    static std::stack<mat4> g_PjMstack;
    static std::stack<mat3> g_NMstack;
};

}
#endif
