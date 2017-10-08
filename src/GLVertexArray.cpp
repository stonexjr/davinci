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

#include <iostream>
#include "GLVertexArray.h"

namespace davinci{

GLVertexArrayObject::GLVertexArrayObject(GLenum geotype,std::shared_ptr<GLenum> arrayId/*=NULL*/,
										 GLenum usage/*=GL_STATIC_DRAW*/, bool warning/*=false*/)
		:m_arrayId(arrayId), m_geotype(geotype),
		  m_hasVertex(false), m_hasColor(false), m_hasNormal(false),
		  m_vertexComponetCount(3),m_vertexType(0), m_vertexStrideInBytes(0),
		  m_vertexOffsetInBytes(0),m_colorComponentCount(3),m_colorType(0),
		  m_colorOffsetInBytes(0), m_normalType(0), m_normalOffsetInBytes(0),
		  m_attribsSizeInBytes(0), m_enableWarning(warning)
		,m_GLVersion(3.3f),m_enabled(false)
{

	GLint maxTexImgUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxTexImgUnits);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &g_maxAttrib);
	m_hasTextures.resize(maxTexImgUnits,0);
	m_textureType.resize(maxTexImgUnits,0);
	m_textureStride.resize(maxTexImgUnits, 0);
	m_textureOffset.resize(maxTexImgUnits, 0);
	m_textureComponentCount.resize(maxTexImgUnits, 0);

	g_GLTypeSizeInBytes[GL_FLOAT]			= sizeof(GLfloat);
	g_GLTypeSizeInBytes[GL_DOUBLE]			= sizeof(GLdouble);
	g_GLTypeSizeInBytes[GL_INT]				= sizeof(GLint);
	g_GLTypeSizeInBytes[GL_UNSIGNED_BYTE]	= sizeof(GLubyte);
	g_GLTypeSizeInBytes[GL_UNSIGNED_INT]	= sizeof(GLuint);

	if (m_arrayId==NULL)
	{
		GLenum id=0;
		glGenVertexArrays(1, &id);
		m_arrayId = std::shared_ptr<GLenum>(new GLenum(id));
	}
}

davinci::GLVertexArrayObject::~GLVertexArrayObject(void)
{
	disable();
	if (m_arrayId && m_arrayId.unique())
		glDeleteVertexArrays(1, &(*m_arrayId) );
}

void davinci::GLVertexArrayObject::setShaderProgId( GLuint progId )
{
	for (std::unordered_map<std::string, GLAttributeRef>::iterator it = m_attribs.begin();
		 it != m_attribs.end() ; ++it)
	{
		//check if the current attribute is active.
		it->second->setShaderProgId(progId);
	}
}

void GLVertexArrayObject::enable()
{
	glBindVertexArray(*m_arrayId);
	GLError::glCheckError("GLVertexArrayObject::enable(): glBindVertexBuffer() failed.");
	if (m_vboDefault)// && m_GLVersion>4.4)
	{
		GLuint bindingIndex = 0 ;
		GLintptr offset = 0;
		GLsizei  stride = m_vertexStrideInBytes;//m_vboDefault->getStride();//computeStride();

		m_vboDefault->bindBufferObject();
#if defined(__APPLE__) || defined(MACOSX)
#else
		glBindVertexBuffer(m_vboDefault->getBindingIndex()//bindingIndex
						   , m_vboDefault->getId(), offset, stride);
#endif

		GLError::glCheckError("GLVertexArrayObject::enable():glBindVertexBuffer failed!");

	}else if(!m_vboDefault && m_attribs.empty()){
		GLError::ErrorMessage(string("m_vboDefault is not specified, VAO has no associated VBO."));
	}

	enableGLClientState();
	GLError::glCheckError("GLVertexArrayObject::enable():enableGLClientState() failed!");
    if (m_vboDefault)
    {
        m_vboDefault->unbindBufferObject();
    }
	glBindVertexArray(0);
	m_enabled = true;
}

