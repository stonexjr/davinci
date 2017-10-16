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
//CREATED: 2014-02-15
//UPDATED: 2014-02-15
#ifndef _GLCOMPUTESHADER_H_
#define _GLCOMPUTESHADER_H_

#include <string>

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include "GLShader.h"

namespace davinci{
class vec2f;
class vec3f;
class vec4f;
class vec3d;

class GLComputeShader: public GLShader{
public:
	GLComputeShader(const std::string& shaderName="Untitled Compute Shader", bool useCoreProfile=false);
	void setComputeShaderStr(std::string& compShaderStr){m_computeShaderProg = compShaderStr;}

	void UseShaders(int num_group_x, int num_group_y, int num_group_z);
	bool refresh();

	bool loadComputeShaderFile(const string& fileName);

	GLuint CreateShaders(void);
	void DestroyShaders(void);
private:
	void UseShaders();
	void loadVertexShaderFile(const std::string& fileName);
	void loadFragShaderFile(const std::string& fileName);
	void loadGeomShaderFile(const std::string& fileName);
	void setVertexShaderStr(std::string& vtxShaderStr);
	void setFragShaderStr(std::string& fragShaderStr);
	void setGeomShaderStr(std::string& geoShaderStr);
protected:
	GLuint      m_computeShaderId;
	std::string m_computeShaderProg;
	std::string m_computeShaderFileName;
};

typedef std::shared_ptr<GLComputeShader> GLComputeShaderRef;

}

#endif
