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

#include <sstream>
#include <iostream>
#include "GLAttribute.h"
#include "GLError.h"

namespace davinci{

GLAttribute::GLAttribute(GLuint shaderProgId,  const string& name,
	 GLenum type, GLuint size, GLuint stride/*=0*/, 
	 GLuint offset/*=0*/, bool normalized/*=false*/,bool warning/*=false*/,
	 const GLVertexBufferObjectRef vbo/*=NULL*/)
     :m_shaderProgId(shaderProgId), m_name(name),
	  m_type(type),m_nComponents(size), m_stride(stride),m_offset(offset),
      m_normalized(normalized), m_enableWarning(warning), m_divisor(0),
      m_vbo(vbo), m_bActive(true)
{
}

GLAttribute::GLAttribute(GLuint shaderProgId,  const string& name,
	 GLenum type, GLuint size, GLuint stride/*=0*/, 
	 GLuint offset/*=0*/, bool normalized/*=false*/,bool warning/*=false*/,
     GLuint divisor/*=0*/, const GLVertexBufferObjectRef vbo/*=NULL*/)
     :m_shaderProgId(shaderProgId), m_name(name),
	  m_type(type),m_nComponents(size), m_stride(stride),m_offset(offset),
      m_normalized(normalized), m_enableWarning(warning), m_divisor(divisor),
      m_vbo(vbo), m_bActive(true)
{
}

GLAttribute::~GLAttribute(void)
{
}
//for GL 3.3 core version.
#if defined(__APPLE__) || defined(MACOSX)
void GLAttribute::enable()
{
    GLint index=-1;
    index = glGetAttribLocation( m_shaderProgId, m_name.data());
    if (index==-1)
    {
        std::stringstream ss;
        ss <<"GLAttribute::enable3_3(): index==-1, cannot find "<<m_name<<"\n";
        std::string msg=ss.str();
        GLError::ErrorMessage(msg);
    }else if(m_vbo && m_vbo->getVertexCount()>0){
        m_vbo->bindBufferObject();
        GLError::glCheckError("GLAttribute::enable() m_vbo.bind() failed!");
    }
    glEnableVertexAttribArray(index);
    //http://www.opengl.org/wiki/Vertex_Buffer_Object#Vertex_format/
    //For floating-point attributes, you must use glVertexAttribPointer.
    //For integer (both signed and unsigned), you must use glVertexAttribIPointer.
    //And for double-precision attributes, where available, you must use glVertexAttribLPointer.
    if(m_type == GL_INT || m_type == GL_UNSIGNED_BYTE
                    || m_type == GL_UNSIGNED_INT){
        //We need glVertexAttribIPointer( with I) to match your shader
        //int/uint attribute.
        glVertexAttribIPointer(index, m_nComponents, m_type,m_stride,\
                               reinterpret_cast<const GLvoid*>(m_offset));
    }
//    else if (m_type == GL_DOUBLE)
//    {
//        glVertexAttribLPointer(index, m_nComponents, m_type,m_stride,\
//                               reinterpret_cast<const GLvoid*>(m_offset));
//    }
    else{
        glVertexAttribPointer(index, m_nComponents, m_type,m_normalized,m_stride,
                              reinterpret_cast<const GLvoid*>(m_offset));
    }
    glVertexAttribDivisor(index, m_divisor); 
    if(m_vbo && m_vbo->getVertexCount()>0){
        m_vbo->unbindBufferObject();
        GLError::glCheckError("GLAttribute::enable() m_vbo.unbind() failed!");
    }

    GLError::glCheckError(__func__);
}
#else
//For GL version 4.4
void GLAttribute::enable()
{
	GLint attrbIndex=-1;
	GLError::glCheckError(__func__);
	attrbIndex = glGetAttribLocation( m_shaderProgId, m_name.data());

	if (attrbIndex==-1)
	{
		std::stringstream ss;
		ss <<__func__<<": index==-1, cannot find "<<m_name<<"\n";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);

	}else if (!m_vbo)
	{
		std::stringstream ss;
		ss <<__func__<<": m_vbo==NULL, there is no associated VBO with the attribute "
		   <<m_name<<".\nCall attach() or specify a VBO at the constructor.";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
	}else if (m_vbo->getVertexCount()<=0)
	{
		std::stringstream ss;
		ss <<__func__<<": m_vbo->getVertexCount()<=0, No vertex attribute content in the associated VBO with the attribute "
		   <<m_name<<".\nCall GLVertexBufferObject::upload() to upload vertex attribute content to GPU";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
	}
	//glBindVertexBuffer(0, buff, baseOffset, sizeof(Vertex));
	//http://www.opengl.org/wiki/Vertex_Buffer_Object#Vertex_format/
	//http://www.sinanc.org/blog/?p=450
	//if there is any GLArrayBufferObject associate with the attribute,
	//associate it with the attribute. It assumes that one attribute
	//is associated with one array buffer object. Structure of Array is assumed.
	if(m_type == GL_INT || m_type == GL_UNSIGNED_BYTE
       || m_type == GL_UNSIGNED_INT){
        //We need glVertexAttribIFormat( with I) to match your shader 
        //int/uint attribute.
		glVertexAttribIFormat(attrbIndex, m_nComponents, m_type, m_offset);
    }
	else if (m_type == GL_DOUBLE)
	{
		glVertexAttribLFormat(attrbIndex, m_nComponents, m_type, m_offset);
	}
    else{
	GLError::glCheckError(__func__);
		glVertexAttribFormat(attrbIndex, m_nComponents, m_type, m_normalized, m_offset);
    }
    GLint bindIdx = m_vbo->getBindingIndex();
	GLError::glCheckError(__func__);
    glVertexAttribBinding(attrbIndex, bindIdx);// m_vbo->getBindingIndex());
	GLError::glCheckError(__func__);
	glEnableVertexAttribArray(attrbIndex);
	GLError::glCheckError(__func__);
	glBindVertexBuffer(/*m_vbo->getBindingIndex()*/bindIdx, m_vbo->getId(), 0, m_stride);
	GLError::glCheckError(__func__);
    glVertexAttribDivisor(bindIdx, m_divisor); 
	GLError::glCheckError(__func__);
}
#endif

void GLAttribute::disable()
{
	GLint index=-1;
	index = glGetAttribLocation( m_shaderProgId, m_name.data());
	if (index==-1)
	{
		if (m_enableWarning)
		{
			std::stringstream ss;
			ss <<__func__<<": Attribute "<<m_name<<" is not active.\n";
			std::string msg=ss.str();
			cerr << msg;
		}
		std::stringstream ss2;
		ss2 <<__func__<<": Something weird happens while attempting to disable attribute "
			<< m_name <<". It's not found/active at current status and also triggers an OpenGL internal error."
			<<"You've probably declared one of GLVertexArrayObject as global GLVertexBufferObjectRef, "
			<<"try to use GLVertexBufferObject* or declare GLVertexBufferObjectRef inside a class or "
			<<"use GLVertexBufferObjectRef as global variable but use its compatibility mode to specify vertex format.\n";
		std::string msg2 = ss2.str();
		GLError::glCheckError(msg2.data());
	}else{
		std::stringstream ss;
		ss << __func__ <<": Disabling vertexAttribArray:"<<m_name<<" failed!\n";
		std::string msg = ss.str();

		glDisableVertexAttribArray(index);
		m_vbo->releaseBindingIndex();
		GLError::glCheckError(msg);
	}
}

}//end of namespace