void GLVertexArrayObject::disable()
{
	glBindVertexArray(*m_arrayId);
	if (m_vboDefault)// && m_GLVersion>4.4)
	{
		GLuint bindingIndex = 0 ;
		GLintptr offset = 0;
		GLsizei  stride = m_vertexStrideInBytes;//m_vboDefault->getStride();//computeStride();

		m_vboDefault->bindBufferObject();
#if defined(__APPLE__) || defined(MACOSX)
		//m_vboDefault->bind();
#else
		/*
		glBindVertexBuffer(m_vboDefault->getBindingIndex()//bindingIndex
						 , m_vboDefault->getId(), offset, stride);
						 */
#endif

	}else if(!m_vboDefault && m_attribs.empty()){
		GLError::ErrorMessage(string("m_vboDefault is not specified, VAO has no associated VBO."));
	}

	disableGLClientState();
	if (m_vboDefault)
	{
		m_vboDefault->releaseBindingIndex();
        m_vboDefault->unbindBufferObject();
	}
	glBindVertexArray(0);
	m_enabled = false;
}

void GLVertexArrayObject::sanityCheckVBO()
{
	if (!m_vboDefault)
	{
		GLError::ErrorMessage(string("enableGLClientState(): m_vboDefault==NULL, Please set default VBO if you want to GL compatibility mode to set up vertex attributes!\n"));
	}else if (m_vboDefault->getVertexCount()<=0)
	{
		std::stringstream ss;
		ss <<"GLVertexArrayObject::enableGLClientState(): m_vboDefault->getVertexCount()<=0, No vertex attribute content in the associated VBO"
		  <<".\nCall GLVertexBufferObject::upload() to upload vertex attribute content to GPU";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
	}
}
void GLVertexArrayObject::enableGLClientState()
{
	//gl[Vertex/Color/Normal/Texture]Pointer() is supported since GL 1.1,
	//but is deprecated since 3.3. Use it if you need OpenGL compatibility mode.
	//Same functionality can be achieved using glVertexAttribFormat
	if (m_hasVertex)
	{
		if (m_vertexType==0)
		{
			GLError::ErrorMessage(string("enableGLClientState():Please set vertex type!\n"));
		}
		sanityCheckVBO();

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(m_vertexComponetCount, m_vertexType/*GL_FLOAT*/, m_vertexStrideInBytes,
						reinterpret_cast<const GLvoid*>(m_vertexOffsetInBytes));
		GLError::glCheckError(__func__);
	}
	if (m_hasColor)
	{
		if (m_colorType==0)
		{
			GLError::ErrorMessage(string("GLVertexArrayObject::enableGLClientState():Please set color type!\n"));
		}
		sanityCheckVBO();

		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(m_colorComponentCount, m_colorType /*GL_FLOAT*/,m_vertexStrideInBytes,// m_colorStrideInBytes,
					   reinterpret_cast<const GLvoid*>(m_colorOffsetInBytes));
		GLError::glCheckError(__func__);
	}
	if (m_hasNormal)
	{
		if (m_normalType==0)
		{
			GLError::ErrorMessage(string("GLVertexArrayObject::enableGLClientState():Please set normal type!\n"));
		}
		sanityCheckVBO();

		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(m_normalType /*GL_FLOAT*/,m_vertexStrideInBytes,
						reinterpret_cast<const GLvoid*>(m_normalOffsetInBytes));
		GLError::glCheckError(__func__);
	}
	GLint maxTexImgUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxTexImgUnits);
	for (int i = 0 ; i < maxTexImgUnits ; i++)
	{
		if (m_hasTextures[i])
		{
			if (m_textureType[i]==0)
			{
				GLError::ErrorMessage(string("GLVertexArrayObject::enableGLClientState():Please set texture type!\n"));
			}

			sanityCheckVBO();
			glClientActiveTexture(GL_TEXTURE0+i);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(m_textureComponentCount[i], m_textureType[i],
							  m_vertexStrideInBytes,
							  reinterpret_cast<const GLvoid*>(m_textureOffset[i]));
			GLError::glCheckError(__func__);
		}
	}
	/*
	//bind user defined attributes.
	//https://www.opengl.org/sdk/docs/man/xhtml/glGetActiveAttrib.xml
	if (getShaderProgId() > 0)
	{
		int index=0;
		#define maxLength 255
		GLenum  type=0;
		GLchar  name[maxLength]={0};
		GLint nActiveAttrib=0;
		glGetProgramiv(getShaderProgId(), GL_ACTIVE_ATTRIBUTES, &nActiveAttrib);
		for (int index = 0 ; index < nActiveAttrib ; index++)
		{//Only bind active attribute.
			GLsizei length=-1;
			GLint	size=-1;
			glGetActiveAttrib(getShaderProgId(), index,
						maxLength-1, &length, &size, &type, name);
			name[length]='\0';
			string strName(name);
			if ( m_attribs.find(strName)==m_attribs.end())
			{
				if (m_enableWarning)
				{
					char msg[1024];
					sprintf(msg,__func__": Attribute %s is not added in the GLVertexBufferObject.\n",name);
					//GLError::WarningMessage(msg);
					cout << msg;
				}
			}else{
				m_attribs[strName]->bind();
			}
		}
	}
	*/
	for (std::unordered_map<string, GLAttributeRef>::iterator it = m_attribs.begin();
		 it != m_attribs.end() ; ++it)
	{
		//check if the current attribute is active.
        if (it->second->isActive())
        {
            it->second->enable();
        }
	}
	GLError::glCheckError(__func__);
}

