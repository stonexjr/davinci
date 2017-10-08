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
//CREATED: 2013-11-18
//UPDATED: 2013-11-22

#ifndef _GL_SAMPLEROBJECT_H_
#define  _GL_SAMPLEROBJECT_H_

#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include <memory>

namespace davinci{

	class GLSamplerObject
	{
		public:
			//Sampler Object, introduced in OpenGL 3.3, separates sampler state from texture data
			//http://www.opengl.org/wiki/Sampler_Object
			//https://www.opengl.org/wiki/Sampler_%28GLSL%29
			//GLSamplerObject( GLuint texUnitId=0);
			//filter:GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_NEAREST
			//GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR.
			//edgeSampling: GL_REAPT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT
			GLSamplerObject(GLint filter=GL_LINEAR, GLint edgeSampling=GL_CLAMP_TO_EDGE);
			~GLSamplerObject(void);
			void setFiltering(GLint filter);
			void setEdgeSampling(GLint sampling);
			void setAnisotropicFiltering(float val);
			//comp_fun: GL_ALWAYS,GL_NEVER,GL_LESS,GL_LEQUAL,GL_GREATER...
			void setComparisionMode(GLint comp_fun);
			//TODO
			//set LOD range/bias, Boader Color,Seamless cubemap fetching
			//texUnitId=0 <=> GL_TEXTURE_0
			//texUnitId=1 <=> GL_TEXTURE_1
			//...
			void   bind(GLuint texUnitId);
			void   unbind(GLuint texUnitId);
			
			//Returns i if the texture is create with 
			//glActiveTexture(GL_TEXTUREi)
			GLuint getSamplerId() const { return m_samplerId;}

		private:
			GLuint  m_samplerId;

	};
	typedef std::shared_ptr<GLSamplerObject> GLSamplerObjectRef;
}
#endif