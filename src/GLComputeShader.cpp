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

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "vec3d.h"
#include "GLError.h"
#include "GLUtilities.h"
#include "GLComputeShader.h"

using namespace std;

namespace davinci{

	GLComputeShader::GLComputeShader(const std::string& shaderName/*="Untitled Shader"*/,
		 bool useCoreProfile /*=false*/ )
		:GLShader(shaderName, useCoreProfile), m_computeShaderId(0)
{
}

void GLComputeShader::UseShaders(int num_group_x, int num_group_y, int num_group_z){
	GLShader::UseShaders();
	// invoke compute shader
	glDispatchCompute(num_group_x, num_group_y, num_group_z);
}

GLuint GLComputeShader::CreateShaders(void)
{
	if (m_computeShaderProg.empty())
	{
		GLError::ErrorMessage(string("GLComputeShader:CreateShaders():computeShader string is empty, please load computeshader program first!"));
	}
	//1.Compile compute shader
	if (!m_computeShaderId)
	{
		m_computeShaderId = glCreateShader(GL_COMPUTE_SHADER);
		GLError::glCheckError("0");
	}
	const char* str_computedata = m_computeShaderProg.data();
	glShaderSource(m_computeShaderId, 1, &str_computedata, NULL);
	GLError::glCheckError("1");
	glCompileShader(m_computeShaderId);
	GLError::glCheckError("2");
	checkShaderCompileError(m_computeShaderId, "compute shader");
	GLError::glCheckError(" GLComputeShader:::CreateShaders():computeShader compiling failure.");
	//2. Link shader program
	if (m_programId)
	{
		glDeleteProgram(m_programId);
	}
	m_programId = glCreateProgram();
	glAttachShader(m_programId, m_computeShaderId);
	glLinkProgram(m_programId);
	checkShaderLinkError();
	GLError::glCheckError("GLComputeShader::CreateShaders(): create program failed.");
	int nActive=0;
	cout << "****************************************\n";
	cout << "|\t Compute Shader:" << m_shaderName << "\t\n";
	cout << "********Active Uniform******************\n";
	GLint maxLength=0;
	glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &nActive);
	glGetProgramiv(m_programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
	cout << nActive <<" active uniforms.\n";

	char* name = new char[maxLength+1];

	for(int i=0; i< nActive; ++i)  {
		int name_len=-1, num=-1;
		GLenum type = GL_ZERO;
		glGetActiveUniform( m_programId, GLuint(i), maxLength+1,
							&name_len, &num, &type, name );
		name[name_len] = 0;
		std::string strName(name);
		//Active Block Uniform use '.' to separate block name and inner variable name.
		//When uploading values to block uniform, only the block name is used to query
		//its index via glGetUniformBlockIndex(). Therefore, we only need block name
		//as has key. If there is no '.' simply take the entire key.
		m_uniformsActive.insert(strName.substr(0,strName.find('.')));
		cout << "index:" << i << " name:" << name << " type:" << GLUtilities::m_openglInt2TypeName[type] << endl;
		
	}
	delete[] name;

	cout << "\n********Active Attributes******************\n";
	glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTES, &nActive);
	glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
	cout << nActive <<" active attributes.\n";

	name = new char[maxLength+1];

	for(int i=0; i< nActive; ++i)  {
		int name_len=-1, num=-1;
		GLenum type = GL_ZERO;
		glGetActiveAttrib( m_programId, GLuint(i), maxLength+1,
						   &name_len, &num, &type, name );
		name[name_len] = 0;
		cout << "index:" << i << " name:" << name << " type:" << GLUtilities::m_openglInt2TypeName[type] << endl;
		
	}
	delete [] name;
	cout <<endl;
	cout << "=============== End of " << m_shaderName << " Info ===============\n";
	cout << "interface query:\n";
	interfaceQuery();
	return m_programId;
}

void GLComputeShader::DestroyShaders(void)
{
	GLenum ErrorCheckValue = glGetError();

	glUseProgram(0);
	glDetachShader(m_programId, m_computeShaderId);
	glDeleteShader(m_computeShaderId);
	glDeleteProgram(m_programId);

	m_uniforms.clear();

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
				stderr,
				"ERROR: Could not destroy the shaders: %s \n",
				gluErrorString(ErrorCheckValue)
				);

		exit(-1);
	}
}

bool GLComputeShader::loadComputeShaderFile(const string& fileName )
{
	m_computeShaderFileName = fileName;
	int recDepth = 0;
	return loadShaderFile(fileName, m_computeShaderProg, recDepth);
}

bool GLComputeShader::refresh()
{
	ReleaseShader();
	bool status = loadComputeShaderFile(m_computeShaderFileName);
	CreateShaders();
	return status;
}

}//end of namespace lily
