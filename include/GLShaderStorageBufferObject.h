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
//CREATED: 2015-04-16
//UPDATED: 2017-10-15

#ifndef _GLSHADERSTORAGEBUFFEROBJECT_H_
#define _GLSHADERSTORAGEBUFFEROBJECT_H_
#include <memory>
#include <string>
#include "vec2f.h"
#include "vec3d.h"
#include "mat4.h"
#include "GLBufferObject.h"

namespace davinci
{
	//reference
	//http://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
	//https://www.opengl.org/registry/specs/ARB/shader_storage_buffer_object.txt
	//https://www.opengl.org/wiki/Interface_Block_%28GLSL%29#Shader_binding_index_setting

	class GLShaderStorageBufferObject : public GLBufferObject
	{
		public:
			GLShaderStorageBufferObject(const std::string& name="Untitled SBBO", GLenum bufferUsage=GL_STATIC_DRAW);
	};
	typedef std::shared_ptr <GLShaderStorageBufferObject> GLShaderStorageBufferObjectRef;
}

#endif