void GLVertexArrayObject::disableGLClientState()
{
	if (m_hasVertex)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	if (m_hasColor)
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}
	if (m_hasNormal)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	GLint maxTexImgUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxTexImgUnits);
	for (int i = 0 ; i < maxTexImgUnits; i++)
	{
		if (m_hasTextures[i])
		{
			glClientActiveTexture(GL_TEXTURE0+i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
	//unbind user defined attributes.
	for (std::unordered_map<string, GLAttributeRef>::iterator it = m_attribs.begin();
		 it != m_attribs.end() ; ++it)
	{
		//it->second->unbind();
		it->second->disable();
	}
	GLError::glCheckError(__func__);
}

void GLVertexArrayObject::draw(GLuint first/*=0*/, size_t count/*=0*/)
{
	checkEnable(count);

	glBindVertexArray(*m_arrayId);
	GLError::glCheckError("GLVertexArrayObject::draw(): glBindVertexArray() failed!");

	glDrawArrays(m_geotype, first, count);

	GLError::glCheckError("GLVertexArrayObject::draw(): glDrawArrays() failed!");
	glBindVertexArray(0);
}

void GLVertexArrayObject::drawInstanced(size_t primcount, GLuint first/*=0*/, size_t count/*=0*/)
{
	checkEnable(count);
	glBindVertexArray(*m_arrayId);
	GLError::glCheckError("GLVertexArrayObject::drawInstanced(): glBindVertexArray() failed!");

	glDrawArraysInstanced(m_geotype, first, count, primcount);

	GLError::glCheckError("GLVertexArrayObject::drawInstanced(): glDrawArrays() failed!");
	glBindVertexArray(0);
}

void GLVertexArrayObject::drawInstancedBaseInstance(size_t primcount, size_t baseInst, GLuint first/*=0*/, size_t count/*=0*/)
{
	checkEnable(count);
	glBindVertexArray(*m_arrayId);
	GLError::glCheckError("GLVertexArrayObject::drawInstancedBaseInstance(): glBindVertexArray() failed!");

    glDrawArraysInstancedBaseInstance(m_geotype, first, count, primcount, baseInst);

	GLError::glCheckError("GLVertexArrayObject::drawInstancedBaseInstance(): glDrawArraysInstancedBaseInstance() failed!");
	glBindVertexArray(0);
}


void GLVertexArrayObject::setTextureType( GLenum type/*=GL_FLOAT*/,GLenum texUnitId/*=GL_TEXTURE0*/ )
{
	if (!m_hasTextures[texUnitId])
	{
		std::stringstream ss;
		ss <<"GLVertexArrayObject::setTextureType():Please enable GL_TEXTURE"
		   <<texUnitId<<" first by calling GLVertexBufferObject::enableTexture()";

		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
	}
	m_textureType[texUnitId] = type;
	m_enabled = false;
}

void GLVertexArrayObject::setTextureComponentSize( GLuint s,GLenum texUnitId/*=0*/ )
{
	if (!m_hasTextures[texUnitId])
	{
		std::stringstream ss;
		ss <<"GLVertexArrayObject::setTextureComponentSize():Please enable GL_TEXTURE"
		   <<texUnitId
		   <<" first by calling GLVertexBufferObject::enableTexture()";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
	}
	m_textureComponentCount[texUnitId] = s;
	m_enabled = false;
}

void GLVertexArrayObject::setTextureOffsetInBytes( GLuint s, GLenum texUnitId/*=0*/ )
{
	if (!m_hasTextures[texUnitId])
	{
		std::stringstream ss;
		ss <<"GLVertexArrayObject::setTextureOffsetInBytes():Please enable GL_TEXTURE"
		   <<texUnitId << " first by calling GLVertexBufferObject::enableTexture()";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
	}
	m_textureOffset[texUnitId] = s;
	m_enabled = false;
}

void GLVertexArrayObject::activateAttribute(const std::string& name, bool flag)
{
    if (m_attribs.find(name) != m_attribs.end())
    {
        m_attribs[name]->setActive(flag);
    }
}

void GLVertexArrayObject::addAttribute(
				GLuint shaderProgId, const std::string& name, GLenum type,
				GLuint nComponents, GLuint stride/*=0*/, GLuint offset/*=0*/,
				bool normalized/*=0*/, const GLVertexBufferObjectRef vbo/*=NULL*/)
{
	if (m_attribs.size()>=g_maxAttrib)
	{
		stringstream ss;
		ss<<__func__<<":exceeds maximum vertex attributes("<<g_maxAttrib<<").\n";
		string msg = ss.str();
		std::cerr << msg;
		GLError::glCheckError(msg);
	}

	addAttribute(GLAttributeRef(new GLAttribute(shaderProgId, name, type, nComponents,
												stride,offset,normalized,
												m_enableWarning,vbo)
								)
				 );
}

void GLVertexArrayObject::addAttribute(
				GLuint shaderProgId, const std::string& name, GLenum type,
				GLuint nComponents, GLuint stride/*=0*/, GLuint offset/*=0*/,
				bool normalized/*=0*/, GLuint divisor/*=0*/,
                const GLVertexBufferObjectRef vbo/*=NULL*/)
{
	if (m_attribs.size()>=g_maxAttrib)
	{
		stringstream ss;
		ss<<__func__<<":exceeds maximum vertex attributes("<<g_maxAttrib<<").\n";
		string msg = ss.str();
		std::cerr << msg;
		GLError::glCheckError(msg);
	}
	
	addAttribute(GLAttributeRef(new GLAttribute(shaderProgId, name, type, nComponents,
												stride,offset,normalized,
												m_enableWarning,divisor, vbo)
								)
				 );
}

void GLVertexArrayObject::addAttribute( const GLAttributeRef attrib )
{
	GLenum type = attrib->getType();
	if ( g_GLTypeSizeInBytes.find(type)==g_GLTypeSizeInBytes.end())
	{
		GLError::ErrorMessage(string(__func__)+": cannot recognize the attribute type\n");
	}
	GLenum shaderProgId = attrib->getShaderProgId();
	string name = attrib->getName();
	GLint nComponents = attrib->getComponents();
	//if (m_attribs.find(name) == m_attribs.end())
	//{//add only when not present in m_attribs.
		m_shaderProgId = shaderProgId;
		m_attribs[name] = attrib;
		m_attribsSizeInBytes += (nComponents * g_GLTypeSizeInBytes[type]);
	//}
	m_enabled = false;
}

GLAttributeRef GLVertexArrayObject::getAttribute( const std::string& name )
{
	if(m_attribs.find(name)!= m_attribs.end())
		return m_attribs[name];
	else
		return GLAttributeRef((GLAttribute*)NULL);
}

//TODO: deprecated
GLuint GLVertexArrayObject::computeStride(){
	stringstream ss;
	string msg;
	ss << __func__;
	if (m_hasVertex && g_GLTypeSizeInBytes.find(m_vertexType)==g_GLTypeSizeInBytes.end())
	{
		ss << ": cannot recognize the m_vertexType\n";
		msg = ss.str();
		GLError::ErrorMessage(msg);
	}
	if (m_hasColor && g_GLTypeSizeInBytes.find(m_colorType)==g_GLTypeSizeInBytes.end())
	{
		ss << ": cannot recognize the m_colorType\n";
		msg = ss.str();
		GLError::ErrorMessage(msg);

	}
	if (m_hasNormal && g_GLTypeSizeInBytes.find(m_normalType)==g_GLTypeSizeInBytes.end())
	{
		ss <<__func__<< ": cannot recognize the m_normalType\n";
		msg = ss.str();
		GLError::ErrorMessage(msg);

	}
	GLuint bytesPerVertex  =(m_hasVertex  ? m_vertexComponetCount * g_GLTypeSizeInBytes[m_vertexType] : 0);
	bytesPerVertex += (m_hasColor   ? m_colorComponentCount * g_GLTypeSizeInBytes[m_colorType] : 0);
	bytesPerVertex += (m_hasNormal  ? 3 * g_GLTypeSizeInBytes[m_normalType] : 0);
	GLint maxTexImgUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxTexImgUnits);
	for (int i = 0 ; i < maxTexImgUnits; i++)
	{
		if (m_hasTextures[i] && g_GLTypeSizeInBytes.find(m_textureType[i])==g_GLTypeSizeInBytes.end())
		{
			ss <<__func__<< ": cannot recognize the m_textureType in texture unit %d\n";
			msg = ss.str();
			GLError::ErrorMessage(msg);
		}
		bytesPerVertex += (m_hasTextures[i] ? m_textureComponentCount[i] * g_GLTypeSizeInBytes[m_textureType[i]] : 0);
	}

	bytesPerVertex += m_attribsSizeInBytes;
	//Considering alignment. The computed bytesPerVertex of type T maybe
	//smaller than sizeof(T). We should use the larger one.
	//bytesPerVertex = bytesPerVertex < sizeof(T) ? sizeof(T) : bytesPerVertex;
	bytesPerVertex = bytesPerVertex < m_vertexStrideInBytes ?
							m_vertexStrideInBytes : bytesPerVertex;
	return bytesPerVertex;
}

void GLVertexArrayObject::checkEnable(size_t &count)
{
	if (!m_enabled)
	{
		GLError::ErrorMessage(string("Please enable GLVertexArrayObject first, by calling enable(), since your last modification of VAO!"));
	}

	if(count == 0)
	{//If count is 0 or not specified, we will figure it out by counting all available
		//vertices in the vertex buffer.
		//count how many floats the attributes of a vertex take up.
		if(m_vboDefault)
			count = m_vboDefault->getVertexCount();//(dataBufferSizeInBytes/bytesPerVertex);
		else
		{
			//check if any attribute exists.
			std::unordered_map<std::string, GLAttributeRef>::iterator it = m_attribs.begin();
			for ( ;it != m_attribs.end() ; ++it)
			{
				if (it->second->m_vbo)
				{
					break;
				}

			}
			if (it == m_attribs.end())
			{
				GLError::ErrorMessage(string("GLVertexBufferObject::draw():Empty VAO detected! No VBO attached to this VAO! Give VAO a content before calling draw()."));
			}
			count = it->second->m_vbo->getVertexCount();
		}
		if (count <= 0)
		{
			GLError::ErrorMessage(string("GLVertexBufferObject::draw() count<=0.\nMake sure you feed the VBO with correct number of data."));
		}
	}
}

GLint GLVertexArrayObject::g_maxAttrib = 0;

}
