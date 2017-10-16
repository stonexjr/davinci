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
//CREATED: 2014-01-23
//UPDATED: 2017-10-15

#ifndef _GL_TEXUTRE_ABSTRACTION_H
#define  _GL_TEXUTRE_ABSTRACTION_H

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <memory>
#include <vector>
#include <string>

#include "GLSamplerObject.h"

namespace davinci{
	class GLTextureAbstract
	{
		friend class GLTexture1d;
		friend class GLTexture2d;
		friend class GLTexture3d;
		friend class GLTextureBufferObject;
		friend class GLTextureCubeMap;
		//Abstract class of GLTexture that store all the common status
		//used by GLTexture1d,GLTexture2d and GLTexture3d.
		// using predefined pixel data specified by the
		//parameter pixelData. By default it is NULL. if pixelData is NULL
		// the content of the texture is undefined.
		//param target: GL_TEXTURE_1D,GL_TEXTURE_2D,GL_TEXTURE_3D.
		//param texUnitId: 0<==>GL_TEXTURE0,1<==>GL_TEXTURE1, ..,GL_TEXTURE31
		//param internalformat:
		//Named formats: GL_RGB, GL_RGBA(default), GL_DEPTH_COMPONENT32, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_INTENSITY and GL_ALPHA.
		//You can specify sized representations of these internalformats,
		//e.g. GL_RGBA8(default),GL_RGBA32F_ARB,GL_RGB8,GL_RGBA16F_ARB.
		//This is a hint to the implementation, and indicates the desired color resolution of the texture.
		//For more information please refer to https://www.opengl.org/wiki/GLAPI/glTexImage2D
		//param format:
		//specify how many color components. eg. GL_RGB,GL_RGBA(default),GL_DEPTH_COMPONENT.
		//Used for performing a Pixel Transfer operation.
		//param type:
		//the data type of each color components. eg. GL_UNSIGNED_BYTE(default) GL_FLOAT
		//Used for performing a Pixel Transfer operation.
	protected:
		GLTextureAbstract(int w, int h, int d, GLenum target,//e.g. GL_TEXTURE_1D/2D/3D etc.
			GLint internalformat=GL_RGBA8, GLint format=GL_RGBA,
			GLint type=GL_UNSIGNED_BYTE, GLint minFilter=GL_LINEAR,
			GLint magFilter=GL_LINEAR, GLint wrapMode=GL_CLAMP_TO_EDGE);

		virtual ~GLTextureAbstract(void);
		//param mode:
		//What this does is sets the texture mode of active texture to either
		//GL_MODULATE or GL_DECAL.
		//The GL_MODULATE attribute allows you to apply effects such as lighting
		//and coloring to your texture. If you do not want lighting and coloring
		//to effect your texture and you simply would like to display the raw 
		//texture pixel, please replace GL_MODULATE with GL_DECAL. 
	public:
        //If you are using fix pipeline of OpenGL for texture mapping. Make sure
        //to call glEnable(GL_TEXTURE_XD) and glDisable(GL_TEXTURE_XD) before or after bindTexture() respectively;
		virtual void   bindTexture();
        //If you are using fix pipeline of OpenGL for texture mapping. Make sure
        //to call glDisable(GL_TEXTURE_XD) before or after callling unbindTexture();
		virtual void   unbindTexture();
		bool   isBind() const{ return m_isBind;}
		std::string getName() const{ return m_strName;}
		void   setName(const std::string& name){ m_strName = name;}
		void   setUseFixedPipeline(bool v){ m_useFixedPipeline = v;}

		bool   useFixedPipeline() const { return m_useFixedPipeline; }
		bool   useMipmap() const;
		void   deleteTexture();
		void   releaseTexUnitId();
		void   releaseImgUnitId();
		void   setSamplerObject(const GLSamplerObjectRef& sampler){ m_samplerObj = sampler;}
		GLuint getTextureId() const {return m_texId;}
		//Returns i if the texture is create with 
		//glActiveTexture(GL_TEXTUREi)
		GLint  getTextureUnitId();// const { return m_texUnitId;}
		GLint  getImageUnitId();
		//Note: If you're using glMultiTexCoordxf(), the maximum available texture
		//channels is 4.
		// http://www.opengl.org/wiki/Textures_-_more
		static GLenum getNextAvailabeTexUnitId(bool useFixedPiple=false);
		/*
		Note that image units used for image variables are independent of
		the texture image units used for sampler variables; the number of
		units provided by the implementation may differ.
		Textures are bound independently and separately to image and texture
		image units.The error INVALID_VALUE is generated if a
		Uniform1i{v} call is used to set an image uniform to a value less than
		zero or greater than or equal to MAX_IMAGE_UNITS
		Ref: https://www.opengl.org/registry/specs/ARB/shader_image_load_store.txt
		*/
		static GLenum getNextAvailabeImageUnitId();
		//Returns the texture target like GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D.
		GLenum getTarget() const{ return m_target;}
		//Returns GL pixel internal format like: GL_RGBA8, GL_RGB, etc.
		GLint  getInternalFormat() const{ return m_internalformat;}
		//Returns GL pixel internal format like: GL_RGBA, GL_RGB, etc.
		GLint  getFormat() const{ return m_format;}
		//return GL type like: GL_FLOAT, GL_UNSIGNED_BYTE, etc.
		GLint  getType() const{ return m_type;}
		GLint  getWrapMode() const { return m_wrapMode;}
		GLint  getMinFilter() const{ return m_minFilter;}
		GLint  getMagFilter() const{ return m_magFilter;}
		//Return the width of the texture in pixel.
		GLuint getWidth() const{ return m_width;}
		//Return the height of the texture in pixel.
		GLuint getHeight() const{ return m_height;}
		//Return the depth of the texture in pixel.
		GLuint getDepth() const{ return m_depth;}

