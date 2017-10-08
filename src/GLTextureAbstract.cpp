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

#include "GLTextureAbstract.h"
#include "GLError.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

namespace davinci{

std::shared_ptr<std::vector<int> >
	GLTextureAbstract::g_availTexUnitIdsRef=
	std::shared_ptr<std::vector<int> >(new std::vector<int>);

std::shared_ptr<std::vector<int> >
	GLTextureAbstract::g_availImgUnitIdsRef=
	std::shared_ptr<std::vector<int> >(new std::vector<int>);

GLTextureAbstract::GLTextureAbstract(
		int w/*=16*/, int h/*=16*/,int d/*=16*/, GLenum target,// GLenum texUnitId/*=0*/,
		GLint internalformat/*=GL_RGBA8*/, GLint format/*=GL_RGBA*/,
		GLint type/*=GL_UNSIGNED_BYTE*/, GLint minFilter/*=GL_LINEAR*/,
		GLint magFilter/*=GL_LINEAR*/,GLint wrapMode/*=GL_CLAMP_TO_EDGE*/)

		:m_bUseAlpha(true),m_target(target),m_texId(0),m_width(w),m_height(h),
		 m_depth(d),m_internalformat(internalformat), m_format(format),
		 m_type(type),m_isBind(false),m_strName("Untitled Texture"),
		 m_texUnitId(-1),m_imgUnitId(-1),m_useFixedPipeline(false),m_minFilter(minFilter),
		 m_magFilter(magFilter),m_wrapMode(wrapMode)
{
	if (!g_availTexUnitIdsRef)
	{
		g_availTexUnitIdsRef = 
			std::shared_ptr<std::vector<int> >(new std::vector<int>);
	}
	m_availTexUnitIdsRef = g_availTexUnitIdsRef;
	if (!g_availImgUnitIdsRef)
	{
		g_availImgUnitIdsRef = 
			std::shared_ptr<std::vector<int> >(new std::vector<int>);
	}
	m_availImgUnitIdsRef = g_availImgUnitIdsRef;
	//supported from OpenGL 4.2 and above
	m_access = GL_READ_WRITE;
	m_layer = 0;
	m_level = 0;
	m_bLayered = false;
}

GLTextureAbstract::~GLTextureAbstract(void)
{
	
	deleteTexture();
}

GLint GLTextureAbstract::getTextureUnitId()
{
	if (m_texUnitId == -1)
	{
		m_texUnitId = getNextAvailabeTexUnitId(m_useFixedPipeline);
	}
	//if m_texUnitId is already allocated, simply return it.
	return m_texUnitId;
}

GLenum GLTextureAbstract::getNextAvailabeImageUnitId()
{
	static GLint MaxImageUnits=0;
	static GLint MaxTexBufferSize = 0;
	if (g_availImgUnitIdsRef->empty())
	{
		PRINT_GL_CAPABILITY(GL_MAX_IMAGE_UNITS, MaxImageUnits);
		PRINT_GL_CAPABILITY(GL_MAX_TEXTURE_BUFFER_SIZE, MaxTexBufferSize);
		printf("*** Max Image Units available (available since GL 2.0)=%d****\n",MaxImageUnits);
		printf("*** Max Texture Buffer size =%d texel ****\n",MaxTexBufferSize);
		//linear search the next available tex unit id.
		g_availImgUnitIdsRef->resize(MaxImageUnits, 1);
	}
	for (int i = 0; i < g_availImgUnitIdsRef->size(); i++)
	{
		if ((*g_availImgUnitIdsRef)[i])
		{
			(*g_availImgUnitIdsRef)[i] = 0;
			return i;
		}
	}
	std::stringstream ss;
	ss << __func__ << ": You're running out of image units (maximum image units=";
	ss << MaxImageUnits;
	std::string msg = ss.str();
	GLError::ErrorMessage(msg);
	return -1;
}

GLint GLTextureAbstract::getImageUnitId()
{
	if (m_imgUnitId == -1)
	{
		m_imgUnitId = getNextAvailabeImageUnitId();
	}
	return m_imgUnitId;
}
GLenum GLTextureAbstract::getNextAvailabeTexUnitId(bool useFixedPiple/*=false*/)
{
	static GLint MaxTextureImageUnits=0;
	static GLint MaxTextureUnits=0;
	static GLint MaxTextureCoords=0;
	static GLint MaxCombinedTextureImageUnits=0;
	static GLint MaxVertexTextureImageUnits=0;
	static GLint MaxGSGeometryTextureImageUnits=0;

	if (g_availTexUnitIdsRef->empty())
	{//access m_availIds for the first time. Initialize it.
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureImageUnits);
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &MaxTextureUnits);
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &MaxTextureCoords);
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &MaxVertexTextureImageUnits);
		glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &MaxGSGeometryTextureImageUnits);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxCombinedTextureImageUnits);
		printf("*** Max Texture Image Units in fragment shader (available since GL 2.0)=%d****\n",MaxTextureImageUnits);
		printf("*** Max Texture Units in vertex shader (available since GL 2.0)=%d ***\n", MaxVertexTextureImageUnits);
		printf("*** Max Texture Units in geometry shader (available since GL 3.2)=%d ***\n", MaxGSGeometryTextureImageUnits);
		printf("*** Max Combined Texture image units(VS + GS + FS (available since GL 2.0))=%d****\n",MaxCombinedTextureImageUnits);
		printf("*** Max Conventional Texture Units(for the fixed pipeline which is deprecated)=%d****\n",MaxTextureUnits);
		printf("*** Max Texture Coords=%d****\n",MaxTextureCoords);
		//linear search the next available tex unit id.
		g_availTexUnitIdsRef->resize(MaxTextureImageUnits, 1);
	}
	int i = useFixedPiple ? 0 : MaxTextureUnits;
	//texture units [0, MaxTextureUnits-1] is intended to be reserved for 
	//fixed pipeline.
	for ( ; i < g_availTexUnitIdsRef->size() ; i++)
	{
		if ((*g_availTexUnitIdsRef)[i])
		{
			(*g_availTexUnitIdsRef)[i]=0;
			if (!useFixedPiple 
			  || (useFixedPiple && i < MaxTextureUnits)
			  )
			{
			   return i;
			}else{
				break;
			}
		}
	}
	std::stringstream ss;
	if (!useFixedPiple)
	{
		ss << __func__<<": You're running out of texture image id (maximum texture image ids=";
		ss << MaxTextureImageUnits;
	}else{
		ss << __func__<<": You're running out of fixed pipeline texture image id (maximum texture units=";
		ss << MaxTextureUnits;
		ss << "Try to use core version and avoid using glMultiTexCoordx(), and call getNextAvailabeTexUnitId(false).\n";
	}
	std::string msg=ss.str();
	GLError::ErrorMessage(msg);
	return -1;
}

