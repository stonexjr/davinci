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

#include <GL/glew.h>
#include "GLContext.h"
#include "GLError.h"
#include "vec4i.h"

namespace davinci{

GLContext::GLContext(void)
{
}


GLContext::~GLContext(void)
{
}

void GLContext::glPushMatrix()
{
    switch(GLContext::g_matrixMode)
    {
        case GL_ModelViewMatrix:
            g_MVMstack.push(GLContext::g_MVM);
            break;
        case GL_ProjectionMatrix:
            g_PjMstack.push(GLContext::g_PjM);
            break;
        case GL_NormalMatrix:
            g_NMstack.push(GLContext::g_NM);
            break;
        default:
            GLError::ErrorMessage(string(__func__)+": Unrecognized matrix mode!\n");
    }
}

void GLContext::glPopMatrix()
{
    switch(GLContext::g_matrixMode)
    {
        case GL_ModelViewMatrix:{
            if(g_MVMstack.empty()) GLError::ErrorMessage(string(__func__)+": g_MVMstack underflow!\n");
            GLContext::g_MVM = g_MVMstack.top();
            g_MVMstack.pop();
            break;
                                }
        case GL_ProjectionMatrix:{
            if(g_PjMstack.empty()) GLError::ErrorMessage(string(__func__)+": g_PjMstack underflow!\n");
            GLContext::g_PjM = g_PjMstack.top();
            g_PjMstack.pop();
            break;
                                 }
        case GL_NormalMatrix:{
            if(g_NMstack.empty()) GLError::ErrorMessage(string(__func__)+": g_NMstack underflow!\n");
            GLContext::g_NM = g_NMstack.top();
            g_NMstack.pop();
            break;
                             }
        default:
            GLError::ErrorMessage(string(__func__)+": Unrecognized matrix mode!\n");
    }
}

std::stack<mat4> GLContext::g_MVMstack;

std::stack<mat4> GLContext::g_PjMstack;

std::stack<mat3> GLContext::g_NMstack;

GLContext::MatrixMode GLContext::g_matrixMode = GLContext::GL_ModelViewMatrix;

mat3 GLContext::g_NM;
mat4 GLContext::g_PjM;
mat4 GLContext::g_MVM;

std::vector<GLLights> GLContext::g_lights;

}
