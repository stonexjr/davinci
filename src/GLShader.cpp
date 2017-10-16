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
#include <GL/glew.h>
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "vec3d.h"
#include "GLShader.h"
#include "GLError.h"
#include "GLUtilities.h"

using namespace std;

namespace davinci{

	GLShader::GLShader(const std::string& shaderName/*="Untitled Shader"*/, bool useCoreProfile /*=false*/ )
		:m_vertShaderId(0), m_fragShaderId(0), m_geomShaderId(0),m_programId(0)
		,m_vertShaderFileName(""),m_fragShaderFileName(""),m_geomShaderFileName("")
		,m_vertexShaderProg(""),m_fragShaderProg(""),m_geomShaderProg("")
		, m_shaderName(shaderName), m_activated(false), m_bCoreProfile(useCoreProfile)
{
}

void GLShader::UseShaders(){
	if (!m_programId)
	{
		GLError::ErrorMessage(string("Please CreateShader() before UseShader()!"));
	}

	glUseProgram(m_programId);
	m_activated = true;

#if defined(DEBUG) || defined(_DEBUG)
	GLError::glCheckError("glUseProgram failed!");
#endif
	
	std::unordered_set<std::string>::iterator it = m_uniformsActive.begin();
	for ( ; it != m_uniformsActive.end() ; it++)
	{//iterate each active uniform variable defined in the current shader.
		std::string uniformName = *it;
		if (m_uniforms.count(uniformName) > 0){
			GLUniform* curUniform = m_uniforms[uniformName];
			//GLint location = getUniformLocation(curUniform->getName().data());
			GLint location =
				curUniform->getUniformLocation(*this, curUniform->getName());

			curUniform->setLocation(location);
			curUniform->upload();

#if defined(DEBUG) || defined(_DEBUG)
			{
				stringstream ss;
				ss << "upload uniform: " << curUniform->getName() << " failed!";
				string msg = ss.str();
				GLError::glCheckError(msg);
			}
#endif
		}else{
			if (m_bCoreProfile)
			{//core profile insist not using uninitialized variable.
				stringstream ss;
				ss << __func__ << ": Shader " << getShaderName() << " has active uniform "
					<< uniformName << " not initialized on client side!\n";
				GLError::ErrorMessage(ss.str());
			}
		}
	}
	/*
	std::unordered_map<string, GLUniform*>::iterator it = m_uniforms.begin();
	for ( ; it != m_uniforms.end() ; it++)
	{
		GLUniform* curUniform = it->second;

		GLint location = getUniformLocation(curUniform->getName().data());
		curUniform->setLocation(location);
		curUniform->upload();

#if defined(DEBUG) || defined(_DEBUG)
		{
			stringstream ss;
			ss << "upload uniform: "<<curUniform->getName()<<" failed!";
			string msg=ss.str();
			GLError::glCheckError(msg);
		}
#endif
	}
	*/

}
void GLShader::ReleaseShader(){
	//Release texture unit id if there is any GLUniformSampler exists.
	std::unordered_map<string, GLUniform*>::iterator it = m_uniforms.begin();
	for ( ; it != m_uniforms.end() ; it++)
	{
		GLUniform* curUniform = it->second;
		GLUniformSampler* pUniformSampler = 
			dynamic_cast<GLUniformSampler*>(curUniform);
		if (pUniformSampler)
		{
			pUniformSampler->unbind();
		}
	}
	glUseProgram(0);
	m_activated = false;
}
GLuint GLShader::CreateShaders(void)
{
	if (m_vertexShaderProg.empty())
	{
		GLError::ErrorMessage(string("GLShader::CreateShaders():vertexShader string is empty, please load vertex shader program first!"));
	}
	if (m_fragShaderProg.empty())
	{
		GLError::ErrorMessage(string("GLShader::CreateShaders():fragmentShader string is empty, please load fragment shader program first!"));
	}
	clearActiveUniform();
	//1. Compile vertex shader
	if (!m_vertShaderId)
	{
		m_vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	}
	const char* str_vtxdata = m_vertexShaderProg.data();
	glShaderSource(m_vertShaderId, 1, &str_vtxdata, NULL);
	glCompileShader(m_vertShaderId);
	checkShaderCompileError(m_vertShaderId, "vertex shader");
	GLError::glCheckError(" GLShader::CreateShaders():vertex shader compiling failure.");

	//2. Compile fragment shader
	if (!m_fragShaderId)
	{
		m_fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	}
	const char* str_fragdata = m_fragShaderProg.data();
	glShaderSource(m_fragShaderId, 1, &str_fragdata, NULL);
	glCompileShader(m_fragShaderId);
	checkShaderCompileError(m_fragShaderId, "fragment shader");
	GLError::glCheckError("GLShader::CreateShaders():fragment shader compiling failure.");

	//3. Compile geometry shader if given.
	if (!m_geomShaderProg.empty())
	{
		if (!m_geomShaderId)
		{
			m_geomShaderId = glCreateShader(GL_GEOMETRY_SHADER);
		}
		const char* str_geomdata = m_geomShaderProg.data();
		glShaderSource(m_geomShaderId, 1, &str_geomdata, NULL);
		glCompileShader(m_geomShaderId);
		checkShaderCompileError(m_geomShaderId, "geometry shader");
		GLError::glCheckError("GLShader::CreateShaders(): geometry shader compiling failure.");
	}

	//4. Link shader program
	if (m_programId)
	{
		glDeleteProgram(m_programId);
	}
	m_programId = glCreateProgram();
	glAttachShader(m_programId, m_vertShaderId);
	glAttachShader(m_programId, m_fragShaderId);
	if (!m_geomShaderProg.empty())
	{
		glAttachShader(m_programId, m_geomShaderId);
	}
	glLinkProgram(m_programId);

	checkShaderLinkError();
	//checkProgamError();

	GLError::glCheckError("GLShader::CreateShaders(): create program failed.");
	int nActive=0;
	cout << "****************************************\n";
	cout << "|\t" << m_shaderName << "\t\n";
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
	if (!m_geomShaderProg.empty())
	{
		cout << "\n********Geometry Shader Info******************\n";
		GLint intvalue = 0;
		glGetProgramiv(m_programId, GL_GEOMETRY_VERTICES_OUT, &intvalue);
		cout << "# of vertices to emit: " << intvalue << endl;
		glGetProgramiv(m_programId, GL_GEOMETRY_INPUT_TYPE, &intvalue);
		cout << "input type: " << GLUtilities::m_openglInt2TypeName[intvalue] << endl;
		glGetProgramiv(m_programId, GL_GEOMETRY_OUTPUT_TYPE, &intvalue);
		cout << "output type: " << GLUtilities::m_openglInt2TypeName[intvalue] << endl;
		GLError::glCheckError("geometry shader info error");
	}

	delete [] name;

	cout <<endl;

	cout << "interface query:\n";

	interfaceQuery();

	cout << "=============== End of " << m_shaderName << " Info ===============\n";
	return m_programId;
}
void GLShader::interfaceQuery()
{
	//https://www.opengl.org/wiki/Program_Introspection
	//https://www.opengl.org/wiki/Program_Introspection#Interface_query
	//https://www.opengl.org/wiki/GLAPI/glGetProgramInterface
	//https://www.opengl.org/wiki/GLAPI/glGetProgramResource
	
	int nActive = 0, maxLength=0;
	char* name = NULL;
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(m_programId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
	const GLenum properties[4] = { GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };

	cout << "Active none block uniforms:\n";
	for (int i = 0; i < numUniforms; ++i)
	{
		GLint values[4];
		glGetProgramResourceiv(m_programId, GL_UNIFORM, i, 4, properties, 4, NULL, values);

		//Skip any uniforms that are in a block.
		//if (values[0] != -1)
		//	continue;

		//Get the name. Must use a std::vector rather than a std::string for C++03 standards issues.
		//C++11 would let you use a std::string directly.
		std::vector<char> nameData(values[2]);
		glGetProgramResourceName(m_programId, GL_UNIFORM, i, nameData.size(), NULL, &nameData[0]);
		std::string name(nameData.begin(), nameData.end() - 1);

		m_uniformsActive.insert(name.substr(0, name.find('.')));

		cout<< "index:" << i << " name:" << name 
			<< " type:" << GLUtilities::m_openglInt2TypeName[values[1]] << endl;
	}
	//Iteration over all uniforms within each uniform block.
	GLint numBlocks = 0;
	const GLenum programInterface[2] = { GL_UNIFORM_BLOCK, GL_SHADER_STORAGE_BLOCK };
	const GLenum memberVariableInterface[2] = { GL_UNIFORM, GL_BUFFER_VARIABLE};
	const string interfaceName[2] = {"uniform block", "shader storage block"};
	const GLenum blockProperties[1] = { GL_NUM_ACTIVE_VARIABLES };
	const GLenum activeUnifProp[1] = { GL_ACTIVE_VARIABLES };
	const GLenum unifProperties[3] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
	//const GLenum unifProperties[2] = { GL_NAME_LENGTH, GL_TYPE };
	//int index = glGetProgramResourceIndex(m_programId, GL_BUFFER_VARIABLE, "latdata");

	/*
	GLError::glCheckError("-2");
	GLint values[3];
	glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, index, 2, unifProperties, 2 , NULL, values);
	GLError::glCheckError("-1");
	*/
	cout << endl;
	glGetProgramInterfaceiv(m_programId, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
	for (int blockIx = 0; blockIx < numBlocks; ++blockIx)
	{
		GLint numActiveUnifs = 0;
		glGetProgramResourceiv(m_programId, GL_UNIFORM_BLOCK, blockIx, 1, blockProperties, 1, NULL, &numActiveUnifs);
		GLError::glCheckError("0");

		if (!numActiveUnifs) continue;

		std::vector<GLint> blockUnifs(numActiveUnifs);
		GLsizei len = -1;
		glGetProgramResourceiv(m_programId, GL_UNIFORM_BLOCK, blockIx, 1, activeUnifProp, numActiveUnifs, &len, &blockUnifs[0]);
		GLError::glCheckError("1");
		cout << "uniform block" << blockIx << "[\n";
		for (int unifIx = 0; unifIx < numActiveUnifs; ++unifIx)
		{
			//For resources that contain other resources (uniform/shader storage blocks and atomic counter buffers),
			//this query is used to return a list of the active variables that they contain. The particular interface
			//to use these indices with depends on what is being queried. If GL_UNIFORM_BLOCK or GL_ATOMIC_COUNTER_BUFFER
			//is used, then the indices are indices into the GL_UNIFORM interface. If GL_SHADER_STORAGE_BLOCK is used, 
			//then they are indices into the GL_BUFFER_VARIABLE interface. If GL_TRANSFORM_FEEDBACK_BUFFER is used, then 
			//they are indices in the GL_TRANSFORM_FEEDBACK_VARYING interface.
			GLint values[3];
			glGetProgramResourceiv(m_programId, GL_UNIFORM, blockUnifs[unifIx], 3, unifProperties, 3, NULL, values);
			GLError::glCheckError("2");

			//Get the name. Must use a std::vector rather than a std::string for C++03 standards issues.
			//C++11 would let you use a std::string directly.
			std::vector<char> nameData(values[0]);
			glGetProgramResourceName(m_programId, GL_UNIFORM, blockUnifs[unifIx], nameData.size(), NULL, &nameData[0]);
			GLError::glCheckError("3");
			std::string name(nameData.begin(), nameData.end() - 1);
			GLError::glCheckError("--");

			m_uniformsActive.insert(name.substr(0, name.find('.')));
			cout << "index:" << unifIx << " name:" << name
				<< " type:" << GLUtilities::m_openglInt2TypeName[values[1]] << endl;
		}
		cout << "]\n";
	}
	glGetProgramInterfaceiv(m_programId, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
	cout << numBlocks << " active shader storage block.\n";
	for (int blockIx = 0; blockIx < numBlocks; ++blockIx)
	{
		GLint numActiveUnifs = 0;
		GLint values[3] = { 0 };
		glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, blockIx, 1, unifProperties, 1, NULL, values);

		std::vector<char> nameData(values[0]);
		glGetProgramResourceName(m_programId, GL_SHADER_STORAGE_BLOCK, blockIx, nameData.size(), NULL, &nameData[0]);
		std::string name(nameData.begin(), nameData.end() - 1);
		m_uniformsActive.insert(name);
		cout << "shader storage name: " << name << "[\n";

		glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, blockIx, 1, blockProperties, 1, NULL, &numActiveUnifs);
		if (!numActiveUnifs) continue;


		std::vector<GLint> blockUnifs(numActiveUnifs);
		glGetProgramResourceiv(m_programId, GL_SHADER_STORAGE_BLOCK, blockIx, 1, activeUnifProp, numActiveUnifs, NULL, &blockUnifs[0]);
		for (int unifIx = 0; unifIx < numActiveUnifs; ++unifIx)
		{
			GLint values[3];
			glGetProgramResourceiv(m_programId, GL_BUFFER_VARIABLE, blockUnifs[unifIx], 2, unifProperties, 2, NULL, values);

			//Get the name. Must use a std::vector rather than a std::string for C++03 standards issues.
			//C++11 would let you use a std::string directly.
			std::vector<char> nameData(values[0]);
			glGetProgramResourceName(m_programId, GL_BUFFER_VARIABLE, blockUnifs[unifIx], nameData.size(), NULL, &nameData[0]);
			std::string name(nameData.begin(), nameData.end() - 1);
			//An array member variable in Active Shader Storage Buffer Block Object queried by glGetProgramResourceName()
			//contains '[0]'. It should be trimed, so that glGetProgramResourceIndex() can use it can fetch the corresponding index.
			cout << "index:" << unifIx << " name:" << name
				<< " type:" << GLUtilities::m_openglInt2TypeName[values[1]] << endl;
		}
		cout << "]\n";

	}
}

void GLShader::checkProgamError()
{
	int loglength=0;
	glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &loglength);
	GLchar *progInfoLog = new GLchar[loglength+1];
	glGetProgramInfoLog(m_programId, sizeof(GLchar)*loglength, NULL, progInfoLog);
	fprintf(stderr, "\n%s\n", progInfoLog);
	delete [] progInfoLog;
}

void GLShader::checkShaderLinkError()
{
	GLchar* infoLog;
	GLint infoLogLen;
	GLint linkStatus;
	glGetProgramiv( m_programId, GL_LINK_STATUS, &linkStatus );
	GLError::glCheckError("Link STATUS");
	glGetProgramiv( m_programId, GL_INFO_LOG_LENGTH, &infoLogLen );

	if( infoLogLen > 1 )
	{
		infoLog = new GLchar[infoLogLen+1];
		glGetProgramInfoLog( m_programId, infoLogLen, NULL, infoLog );
		infoLog[infoLogLen] = '\0';
		if (linkStatus == GL_FALSE)
		{
			fprintf( stderr, "Linking Error Info Log:\n%s\n", infoLog );
		}else{
			fprintf( stderr, "Linking Info Log:\n%s\n", infoLog );
		}
		delete [] infoLog;
	}

	if( linkStatus == GL_FALSE )
	{
		fprintf(stderr,"Failed to link program ");
		glDeleteProgram( m_programId );
		string msg = m_shaderName+": linking failed.\n";
		GLError::ErrorMessage(msg);
	}
}
void GLShader::checkShaderCompileError(GLuint shaderId, const char* shaderType)
{
	int status=0;
	glGetShaderiv( shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int loglength=0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &loglength);
		GLchar* log = new GLchar[loglength];
		glGetShaderInfoLog(shaderId, loglength, NULL, log);
		fprintf(stderr, "\n%s\n", log);
		delete[] log;

		string msg = m_shaderName+":"+string(shaderType)+" compilation failed.\n";
		GLError::ErrorMessage(msg);

	}
}

