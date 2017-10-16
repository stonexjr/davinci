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
#include "GLSamplerObject.h"
#include "GLError.h"

namespace davinci{

GLSamplerObject::GLSamplerObject( GLint filter/*=GL_LINEAR*/, GLint edgeSampling/*=GL_CLAMP_TO_EDGE*/ )
{
	glGenSamplers(1, &m_samplerId);
	setFiltering(filter);
	setEdgeSampling(edgeSampling);
	GLError::glCheckError(__func__);
}

void GLSamplerObject::setFiltering( GLint filter )
{
	glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, filter);
	glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, filter);
}

void GLSamplerObject::setEdgeSampling( GLint sampling )
{
	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_S, sampling);
	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_T, sampling);
}
GLSamplerObject::~GLSamplerObject(void)
{
	if (m_samplerId){
		glDeleteSamplers(1, &m_samplerId);
		m_samplerId = 0;
	}
}
void GLSamplerObject::bind(GLuint texUnitId)
{
	glBindSampler(texUnitId, m_samplerId);
	GLError::glCheckError(__func__);
}

void GLSamplerObject::unbind(GLuint texUnitId)
{
	glBindSampler(texUnitId, 0);
	GLError::glCheckError(__func__);
}

void GLSamplerObject::setAnisotropicFiltering( float val )
{
	glSamplerParameteri(m_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, val);
}

void GLSamplerObject::setComparisionMode( GLint comp_fun )
{
	glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_MODE ,GL_COMPARE_REF_TO_TEXTURE);
	glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_FUNC, comp_fun);
}


}//end of namespace lily
