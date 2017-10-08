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

#include "GLTexture1D.h"
#include "GLError.h"
#include <sstream>
#include <iostream>

namespace davinci{

GLTexture1d::GLTexture1d(
	int w/*=16*/, //GLenum texUnitId/*=0*/,
	GLint internalformat/*=GL_RGBA8*/, GLint format/*=GL_RGBA*/,
	GLint type/*=GL_UNSIGNED_BYTE*/,const GLvoid* pixelData/*=NULL*/)
	:GLTextureAbstract(w,1,1,GL_TEXTURE_1D,/*texUnitId,*/internalformat,format,type)
//    :m_bUseAlpha(true),m_texId(0),m_width(w),m_height(h),m_texUnitId(texUnitId)
//    ,m_internalformat(internalformat), m_format(format),m_type(type)
{
	//glActiveTexture(GL_TEXTURE0+m_texUnitId);
	/*
	GLint id=0;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &id);
	std::cout<<"---GL_TEXTURE_1D----GLTexture"<<id-GL_TEXTURE0<<" activated!-----------\n";
	*/
	glGenTextures(1, &m_texId);
	glBindTexture(m_target,m_texId);
		//bind();
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//What this does is sets the active texture to GL_MODULATE. 
		//The GL_MODULATE attribute allows you to apply effects such as lighting
		//and coloring to your texture. If you do not want lighting and coloring
		//to effect your texture and you would like to display the texture unchanged
		//when coloring is applied replace GL_MODULATE with GL_DECAL. 
		//However, glTexEnvf is deprecated in GL 3.3 Core profile and higher.
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL

		glTexImage1D(GL_TEXTURE_1D, 0, m_internalformat, m_width,
					 0, m_format, m_type, pixelData);
		//unbind();
	glBindTexture(GL_TEXTURE_1D, 0);
	//Deprecated in GL 3.3 Core profile and higher.
	//glDisable(GL_TEXTURE_1D);
	GLError::glCheckError(__func__);
}

GLTexture1d::~GLTexture1d(void)
{
}

void GLTexture1d::resize(int w)
{
	//deleteTexture();
	m_width = w;

	//glActiveTexture(m_texChannelId);
	//glGenTextures(1, &m_texId);
	bindTexture();
	//Reallocate GPU texture memory
	glTexImage1D(GL_TEXTURE_1D, 0, m_internalformat, m_width,
				 0, m_format, m_type, NULL);
	unbindTexture();
	GLError::glCheckError(__func__);
}

void GLTexture1d::upload( int w, const GLvoid *data )
{
	if(getWidth()<w)
		resize(w);

	bindTexture();
		//glTexImage1D(GL_TEXTURE_1D, 0, m_internalformat, w, 0, m_format,
		//			 m_type, data);
		glTexSubImage1D(GL_TEXTURE_1D, 0, 0, w, m_format, m_type, data);
	unbindTexture();
}

void GLTexture1d::setWrapMode( GLint mode )
{
	glBindTexture(m_target,m_texId);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, mode );
	glBindTexture(m_target,0);
}

}//end of namespace lily
