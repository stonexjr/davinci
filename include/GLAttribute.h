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

//AUTHOR: Jinrong Xie (stonexjr at gmail.com)
//CREATED: 2013-10-23
//UPDATED: 2015-06-13
#ifndef _GL_ATTRIBUTE_H_
#define _GL_ATTRIBUTE_H_

#include <string>
#include <memory>
#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include "GLVertexBufferObject.h"

using namespace std;

namespace davinci{
	 
class GLAttribute
{
public:
	friend class GLVertexArrayObject;
	//Vertex attribute descriptor, the actual data is stored in the associated GLArrayBufferObject.
	//GLAttribute describes the format( size,type, normalized, offset) of the vertex attribute 
	//it represents. You can think of it as a bridge the connect one attribute variable in GLSL 
	//Vertex Shader and the actual physical buffer that stores the content of those attributes.
	//param name: attribute variable name that appears in the vertex shader.
	//param vbo: An associated instance of GLArrayBufferObject corresponding to the GLSL attribute.
	//param size: Determines the number of components per vertex allocated to the current
    //must be 1, 2, 3 or 4.
	//attribute and must be 1, 2, 3 or 4. 
	//param type: Indicates the type of the data. Following table map the available GL type
	//to the actual data type:
	//---------------------------------
	//| type    | physical data type  |
	//---------------------------------
	// GL_BYTE  | GLbyte
	// GL_SHORT | GLshort
	// GL_INT   | GLint
	// GL_FIXED | GLfixed
	// GL_FLOAT | GLfloat
	// GL_HALF_FLOAT | GLhalf
	// GL_DOUBLE | GLdouble
	// GL_UNSIGNED_BYTE | GLubyte
	// GL_UNSIGNED_SHORT| GLushort
	// GL_UNSIGNED_INT  | GLuint
	//
	// param stride: Amount of bytes required to get from one vertex to the next.
	// param offset: It could be the offset into a VBO where the attribute starts.
	//              or it could be the actual pointer to the attribute array on the client side.
	// param normalized: Indicate if the data will be normalized in the pipeline.
	GLAttribute(GLuint shaderProgId, const string& name,GLenum type,
				GLuint nComponets,  GLuint stride=0, GLuint offset=0,
				bool normalized=false, bool warning=false, 
				const GLVertexBufferObjectRef vbo=GLVertexBufferObjectRef((GLVertexBufferObject*)NULL));//GLenum bindingIndex,);

	GLAttribute(GLuint shaderProgId, const string& name,GLenum type,
				GLuint nComponets,  GLuint stride=0, GLuint offset=0,
				bool normalized=false, bool warning=false, GLuint divsor = 0,
				const GLVertexBufferObjectRef vbo=GLVertexBufferObjectRef((GLVertexBufferObject*)NULL));//GLenum bindingIndex,);
	~GLAttribute(void);

	void attach(const GLVertexBufferObjectRef vbo){m_vbo = vbo;}
	GLVertexBufferObjectRef getAttachedVBO(void) { return m_vbo;}
	//void setBindingIndex(GLenum idx){m_bindingIndex=idx;}
	//GLenum getBindingIndex(){ return m_bindingIndex;}
	//bind the attribute to a variable named m_name in shader program specified
	//by programId. User is responsible to prepare, compile and link shader program
	//prior the usage of GLAttribute::bind().
	void enable();
	void disable();

    void setActive(bool v){ m_bActive = v; }
    bool isActive() const { return m_bActive; }

	GLuint getShaderProgId() const { return m_shaderProgId; }
	void   setShaderProgId( GLuint progId ) { m_shaderProgId = progId;}
	GLenum getType() const { return m_type; }
	void   setType(GLenum type) { m_type = type; }
	std::string getName() const { return m_name; }
	void   setName(std::string val) { m_name = val; }
	GLuint getComponents() const { return m_nComponents; }
	void   setComponents(GLuint val) { m_nComponents = val; }
	GLuint getStride() const { return m_stride; }
	void   setStride(GLuint val) { m_stride = val; }

private:
    bool m_bActive;//if attribute is active in the shader.
	GLVertexBufferObjectRef m_vbo;//The array buffer associated with the attribute.
	string m_name;//string representation of attribute name
	//GLenum m_bindingIndex;//binding index used to connect GLSL attribute index with vertex buffer data	// .
	GLenum m_type;//GL data type, i.e. GL_FLOAT, GL_UNSIGNED_INT.
	GLuint m_nComponents;//# of components(e.g floats or ints) of the attribute
	GLuint m_stride;//amount of bytes required to get from one vertex to the next
	GLuint m_shaderProgId;//The shader program id where current attribute is defined.
	//If you bind GLAttribute as VAO, m_offset is the offset into a 
	//currently bound buffer object.
	GLuint m_offset;
    //modify the rate at which generic vertex attributes advance during instanced rendering
    //https://www.opengl.org/sdk/docs/man/html/glVertexAttribDivisor.xhtml
    GLuint m_divisor;
	bool   m_normalized;
	bool   m_enableWarning;
};

typedef std::shared_ptr<GLAttribute> GLAttributeRef;

}//end of namespace 
#endif
