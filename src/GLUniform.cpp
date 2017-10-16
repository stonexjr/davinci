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

#include <GL/glew.h>
#include "GLError.h"
#include "GLUniform.h"
#include "GLTextureAbstract.h"
#include "GLTextureBufferObject.h"
#include "GLShader.h"

namespace davinci{

	GLUniform::GLUniform(void)
		:m_location(0), m_name("Undefined")//, m_bActive(false)
	{
	}

	GLUniform::GLUniform( int loc, const char* name )
		:m_location(loc), m_name(name)
	{
	}

	GLint GLUniform::getUniformLocation(GLShader &shader, std::string uniformName)
	{
		GLint programId = shader.getProgramId();
		GLint location = glGetUniformLocation(programId, uniformName.c_str());

		if (location == -1)
		{
			std::stringstream ss;
			ss << shader.getShaderName() << ":\n" << __func__
				<< ": index==-1, cannot find " << uniformName << "\n";
			//GLError::ErrorMessage(msg);
			cout << ss.str();
		}
		return location;
	}

	void GLUniform1f::upload()
	{
		if (m_location != -1)
		{
			glUniform1f(m_location, m_val);
		}
	}

	GLUniform1f::GLUniform1f( int loc, const char* name, float val )
		:GLUniform(loc, name), m_val(val)
	{
	}

