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

#ifndef _GLSHADER_H_
#define _GLSHADER_H_

#include <string>

#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "GLUniform.h"

namespace davinci{
class vec2f;
class vec3f;
class vec4f;
class vec3d;

class GLShader{
public:
	GLShader(const std::string& shaderName="Untitled Shader", bool useCoreProfile=false);
	void setVertexShaderStr(std::string& vtxShaderStr){m_vertexShaderProg=vtxShaderStr;}
	void setFragShaderStr(std::string& fragShaderStr){m_fragShaderProg=fragShaderStr;}
	void setGeomShaderStr(std::string& geoShaderStr){m_geomShaderProg=geoShaderStr;}

	bool refresh();

	bool loadVertexShaderFile(const std::string& fileName);
	bool loadFragShaderFile(const std::string& fileName);
	bool loadGeomShaderFile(const std::string& fileName);

	GLuint CreateShaders(void);
	void DestroyShaders(void);
	void UseShaders();
	void ReleaseShader();

	void SetFloatUniform( char* name, GLfloat val);
	void SetFloat2Uniform( char* name, const vec2f& val);
	void SetFloat3Uniform( char* name, const vec3f& val);
	void SetFloat4Uniform( char* name, const vec4f& val);

	void SetDoubleUniform( char* name, GLdouble val);

	void SetMatrixUniform( char* name, GLfloat mat[16], bool isRowMajor = true);
	void SetMatrixUniform( char* name, const mat4& mat, bool isRowMajor = true);
	void SetMatrixUniform( char* name, const mat3& mat, bool isRowMajor = true);
	//textUnitId: By default it is zero, which corresponds to
	// glActiveTexture(GL_TEXTURE0).
	//If you need multiple texture accessed in your fragment shader
	// you should send those texture to shader like the following.
	// Attach Texture 0
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _texture0);
	//glUniform1i(_uSampler0, 0);
	//
	// Attach Texture 1
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, _texture1);
	//glUniform1i(_uSampler1, 1);
	//
	//Be aware, the texUnitId is not
	// the texture id returned by glGenTextures(1, &m_texId);
	//user is responsible to call glActiveTexture() and GLTextureAbstract::bind()
	//before draw call. It is recommended to call the variation:
	//void SetSamplerUniform( char* name, GLTextureAbstract* tex) instead.
	void SetSamplerUniform( char* name, GLuint textUnitId=0);
	//A recommended call to set sampler uniform.
	void SetSamplerUniform( char* name, GLTextureAbstract* tex);
	//Attach GLTexture1D/2D/3D and GLTextureBufferObject to image unit
	//For GLTexture1D/2D/3D, you use imageLoad/Store to read/write texel from/to image1D/2D/3D
	//For GLTextureBufferObject, you use texelFetch() to read from samplerBuffer
	void SetImageUniform(char* name, GLTextureAbstract* tex);
	void SetUintUniform( char* name, GLuint val);
	void SetIntUniform( char* name, GLint val);
	void SetInt2Uniform( char* name, const vec2i& val);
	void SetInt3Uniform( char* name, const vec3i& val);
	void SetBoolUniform( char* name, bool val);
	/*TODO: will be available once vec4d is ready.
	void SetDouble2Uniform( char* name,const vec2f& val );
	void SetDouble4Uniform( char* name, const vec4f& val );
	*/
	void SetDouble3Uniform(char* name, const vec3d& val);
	void SetBlockUniform(char* name, GLUniformBlockBufferObjectRef bbo);
	void SetAtomicCounterUniform(char* name, GLAtomicCounterRef aco);
	void SetShaderStorageBlockUniform(char* name, GLShaderStorageBufferObjectRef ssbo);

	GLuint getProgramId(){ return m_programId;}
	std::string getShaderName() const { return m_shaderName; }
	void setShaderName(std::string val) { m_shaderName = val; }

protected:
	//This new interface provides a uniform mechanism for querying just about everything from a program.
	//At least, you can query everything that external OpenGL code can interface with. You can't query 
	//information about number of functions (unless they're subroutines) or global variables or things 
	//like that. But uniforms, inputs/outputs, etc? All of those are made available through this consistent interface.
	//https://www.opengl.org/wiki/Program_Introspection#Interface_query
	void interfaceQuery();

	bool m_activated;//indicate if the shader is in usage.
	bool m_bCoreProfile;

	GLuint m_vertShaderId;//=0;
	GLuint m_fragShaderId;//=0;
	GLuint m_geomShaderId;//=0;
	GLuint m_programId;//=0;

	std::string m_vertexShaderProg;
	std::string m_fragShaderProg;
	std::string m_geomShaderProg;

	std::string m_vertShaderFileName;
	std::string m_fragShaderFileName;
	std::string m_geomShaderFileName;

	//A readable identifiable name to show in case of compiling/linking error.
	std::string m_shaderName;

	std::unordered_map<std::string, GLUniform*> m_uniforms;
	std::unordered_set<std::string> m_uniformsActive;
protected:
	bool  loadShaderFile(const std::string& fileName, std::string& shaderProg, int& recDepth);
	GLint getUniformLocation(const char* name);
	void  dumpShaderFile( const std::string& fileName, std::string& shaderProg );
	void  checkProgamError();
	void  checkShaderCompileError(GLuint shaderId, const char* shaderName);
	void  checkShaderLinkError();
	void  clearActiveUniform();
};

typedef std::shared_ptr<GLShader> GLShaderRef;

}

#endif