		//Texture Image related methods
		//supported from OpenGL 4.2 and above.
		void   setImageAccess(GLenum access){ m_access = access; }
		//bind texture as image (supported from OpenGL 4.2 and above)
		//access: GL_READ_ONLY, GL_WRITE_ONLY and GL_READ_WRITE.
		//https://www.opengl.org/sdk/docs/man/html/glBindImageTexture.xhtml
		//When 3D texture is used, it has to be bound as layered, i.e. bLayered=true,
		//otherwise the shader isn't able to modify any uvw coordinate with w>0.
		//http://stackoverflow.com/questions/17015132/compute-shader-not-modifying-3d-texture
		//
		void   bindImage(GLenum access, bool bLayered=false, GLint layer=0, GLint level=0);
		virtual void   bindImage();
		virtual void   unbindImage();
		void   setImageLayer(GLint layer){ m_layer = layer; }
		void   setImageLevel(GLint level){ m_level = level; }
		//call this when 3d texture is used.
		void   setImageIsLayered(bool v){ m_bLayered = v;}
		void   isLayered(bool v){ m_bLayered = v; }

		//For bind less texture feature.
		//https://www.opengl.org/registry/specs/NV/bindless_texture.txt
		uint64_t getBindlessTextureHandle() const;
		uint64_t getBindlessSamplerHandle() const;
		//v: true/false make texture handle resident or non-resident.
		void bindlessMakeTextureResident(bool v);
		uint64_t getBindlessImageHandle() const;
		void bindlessMakeImageResident(bool v);
		bool isTextureHandleResident() const;
		bool isImageHandleResident() const;
		/*
		uint64 GetTextureHandleNV(uint texture);
		uint64 GetTextureSamplerHandleNV(uint texture, uint sampler);
		void MakeTextureHandleResidentNV(uint64 handle);
		void MakeTextureHandleNonResidentNV(uint64 handle);
		uint64 GetImageHandleNV(uint texture, int level, boolean layered,
			int layer, enum format);
		void MakeImageHandleResidentNV(uint64 handle, enum access);
		void MakeImageHandleNonResidentNV(uint64 handle);

		void UniformHandleui64NV(int location, uint64 value);
		void UniformHandleui64vNV(int location, sizei count, const uint64 *value);
		void ProgramUniformHandleui64NV(uint program, int location,
			uint64 value);
		void ProgramUniformHandleui64vNV(uint program, int location,
			sizei count, const uint64 *values);

		boolean IsTextureHandleResidentNV(uint64 handle);
		boolean IsImageHandleResidentNV(uint64 handle);
		*/
	private:
		//Entry i indicate the availability of texture unit id i. 
		//1: available, 0: taken.
		static std::shared_ptr<std::vector<int> > g_availTexUnitIdsRef;
		//Entry i indicate the availability of image unit id i. 
		//1: available, 0: taken.
		static std::shared_ptr<std::vector<int> > g_availImgUnitIdsRef;
		//Each instance of GLTextureAbstract(or its subclass) has 
		//reference to the global static variable g_availIdsRef such
		//that g_availIdRef is guaranteed the be the last one to be
		//released.
		std::shared_ptr<std::vector<int> > m_availTexUnitIdsRef;
		std::shared_ptr<std::vector<int> > m_availImgUnitIdsRef;

		std::string m_strName;//name of the texture, for ease of error identification.
		GLenum  m_target;//GL_TEXTURE_1D,GL_TEXTURE_2D,GL_TEXTURE_3D
		GLuint  m_texId;
		GLint   m_texUnitId;
		//Textures are bound independently and separately to image and texture image units.T
		GLint   m_imgUnitId;
		bool    m_bUseAlpha;
		bool    m_isBind;
		bool    m_useFixedPipeline;
		GLuint  m_width, m_height, m_depth;
		GLint   m_internalformat;
		GLint   m_format;
		GLint   m_type;
		GLint   m_minFilter, m_magFilter;
		//https://www.opengl.org/sdk/docs/man2/xhtml/glTexParameter.xml
		//GL_CLAMP, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, or GL_REPEAT
		GLint   m_wrapMode;
		GLSamplerObjectRef m_samplerObj;
		//Texture Image attributes supported from OpenGL 4.2 and above
		GLenum  m_access;
		GLenum  m_layer;
		GLenum  m_level;
		bool    m_bLayered;
	};

	typedef std::shared_ptr<GLTextureAbstract> GLTextureAbstractRef;

}

#endif