	GLUniform2f::GLUniform2f( int loc, const char* name, const vec2f& val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform2f::upload()
	{
		if (m_location != -1)
		{
			glUniform2f(m_location, m_val.x(), m_val.y());
		}
	}

	GLUniform3f::GLUniform3f( int loc, const char* name, const vec3f& val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform3f::upload()
	{
		if (m_location != -1)
		{
			//glUniform3fv can be used to load a uniform variable array of type vec3
			//only one element of type vec3 need to be loaded
			glUniform3fv(m_location, 1, &(m_val[0]) );
			//glUniform3f(m_location, m_val[0], m_val[1], m_val[2] );
		}
	}

	GLUniform4f::GLUniform4f( int loc, const char* name, const vec4f& val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform4f::upload()
	{
		if (m_location != -1)
		{
			glUniform4fv(m_location, 1, &(m_val[0]) );
		}
	}

	GLUniform1d::GLUniform1d( int loc, const char* name, float val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform1d::upload()
	{
		if (m_location != -1)
		{
			glUniform1d(m_location, m_val);
		}
	}

	GLUniform3d::GLUniform3d( int loc, const char* name, const vec3d& val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform3d::upload()
	{
		if (m_location != -1)
		{
			glUniform3dv(m_location, 1,  &(m_val[0]) );
		}
	}

	GLUniform1u::GLUniform1u( int loc, const char* name, unsigned int val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform1u::upload()
	{
		if (m_location != -1)
		{
			glUniform1ui(m_location, m_val);
		}
	}

	GLUniform1i::GLUniform1i( int loc, const char* name, int val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform1i::upload()
	{
		if (m_location != -1)
		{
			glUniform1i(m_location, m_val);
		}
	}
	GLUniform2i::GLUniform2i( int loc, const char* name,const vec2i& val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform2i::upload()
	{
		if (m_location != -1)
		{
			glUniform2i(m_location, m_val[0], m_val[1]);
		}
	}
	GLUniform3i::GLUniform3i( int loc, const char* name,const vec3i& val )
		:GLUniform(loc, name), m_val(val)
	{}

	void GLUniform3i::upload()
	{
		if (m_location != -1)
		{
			glUniform3i(m_location, m_val[0], m_val[1], m_val[2]);
		}
	}

	GLUniformSampler::GLUniformSampler( int loc, const char* name
									  , GLTextureAbstract* val )
		:GLUniform1i(loc, name, -1),m_pTex(val)
	{
	}

	void GLUniformSampler::upload()
	{
		if (!m_pTex->isBind())
		{
			m_pTex->bindTexture();
		}
		GLUniform1i::m_val = m_pTex->getTextureUnitId();
		GLUniform1i::upload();
	}

	void GLUniformSampler::unbind()
	{
		/*
		GLTextureBufferObject* pTBO = dynamic_cast<GLTextureBufferObject*>(m_pTex);
		if (pTBO){
			stringstream ss;
			ss << __func__ << ": please set texture buffer object \"" << pTBO->getName()
			   << "\" using GLShader::SetImageUniform()!\n";
			GLError::ErrorMessage(ss.str());
		}
		*/
		if (GLUniform1i::m_val!=-1)
		{
			if (m_pTex)
			{
				m_pTex->unbindTexture();
				//m_pTex->releaseTexUnitId();
				GLUniform1i::m_val = -1;
			}else{
				stringstream ss;
				ss << __func__ << ": m_pTex==NULL! No GLTexture Object binded"
				<< "to the current GLUniformSampler instance. "
				<< "Please call GLShader::SetSamplerUniform() to bind them.\n";
				std::string msg = ss.str();
				GLError::ErrorMessage(msg);
			}
		}
	}
	//---------------------------------------------------------------------
	GLUniformImage::GLUniformImage(int loc, const char* name
		, GLTextureAbstract* val)
		:GLUniformSampler(loc, name, val)
	{
	}

	void GLUniformImage::upload()
	{
		//Dynamically allocated an available texture unit id.
		if (!m_pTex->isBind())
		{
			m_pTex->bindImage();
		}
		GLUniform1i::m_val = m_pTex->getImageUnitId();
		GLUniform1i::upload();
	}

	void GLUniformImage::unbind()
	{
		if (GLUniform1i::m_val != -1)
		{
			if (m_pTex)
			{
				m_pTex->unbindImage();
				GLUniform1i::m_val = -1;
			}
			else{
				stringstream ss;
				ss << __func__ << ": m_pTex==NULL! No GLTexture Object binded"
					<< "to the current GLUniformImage instance. "
					<< "Please call GLShader::SetImageUniform() to bind them.\n";
				std::string msg = ss.str();
				GLError::ErrorMessage(msg);
			}
		}
	}

	GLUniformMat4::GLUniformMat4( int loc, const char* name,
								  const mat4& val, bool isRowMajor/*=false*/ )
		:GLUniform(loc, name), m_val(val), m_isRowMajor(isRowMajor)
	{}

	void GLUniformMat4::upload()
	{
		if (m_location != -1)
		{
			glUniformMatrix4fv(m_location, 1, m_isRowMajor, m_val.get());
		}
	}

	GLUniformMat3::GLUniformMat3( int loc, const char* name,
		 const mat3& val, bool isRowMajor/*=false*/ )
		:GLUniform(loc, name), m_val(val), m_isRowMajor(isRowMajor)
	{}

	void GLUniformMat3::upload()
	{
		if (m_location != -1)
		{
			glUniformMatrix3fv(m_location, 1, m_isRowMajor, m_val.get());
		}
	}

	GLUniformBlock::GLUniformBlock(int loc, const char* name, 
							GLUniformBlockBufferObjectRef ubbo)
							:GLUniform(loc, name), m_programId(-1),m_ubbo(ubbo)
	{}

	GLint GLUniformBlock::getUniformLocation(GLShader &shader, std::string uniformName)
	{
		m_programId = shader.getProgramId();
		GLuint location = glGetUniformBlockIndex(m_programId, uniformName.c_str());

		if (location == GL_INVALID_INDEX)
		{
			std::stringstream ss;
			ss << shader.getShaderName() << ":\n" << __func__
				<< ": index==GL_INVALID_INDEX, cannot find " << uniformName << "\n";
			GLError::ErrorMessage(ss.str());
			cout << ss.str();
		}

		m_ubbo->bindBufferObject();
		glBindBufferBase(m_ubbo->getTarget(), m_ubbo->getBindingIndex(), m_ubbo->getId());
		stringstream ss;
		ss << __func__ << ": " << m_ubbo->getName() 
		   << " set binding index to buffer base failed!\n";
		GLError::glCheckError(ss.str());
		m_ubbo->unbindBufferObject();

		return location;
	}
	void GLUniformBlock::upload()
	{
		if (m_location != -1)
		{
			glUniformBlockBinding(m_programId, m_location, m_ubbo->getBindingIndex());
		}
	}

	void GLUniformBlock::unbind()
	{
		m_ubbo->releaseBindingIndex();
	}
	//Uniform Atomic Counter
	GLUniformAtomicCounter::GLUniformAtomicCounter(int loc, const char* name, GLAtomicCounterRef ubbo)
		:GLUniform(loc, name), m_programId(-1), m_uac(ubbo)
	{}

	GLint GLUniformAtomicCounter::getUniformLocation(GLShader &shader, std::string uniformName)
	{
		m_programId = shader.getProgramId();
		//https://www.opengl.org/sdk/docs/man/html/glGetProgramResourceIndex.xhtml
		//GL_INVALID_ENUM is generated if programInterface is GL_ATOMIC_COUNTER_BUFFER or GL_TRANSFORM_FEEDBACK_BUFFER,
		//active atomic counter and transform feedback buffer resources are not assigned name strings.
		/*
		GLuint location = glGetProgramResourceIndex(m_programId, GL_UNIFORM, uniformName.c_str());//glGetUniformBlockIndex(m_programId, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			std::stringstream ss;
			ss << shader.getShaderName() << ":\n" << __func__
			   << ": index==GL_INVALID_INDEX, cannot find " << uniformName << "\n";
			GLError::ErrorMessage(ss.str());
			cout << ss.str();
		}
		*/
		GLint numUniforms = 0;
		GLint bindingPoint = -1;
		/*
		GLint nActiveAC = -1, nActiveAC2 = -1;
		//two ways to get # of active atomic counter.
		glGetActiveAtomicCounterBufferiv(m_programId, 0, GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS, &nActiveAC);//Core since version 4.2
		glGetProgramInterfaceiv(m_programId, GL_ATOMIC_COUNTER_BUFFER, GL_ACTIVE_RESOURCES, &nActiveAC2);//Core since version 4.3
		
		const GLenum prop[3] = { GL_BUFFER_BINDING, GL_NUM_ACTIVE_VARIABLES, GL_ACTIVE_VARIABLES};
		GLint val[3] = { -1 };
		for (int i = 0; i < nActiveAC2; ++i)
		{
			glGetProgramResourceiv(m_programId, GL_ATOMIC_COUNTER_BUFFER, i, sizeof(prop) / sizeof(prop[0]),
				prop, sizeof(val) / sizeof(val[0]), NULL, val);

			GLError::glCheckError("Failed on glGetProgramResourceiv()");
			std::vector<char> nameData(val[1]);
			//https://www.opengl.org/wiki/GLAPI/glGetProgramResourceName
			//GL_INVALID_ENUM is generated if programInterface is GL_ATOMIC_COUNTER_BUFFER or GL_TRANSFORM_FEEDBACK_BUFFER,
			//since active atomic counter and transform feedback buffer resources are not assigned name strings.
			//
			//glGetProgramResourceName(m_programId, GL_ATOMIC_COUNTER_BUFFER, i, nameData.size(), NULL, &nameData[0]);
			//
			GLError::glCheckError("Failed on glGetProgramResourceName()");
			std::string name(nameData.begin(), nameData.end() - 1);
			if (name == uniformName)
			{
				glGetActiveAtomicCounterBufferiv(m_programId, i, GL_ATOMIC_COUNTER_BUFFER_BINDING, &bindingPoint);
				break;
			}
		}
		*/
		glGetProgramInterfaceiv(m_programId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

#define NUM_PROPERTIES 2
		const GLenum properties[NUM_PROPERTIES] = { GL_TYPE, GL_NAME_LENGTH };// , GL_ATOMIC_COUNTER_BUFFER_INDEX};
		GLint values[NUM_PROPERTIES] = { -1 };
		//There is no way to specify the binding point of atomic counter at run-time.
		//Has to query its value assigned during shader linking phase.
		for (int i = 0; i < numUniforms; ++i)
		{
			glGetProgramResourceiv(m_programId, GL_UNIFORM, i, sizeof(properties) / sizeof(properties[0]),
									properties, sizeof(values) / sizeof(values[0]),  NULL, values);

			std::vector<char> nameData(values[1]);
			glGetProgramResourceName(m_programId, GL_UNIFORM, i, nameData.size(), NULL, &nameData[0]);
			std::string name(nameData.begin(), nameData.end() - 1);
			if (name==uniformName)
			{
				glGetActiveAtomicCounterBufferiv(m_programId, i, GL_ATOMIC_COUNTER_BUFFER_BINDING, &bindingPoint);
				break;
			}
		}

		m_uac->bindBufferObject();
		//glBindBufferBase(m_uac->getTarget(), m_uac->getBindingIndex(), m_uac->getId());
		//There is no way to specify the binding point of atomic counter at run-time
		glBindBufferBase(m_uac->getTarget(), bindingPoint, m_uac->getId());
		stringstream ss;
		ss << __func__ << ": " << m_uac->getName()
		   << " set binding index to buffer base failed!\n";
		GLError::glCheckError(ss.str());
		m_uac->unbindBufferObject();

		return -1;
	}

	void GLUniformAtomicCounter::upload()
	{//stub
	}

	void GLUniformAtomicCounter::unbind()
	{
		m_uac->releaseBindingIndex();
	}
	//SSBO
	GLUniformSSBO::GLUniformSSBO(int loc, const char* name,
		GLShaderStorageBufferObjectRef ssbo)
		:GLUniform(loc, name), m_programId(-1), m_ssbo(ssbo)
	{}

	GLint GLUniformSSBO::getUniformLocation(GLShader &shader, std::string uniformName)
	{
		m_programId = shader.getProgramId();
		GLuint location = 
			glGetProgramResourceIndex(m_programId, GL_SHADER_STORAGE_BLOCK, uniformName.c_str());

		if (location == GL_INVALID_INDEX)
		{
			std::stringstream ss;
			ss << shader.getShaderName() << ":\n" << __func__
				<< ": index==GL_INVALID_INDEX, cannot find " << uniformName << "\n";
			//GLError::ErrorMessage(msg);
			cout << ss.str();
		}
		m_ssbo->bindBufferObject();
		glBindBufferBase(m_ssbo->getTarget(), m_ssbo->getBindingIndex(), m_ssbo->getId());
		stringstream ss;
		ss << __func__ << ": " << m_ssbo->getName()
			<< " set binding index to buffer base failed!\n";
		GLError::glCheckError(ss.str());
		m_ssbo->unbindBufferObject();

		return location;
	}

	void GLUniformSSBO::upload()
	{
		if (m_location != -1)
		{
			glShaderStorageBlockBinding(m_programId, m_location, m_ssbo->getBindingIndex());
		}
	}

	void GLUniformSSBO::unbind()
	{
		m_ssbo->releaseBindingIndex();
	}
}
