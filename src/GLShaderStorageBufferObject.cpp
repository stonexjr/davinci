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
#include "GLError.h"
#include "GLShaderStorageBufferObject.h"
//https://www.opengl.org/wiki/Shader_Storage_Buffer_Object
//https://www.opengl.org/wiki/Interface_Block_%28GLSL%29#Shader_storage_blocks
//http://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/

namespace davinci{

	/*
	GLShaderStorageBufferObject::GLShaderStorageBufferObject(void)
		:m_bindingPnt(-1), m_name("Untitled"),//, m_bActive(false),
		GLBufferObject(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW)
	{
	}

	GLShaderStorageBufferObject::GLShaderStorageBufferObject(GLuint bindPt, const char* name)
		: m_bindingPnt(bindPt), m_name(name),
		GLBufferObject(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW)
	{
	}
	*/

	
	GLShaderStorageBufferObject::GLShaderStorageBufferObject(
		  const std::string& name/*="Untitled SBBO"*/, GLenum bufferUsage/*=GL_STATIC_DRAW*/)
		: GLBufferObject(GL_SHADER_STORAGE_BUFFER, bufferUsage, name)
	{
		
	}

	/*
	void GLShaderStorageBufferObject::setBindingPoint(GLuint id)
	{
		m_bindingPnt = id;
		bindBufferObject();
		glBindBufferBase(m_target, m_bindingPnt, m_id);
		unbindBufferObject();
		stringstream ss;
		ss << __func__ << ": " << m_name 
		   << " connecting SSBO base to binding point failed!\n";
		GLError::glCheckError(ss.str());
	}

	GLuint GLShaderStorageBufferObject::getBinding() const
	{
		return m_bindingPnt;
	}
	*/
}