GLint GLShader::getUniformLocation(const char* name )
{
	/*
	if (!m_activated)
	{
		stringstream ss;
		ss << "Current Shader "<<m_shaderName<<" is not activated. Call UseShader() to activate it!";
		string msg=ss.str();
		GLError::ErrorMessage(msg);
	}
	*/
	GLint location = glGetUniformLocation(m_programId, name);

	if (location == -1)
	{
		std::stringstream ss;
		ss <<m_shaderName<< ":\n"<<__func__
		   <<": index==-1, cannot find "<<name<<"\n";
		//GLError::ErrorMessage(msg);
		cout << ss.str();
	}
	return location;
}

void GLShader::SetFloatUniform( char* name, GLfloat val){
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform1f(-1, name, val);
    }else{
        GLUniform1f* uniform_ptr = (GLUniform1f*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetFloat2Uniform( char* name,const vec2f& val)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform2f(-1, name, val);
    }else{
        GLUniform2f* uniform_ptr = (GLUniform2f*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetFloat3Uniform( char* name, const vec3f& val )
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform3f(-1, name, val);
    }else{
        GLUniform3f* uniform_ptr = (GLUniform3f*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetFloat4Uniform( char* name, const vec4f& val )
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform4f(-1, name, val);
    }else{
        GLUniform4f* uniform_ptr = (GLUniform4f*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetDoubleUniform( char* name, GLdouble val)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform1d(-1, name, val);
    }else{
        GLUniform1d* uniform_ptr = (GLUniform1d*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

/*TODO: will be available once vec4d is ready.
void GLShader::SetDouble2Uniform( char* name,const vec2d& val )
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform2d(location, val.x(), val.y());
	}
}
*/

void GLShader::SetDouble3Uniform( char* name, const vec3d& val )
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform3d(-1, name, val);
    }else{
        GLUniform3d* uniform_ptr = (GLUniform3d*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}
/*TODO: will be available once vec4d is ready.
void GLShader::SetDouble4Uniform( char* name, const vec4f& val )
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform4d(location, val.x(), val.y(), val.z(), val.w());
	}
}
*/

void GLShader::SetUintUniform( char* name, GLuint val){
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform1u(-1, name, val);
    }else{
        GLUniform1u* uniform_ptr = (GLUniform1u*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetBoolUniform( char* name, bool val){
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform1i(-1, name, val);
    }else{
        GLUniform1i* uniform_ptr = (GLUniform1i*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetIntUniform( char* name, GLint val){
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform1i(-1, name, val);
    }else{
        GLUniform1i* uniform_ptr = (GLUniform1i*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetInt2Uniform(char* name, const vec2i& val)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform2i(-1, name, val);
    }else{
        GLUniform2i* uniform_ptr = (GLUniform2i*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetInt3Uniform(char* name, const vec3i& val)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform3i(-1, name, val);
    }else{
        GLUniform3i* uniform_ptr = (GLUniform3i*)m_uniforms[name];
        uniform_ptr->setValue(val);
    }
}

void GLShader::SetMatrixUniform( char* name, GLfloat mat[16], bool isRowMajor/*=true*/){
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniformMat4(-1, name, mat4(mat), isRowMajor);
    }else{
        GLUniformMat4* uniform_ptr = (GLUniformMat4*)m_uniforms[name];
        uniform_ptr->setValue(mat4(mat), isRowMajor);
    }
}

void GLShader::SetMatrixUniform( char* name, const mat4& mat, bool isRowMajor /*= true*/ )
{
    if (m_uniforms.find(name) == m_uniforms.end()){
        m_uniforms[name] = new GLUniformMat4(-1, name, mat, isRowMajor);
    }else{
        GLUniformMat4* uniform_ptr = (GLUniformMat4*)(m_uniforms[name]);
        uniform_ptr->setValue(mat, isRowMajor);
    }
}

void GLShader::SetMatrixUniform( char* name, const mat3& mat, bool isRowMajor /*= true*/ )
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniformMat3(-1, name, mat, isRowMajor);
    }else{
        GLUniformMat3* uniform_ptr = (GLUniformMat3*)m_uniforms[name];
        uniform_ptr->setValue(mat, isRowMajor);
    }
}

void GLShader::SetSamplerUniform( char* name, GLuint textUnitId/*=0*/)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniform1i(-1, name, textUnitId);
    }else{
        GLUniform1i* uniform_ptr = (GLUniform1i*)m_uniforms[name];
        uniform_ptr->setValue(textUnitId);
    }
}

void GLShader::SetSamplerUniform( char* name,  GLTextureAbstract* tex )
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniformSampler(-1, name, tex);
    }else{
        GLUniformSampler* uniform_ptr = (GLUniformSampler*)m_uniforms[name];
        uniform_ptr->setValue(tex);
    }
}

void GLShader::SetImageUniform( char* name,  GLTextureAbstract* tex )
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniformImage(-1, name, tex);
    }else{
        GLUniformImage* uniform_ptr = (GLUniformImage*)m_uniforms[name];
        uniform_ptr->setValue(tex);
    }
}

void GLShader::SetBlockUniform(char* name, GLUniformBlockBufferObjectRef bbo)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniformBlock(-1, name, bbo);
    }else{
        GLUniformBlock* uniform_ptr = (GLUniformBlock*)m_uniforms[name];
        uniform_ptr->setValue(bbo);
    }
}

