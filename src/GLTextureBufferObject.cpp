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

#include <iostream>
#include <sstream>
#include "GLTextureBufferObject.h"
#include "GLError.h"
using namespace davinci;
using namespace std;

GLTextureBufferObject::GLTextureBufferObject(
	GLenum internalFormat,
	GLenum usage/*=GL_DYNAMIC_DRAW*/)
	:GLBufferObject(GL_TEXTURE_BUFFER, usage), 
	 GLTextureAbstract(1, 1, 1, GL_TEXTURE_BUFFER,internalFormat)
{
	glGenTextures(1, &m_texId);
}

GLTextureBufferObject::~GLTextureBufferObject(void)
{
	deleteBuffer();
}

void GLTextureBufferObject::upload(GLsizeiptr totalSizeInBytes, GLsizeiptr texelCount, const GLvoid* data)
{
	static int maxTexelsAllowed = 0;
	if (maxTexelsAllowed==0)
	{
		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxTexelsAllowed);
	}
	if (texelCount > maxTexelsAllowed)
	{
		stringstream ss;
		ss << __func__ << ": request texel count =" <<texelCount 
			<< " > GL_MAX_TEXTURE_BUFFER_SIZE=" << maxTexelsAllowed << endl;
		GLError::ErrorMessage(ss.str());
	}
	GLBufferObject::upload(totalSizeInBytes, data);
	//map the buffer object to the texture.
	//so the texture content will come from the buffer object storage.
	glBindTexture(GL_TEXTURE_BUFFER, m_texId);
	glTexBuffer(GL_TEXTURE_BUFFER, getInternalFormat(), m_id);
	GLError::glCheckError(string(__func__)+":glTexBuffer() failed!");
	glBindTexture(GL_TEXTURE_BUFFER, 0);
}

void GLTextureBufferObject::deleteBuffer()
{
	GLTextureAbstract::deleteTexture();
	GLBufferObject::deleteBuffer();
}

void davinci::GLTextureBufferObject::bindImage()
{
	GLTextureAbstract::bindImage();
}

void davinci::GLTextureBufferObject::unbindImage()
{
	//GLTextureAbstract::unbindImage();//don't call, because glDisable(GL_TEXTURE_BUFFER) is invalid.
	glBindImageTexture(m_imgUnitId, 0, m_level, m_bLayered, m_layer, m_access, m_internalformat);
	//glDisable(getTarget());//glDisable(GL_TEXTURE_BUFFER) is invalid enumeration
	GLError::glCheckError(__func__);
	m_isBind = false;

	releaseImgUnitId();
}

void davinci::GLTextureBufferObject::bindTexture(GLenum mode /*= GL_MODULATE*/)
{
	//GLTextureAbstract::bind();
	if (m_texUnitId != -1)
	{
		std::stringstream ss;
		ss << __func__ << ":" << m_strName << ": m_texUnitId != -1"
			<< "You probably have forget calling unbind() before calling bind()\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}
	m_texUnitId = getTextureUnitId();
	glActiveTexture(GL_TEXTURE0 + m_texUnitId);
	glBindTexture(GLTextureAbstract::getTarget(), m_texId);
	std::stringstream ss;
	ss << m_strName << ":" << __func__;
	GLError::glCheckError(ss.str());
	m_isBind = true;
}

void davinci::GLTextureBufferObject::unbindTexture()
{
	//GLTextureAbstract::unbind()
	if (m_texUnitId == -1)
	{//simply return if already released.
		return;
	}

	glActiveTexture(GL_TEXTURE0 + m_texUnitId);
	glBindTexture(GLTextureAbstract::getTarget(), 0);
	if (m_samplerObj)
	{
		m_samplerObj->unbind(m_texUnitId);
	}
	GLError::glCheckError(__func__);
	m_isBind = false;

	releaseTexUnitId();
}