void GLTextureAbstract::releaseTexUnitId()
{
	if(m_texUnitId == -1) return;//no need to release multiple times.

	if (m_texUnitId < (*g_availTexUnitIdsRef).size() )
	{
		if(!(*g_availTexUnitIdsRef)[m_texUnitId])
		{
			(*g_availTexUnitIdsRef)[m_texUnitId]=1;
			m_texUnitId = -1;
		}
		else{
			std::stringstream ss;
			ss << __func__
				<<": There is inconsistency at texture "<<m_strName
				<<". \nMaybe some of your GLTexture creation does not use "
				<<"GLTexture::getNextAvailabeTexUnitId() to get texture unit id."
				<<m_texUnitId<<std::endl;
			std::string msg=ss.str();
			std::cerr << msg;
			GLError::ErrorMessage(msg);
		}
	}
	else if(g_availTexUnitIdsRef->size() > 0){
		std::stringstream ss;
		ss << __func__<< "m_texUnitId(" << m_texUnitId
				  << ") out of bound.\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}else{
		std::stringstream ss;
		ss << __func__ << ": Release texture unit id("<<m_texUnitId
		   << ") failed! g_availTexUnitIdsRef.size()=="<<g_availTexUnitIdsRef->size()
		   << ". Please try to create GLTextureXD as GLTextureXDRef.\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}

}

void GLTextureAbstract::releaseImgUnitId()
{
	if (m_imgUnitId == -1) return;//no need to release multiple times.
	if (m_imgUnitId < (*g_availImgUnitIdsRef).size())
	{
		if (!(*g_availImgUnitIdsRef)[m_imgUnitId])
		{
			(*g_availImgUnitIdsRef)[m_imgUnitId] = 1;
			m_imgUnitId = -1;
		}
		else{
			std::stringstream ss;
			ss << __func__
				<< ": There is inconsistency at texture image " << m_strName
				<< ". \nMaybe some of your GLTexture image creation does not use "
				<< "GLTexture::getNextAvailabeImgUnitId() to get image unit id."
				<< m_imgUnitId  << std::endl;
			std::string msg = ss.str();
			std::cerr << msg;
			GLError::ErrorMessage(msg);
		}
	}
	else if (g_availImgUnitIdsRef->size() > 0){
		std::stringstream ss;
		ss << __func__ << "m_imgUnitId(" << m_imgUnitId
			<< ") out of bound.\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}
	else{
		std::stringstream ss;
		ss << __func__ << ": Release image unit id(" << m_imgUnitId
			<< ") failed! g_availImgUnitIdsRef.size()==" << g_availImgUnitIdsRef->size()
			<< ". Please try to create GLTextureXD as GLTextureXDRef.\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}
}

void GLTextureAbstract::deleteTexture()
{
	if (m_texId){
		/*TODO need to figure out how to use glMakeImageHandleResident()
		if (isImageHandleResident())
		{
		bindlessMakeImageResident(false);
		}
		*/
		if (isTextureHandleResident()){
			bindlessMakeTextureResident(false);
		}

		glDeleteTextures(1, &m_texId);
		m_texId = 0;
		releaseTexUnitId();
	}
}

void GLTextureAbstract::bindTexture(GLenum mode/*=GL_MODULATE*/)
{
	if(m_texUnitId!=-1)
	{
		std::stringstream ss;
		ss << __func__ <<":"<<m_strName<< ": m_texUnitId != -1"
			<< "You probably have forget calling unbind() before calling bind()\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}
	m_texUnitId = getTextureUnitId();
	glActiveTexture(GL_TEXTURE0+m_texUnitId);
	glBindTexture(m_target, m_texId);
	//glEnable(m_target);//glEnable(GL_TEXTUREXD); deprecated in core profile.
#if !defined(__APPLE__) || !defined(MACOSX)
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);//GL_DECAL
#endif
	if (m_samplerObj)
	{
		m_samplerObj->bind(m_texUnitId);
	}
	std::stringstream ss;
	ss <<m_strName <<":"<<__func__;
	GLError::glCheckError(ss.str());
	m_isBind = true;
}

void GLTextureAbstract::unbindTexture()
{
	if(m_texUnitId==-1)
	{//simply return if already released.
		return;
		/*
		std::stringstream ss;
		ss << __func__ << ": m_texUnitId==-1"
		   << "Please call unbind() before releaseTexUnitId()\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
		*/
	}

	glActiveTexture(GL_TEXTURE0+m_texUnitId);
	glBindTexture(m_target, 0);
	if (m_samplerObj)
	{
		m_samplerObj->unbind(m_texUnitId);
	}
	//glDisable(m_target);//deprecated in core profile
	GLError::glCheckError(__func__);
	m_isBind = false;

	releaseTexUnitId();
}

bool GLTextureAbstract::useMipmap() const
{
	return  m_minFilter==GL_NEAREST_MIPMAP_NEAREST||//(use the nearest neighbor in the nearest mipmap level)
			m_minFilter==GL_NEAREST_MIPMAP_LINEAR ||//(linearly interpolate in the nearest mipmap level)
			m_minFilter==GL_LINEAR_MIPMAP_NEAREST ||//(use the nearest neighbor after linearly interpolating between mipmap levels)
			m_minFilter==GL_LINEAR_MIPMAP_LINEAR;  //(linearly interpolate both the mipmap levels and at between texels)
}

void GLTextureAbstract::bindImage(GLenum access, bool bLayered/*=false*/, GLint layer/*=0*/, GLint level/*=0*/)
{
	m_access = access;
	m_layer  = layer;
	m_level  = level;
	m_bLayered = bLayered;
	if (m_imgUnitId != -1)
	{
		std::stringstream ss;
		ss << __func__ <<":"<<m_strName<< ": m_imgUnitId != -1"
			<< "You probably have forget calling unbind() before calling bind()\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}
	m_imgUnitId = getImageUnitId();
	glBindImageTexture(m_imgUnitId, m_texId, m_level, m_bLayered, m_layer, m_access, m_internalformat);
	std::stringstream ss;
	ss << m_strName << ":" << __func__;
	GLError::glCheckError(ss.str());
	m_isBind = true;
}

void GLTextureAbstract::bindImage()
{
	bindImage(m_access, m_bLayered, m_layer, m_level);
}

void GLTextureAbstract::unbindImage()
{
	if (m_imgUnitId == -1)
	{//simply return if already released.
		return;
	}

	glBindImageTexture(m_imgUnitId, 0, m_level, m_bLayered, m_layer, m_access, m_internalformat);
	
    GLError::glCheckError(std::string(__func__)+std::string(": glBindImageTexture() failed!\n"));
	//glDisable(m_target);
	//GLError::glCheckError(__func__":glDisable(m_target) failed!\n");
	m_isBind = false;

	releaseImgUnitId();
}

uint64_t GLTextureAbstract::getBindlessTextureHandle() const
{
	uint64_t handle = glGetTextureHandleNV(m_texId);
	GLError::glCheckError(std::string(__func__)+":failed");
	return handle;
}

uint64_t GLTextureAbstract::getBindlessSamplerHandle() const
{
	uint64_t handle = -1ULL;
	if (m_samplerObj)
	{
		uint64_t handle = glGetTextureSamplerHandleNV(m_texId, m_samplerObj->getSamplerId());
	}else{
		std::stringstream ss;
		ss << __func__ << ": m_samplerObj is NULL!\n";
		GLError::ErrorMessage(ss.str());
	}
	return handle;

}

void GLTextureAbstract::bindlessMakeTextureResident(bool v)
{
	uint64_t handle = getBindlessTextureHandle();
	if (v)
	{
		glMakeTextureHandleResidentNV(handle);
	}else{
		glMakeTextureHandleNonResidentNV(handle);
	}
	GLError::glCheckError(string(__func__)+":failed");
}

uint64_t GLTextureAbstract::getBindlessImageHandle() const
{
	uint64_t handle = glGetImageHandleNV(m_texId,m_level,m_bLayered,m_layer,m_format);
	GLError::glCheckError(string(__func__)+":failed");
	return handle;
}

void GLTextureAbstract::bindlessMakeImageResident(bool v)
{
	uint64_t handle = getBindlessImageHandle();
	if (v)
	{
		glMakeImageHandleResidentNV(handle,m_access);
	}else{
		glMakeImageHandleNonResidentNV(handle);
	}
	GLError::glCheckError(string(__func__)+":failed");
}

bool GLTextureAbstract::isTextureHandleResident() const
{
	uint64_t handle = getBindlessTextureHandle();
	return glIsTextureHandleResidentNV(handle);
}

bool GLTextureAbstract::isImageHandleResident() const
{
	uint64_t handle = getBindlessImageHandle();
	return glIsImageHandleResidentNV(handle);
}

}//end of namespace lily
