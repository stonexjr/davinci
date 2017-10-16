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
//CREATED: 2015-03-08
//UPDATED: 2017-10-15
#ifndef _GL_TextureBufferObject_H_
#define _GL_TextureBufferObject_H_
#pragma once
#include "GLBufferObject.h"
#include "GLTextureAbstract.h"

namespace davinci{
//GLTextureBufferObject can be either bind to image unit, using GLShader::SetImageUniform()
//in glsl, it maps to imageBuffer type, use imageLoad/Store to read/write data.
//OR can be binded to sampler/texture unit, using GLShader::SetSamplerUniform()
//in glsl, it maps to samplerBuffer type, use texelFetch() to load data, which is readonly.
//See https://www.opengl.org/wiki/Buffer_Texture

class GLTextureBufferObject :
				public GLBufferObject, public GLTextureAbstract
{
public:
	//internalFormat: GL_R32F, GLRGBA32F, etc.
	//bufferUsage: GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
	GLTextureBufferObject(GLenum internalFormat, GLenum bufferUsage=GL_DYNAMIC_DRAW);
	~GLTextureBufferObject(void);
	void deleteBuffer();
	void upload(GLsizeiptr totalSizeInBytes, GLsizeiptr texelCount, const GLvoid* data);
	virtual void bindTexture();
	virtual void unbindTexture();
	virtual void bindImage();
	virtual void unbindImage();

private://disable the method inherit from GLTextureAbstract
	void   setUseFixedPipeline(bool v){ m_useFixedPipeline = v; }
	bool   useFixedPipeline() const { return m_useFixedPipeline; }
	bool   useMipmap() const;
	void   setSamplerObject(const GLSamplerObjectRef& sampler){ m_samplerObj = sampler; }
	//GLint  getTextureUnitId();// const { return m_texUnitId;}
	static GLenum getNextAvailabeTexUnitId(bool useFixedPiple = false);
	GLint  getFormat() const{ return m_format; }
	GLint  getType() const{ return m_type; }
	GLint  getWrapMode() const { return m_wrapMode; }
	GLint  getMinFilter() const{ return m_minFilter; }
	GLint  getMagFilter() const{ return m_magFilter; }
	GLuint getWidth() const{ return m_width; }
	GLuint getHeight() const{ return m_height; }
	GLuint getDepth() const{ return m_depth; }
};

typedef std::shared_ptr<GLTextureBufferObject> GLTextureBufferObjectRef;

}
#endif
