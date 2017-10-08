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

#include "GLIndexBufferObject.h"
#include "GLError.h"
using namespace davinci;

GLIndexBufferObject::GLIndexBufferObject(GLenum geotype,
        std::shared_ptr<GLenum> arrayid/*=NULL*/, GLenum usage/*=GL_STATIC_DRAW*/)
        :GLBufferObject(GL_ELEMENT_ARRAY_BUFFER, usage),
          m_arrayId(arrayid), m_type(geotype),m_indexDataSize(0),
          m_bUseRestart(false),m_restartIndex(0), m_enabled(false)
{
}

GLIndexBufferObject::~GLIndexBufferObject(void)
{
    clear();
    disable();
    GLError::glCheckError(__func__);
}

void GLIndexBufferObject::clear()
{
    m_indexData.clear();
    m_indexDataSize = 0;
}

void GLIndexBufferObject::upload()
{
    if (!m_indexData.empty())
    {//CPU data upload only once.
        m_indexDataSize = m_indexData.size();
        //glBufferData(m_target, m_indexDataSize*sizeof(GLuint), m_indexData.data(), m_usage);
        GLBufferObject::upload(m_indexDataSize*sizeof(GLuint), m_indexData.data());
        m_indexData.clear();//data uploaded to GPU, no need to keep the CPU counter part.
    }
    GLError::glCheckError(__func__);
}

void GLIndexBufferObject::enable()
{
    //glBindVertexArray(*m_arrayId);
    upload();
    GLBufferObject::bindBufferObject();
    if (m_bUseRestart)
    {
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(m_restartIndex);
    }
    GLBufferObject::unbindBufferObject();
    //glBindVertexArray(0);
    GLError::glCheckError(__func__);
    m_enabled = true;
}

void GLIndexBufferObject::attachVAO( const GLVertexArrayObjectRef& vbo )
{
    m_attachedVAO = vbo;
    m_arrayId = m_attachedVAO->getArrayId();
    //enable();
}

void davinci::GLIndexBufferObject::attachVAO(std::shared_ptr<GLenum> vaoid )
{
	m_arrayId = vaoid;
}
void GLIndexBufferObject::disable()
{
    //glBindVertexArray(*m_arrayId);
    GLBufferObject::bindBufferObject();
    if (m_bUseRestart)
    {
        glDisable(GL_PRIMITIVE_RESTART);
    }
    GLBufferObject::unbindBufferObject();
    //glBindVertexArray(0);
}

void GLIndexBufferObject::draw(GLsizeiptr count/*=0*/ )
{
    if (!m_enabled)
    {
        GLError::ErrorMessage(string(__func__)+" Please enable GLIndexBufferObject first, by calling enable(), since your last modification of IBO!");
    }
    if(!m_attachedVAO) 
		GLError::ErrorMessage(string(__func__)+" Please call GLIndexBufferObject::attachVAO() to attach a VAO to the IBO.!");

    //m_attachedVAO->checkEnable();
	if(!m_attachedVAO->isEnabled())
		m_attachedVAO->enable();

    glBindVertexArray(*m_arrayId);
    GLError::glCheckError("GLIndexBufferObject::draw(): glBindVertexArray() failed!");
    count = count > 0 ? count : m_indexDataSize;
    GLIndexBufferObject::bindBufferObject();

    glDrawElements(m_type, count, GL_UNSIGNED_INT, NULL);

    GLIndexBufferObject::unbindBufferObject();
    GLError::glCheckError("GLIndexBufferObject::draw(): glDrawArrays() failed!");
    glBindVertexArray(0);

	if(m_attachedVAO->isEnabled())
		m_attachedVAO->disable();
}

void davinci::GLIndexBufferObject::drawInstanced(GLsizei primcount, GLsizeiptr count/*=0*/)
{

    if (!m_enabled)
    {
        GLError::ErrorMessage(string(__func__)+" Please enable GLIndexBufferObject first, by calling enable(), since your last modification of IBO!");
    }
    if(!m_attachedVAO) 
		GLError::ErrorMessage(string(__func__)+" Please call GLIndexBufferObject::attachVAO() to attach a VAO to the IBO.!");

    //m_attachedVAO->checkEnable();
	if(!m_attachedVAO->isEnabled())
		m_attachedVAO->enable();

    glBindVertexArray(*m_arrayId);
    GLError::glCheckError("GLIndexBufferObject::draw(): glBindVertexArray() failed!");
    count = count > 0 ? count : m_indexDataSize;
    GLIndexBufferObject::bindBufferObject();

    glDrawElementsInstanced(m_type, count, GL_UNSIGNED_INT, NULL, primcount);

    GLIndexBufferObject::unbindBufferObject();
    GLError::glCheckError("GLIndexBufferObject::draw(): glDrawArrays() failed!");
    glBindVertexArray(0);

	if(m_attachedVAO->isEnabled())
		m_attachedVAO->disable();
}

GLIndexBufferObject& GLIndexBufferObject::operator<<( const GLuint i )
{
    m_indexData.push_back(i);
    m_enabled = false;
    return *this;
}

GLIndexBufferObject& GLIndexBufferObject::operator<<( const vec3i& i )
{
    m_indexData.push_back(i[0]);
    m_indexData.push_back(i[1]);
    m_indexData.push_back(i[2]);
    m_enabled = false;
    return *this;
}

GLIndexBufferObject& GLIndexBufferObject::operator<<( const vec4i& i )
{
    m_indexData.push_back(i[0]);
    m_indexData.push_back(i[1]);
    m_indexData.push_back(i[2]);
    m_indexData.push_back(i[3]);
    m_enabled = false;
    return *this;
}