void GLShader::SetAtomicCounterUniform(char* name, GLAtomicCounterRef aco)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniformAtomicCounter(-1, name, aco);
    }else{
        GLUniformAtomicCounter* uniform_ptr = (GLUniformAtomicCounter*)m_uniforms[name];
        uniform_ptr->setValue(aco);
    }
}

void GLShader::SetShaderStorageBlockUniform(char* name, GLShaderStorageBufferObjectRef ssbo)
{
    if (m_uniforms.find(name) == m_uniforms.end())
    {
        m_uniforms[name] = new GLUniformSSBO(-1, name, ssbo);
    }else{
        GLUniformSSBO* uniform_ptr = (GLUniformSSBO*)m_uniforms[name];
        uniform_ptr->setValue(ssbo);
    }
}

void GLShader::DestroyShaders(void)
{
	GLenum ErrorCheckValue = glGetError();

	glUseProgram(0);

	glDetachShader(m_programId, m_vertShaderId);
	glDetachShader(m_programId, m_fragShaderId);

	glDeleteShader(m_fragShaderId);
	glDeleteShader(m_vertShaderId);

	if (!m_geomShaderProg.empty())
	{
		glDetachShader(m_programId, m_geomShaderId);
		glDeleteShader(m_geomShaderId);
	}

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

bool GLShader::loadVertexShaderFile( const string& fileName )
{
	m_vertShaderFileName = fileName;
	int recDepth = 0;
	return loadShaderFile(fileName, m_vertexShaderProg, recDepth);
}

bool GLShader::loadFragShaderFile( const string& fileName )
{
	m_fragShaderFileName = fileName;
	int recDepth = 0;
	return loadShaderFile(fileName, m_fragShaderProg, recDepth);
}

bool GLShader::loadGeomShaderFile( const string& fileName )
{
	m_geomShaderFileName = fileName;
	int recDepth = 0;
	return loadShaderFile(fileName, m_geomShaderProg, recDepth);
}

void GLShader::dumpShaderFile( const std::string& fileName, std::string& shaderProg )
{
	string dumpfileName = fileName+".dump";
	ofstream ofs(dumpfileName);
	if (!ofs)
	{
		std::stringstream ss;
		ss <<dumpfileName<<" open failed!\n";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
	}
	ofs << shaderProg;
	cout << "Dump preprocessed shader to\n" << dumpfileName<<endl;
	ofs.close();
}

bool GLShader::loadShaderFile( const std::string& fileName, std::string& shaderProg, int& recDepth )
{
	std::ifstream ifs(fileName);
	if (!ifs)
	{
		std::stringstream ss;
		ss <<"Shader file:"<< fileName <<" not found!\n";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
		return false;
	}
	string line;
	shaderProg="";
	string incTag="#include";
	bool hasIncludeFile=false;
	while(getline(ifs,line,'\n')){
		//Preprocess the shader and search the key word "#include" to replace
		//it with the included file, throw error msg upon failure to find the
		//included file.
		size_t incIdx = line.find(incTag);
		size_t commentIdx = line.find("//");
		if (commentIdx == string::npos //The "#include" is not commented by //
			&& incIdx  != string::npos)
		{//extract the include file full path.
			hasIncludeFile = true;
			string incFilePath = line.substr(incIdx+incTag.size()+1);
			loadShaderFile(incFilePath, line, recDepth);
		}

		shaderProg += line+"\n";
	}
	ifs.close();
	if (recDepth==0 && hasIncludeFile)
	{//std::cout << shaderProg <<std::endl;
		//dump shader after preprocessing.
		dumpShaderFile(fileName, shaderProg);
	}
	return true;
}

bool GLShader::refresh()
{
	ReleaseShader();
	bool status = loadVertexShaderFile(m_vertShaderFileName);
	status &= loadFragShaderFile(m_fragShaderFileName);
	if (!m_geomShaderFileName.empty())
	{
		status &= loadGeomShaderFile(m_geomShaderFileName);
	}
	CreateShaders();
	return status;
}

void GLShader::clearActiveUniform()
{
	//m_uniforms.clear();
	m_uniformsActive.clear();
}



}//end of namespace lily
