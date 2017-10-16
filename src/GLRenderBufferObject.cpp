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
#include "GLRenderBufferObject.h"

using namespace davinci;

GLRenderbufferObject::GLRenderbufferObject(void)
    :m_width(10), m_height(10)
{
    glGenRenderbuffersEXT(1, &m_rboId);
    bind();
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height);
    unbind();
}
GLRenderbufferObject::GLRenderbufferObject(GLsizei width, GLsizei height)
    :m_width(width), m_height(height)
{
    glGenRenderbuffersEXT(1, &m_rboId);
    bind();
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height);
    unbind();
}

GLRenderbufferObject::~GLRenderbufferObject( void )
{
    deleteBuffer();
}

void GLRenderbufferObject::resize( GLsizei width, GLsizei height )
{
    //delete existing buffer, if it exists.
    deleteBuffer();
    m_width = width;
    m_height= height;
    glGenRenderbuffersEXT(1, &m_rboId);
    bind();
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height);
    unbind();
}

void GLRenderbufferObject::bind()
{
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rboId);
}

void GLRenderbufferObject::unbind()
{
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

GLuint GLRenderbufferObject::getBufferId() const
{
    return m_rboId;
}
void GLRenderbufferObject::deleteBuffer()
{
    if (m_rboId){
        glDeleteRenderbuffersEXT(1, &m_rboId);
        m_rboId = 0;
        m_width = 10;
        m_height= 10;
    }
}

GLsizei GLRenderbufferObject::getWidth() const
{
    return m_width;
}

GLsizei GLRenderbufferObject::getHeight() const
{
    return m_height;
}