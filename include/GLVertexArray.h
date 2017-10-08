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
//CREATED: 2013-10-23
//UPDATED: 2015-06-13

#ifndef _GL_VERTEX_ARRAY_H_
#define _GL_VERTEX_ARRAY_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include "GLAttribute.h"
#include "GLError.h"

namespace davinci{

class GLVertexArrayObject
{
public:
	//Vertex Array is the descriptor of vertex attribute format.
	//Support OpenGL Compatibility mode and Core mode up to 4.3.
	//The vertex/color/normal/texture array are organized into an array of
	//structure(interleaved array). The boundary is specified by the vertex/color/normal/texture
	// offset and stride. http://www.opengl.org/wiki/Client-Side_Vertex_Arrays
	//Vertex attribute data will not be upload to GPU until the invocation of bind().
	//which call upload() to transfer data.
	//GLenum geotype:
	//Specifies what kind of primitives are constructed
	//and how the array elements construct those primitives.
	//It could be GL_TRIANGLES, GL_POLYGON, GL_QUAD, etc.
	//GLenum arrayId: If 0 is set, GLVertexArrayObject will create a new VAO instance.
	//or else the GLVertexArrayObject will belongs to the same VAO instance whose id is arrayId.
	//GLenum usage:
	//How user will be reading/writing from/to the buffer,
	// and how often the user will be changing it relative to the use of data.
	//GL_STATIC_DRAW, GL_STATIC_READ,
	//GL_STATIC_COPY, GL_DYNAMIC_DRAW,
	//GL_DYNAMIC_READ, GL_DYNAMIC_COPY
	//GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY
	GLVertexArrayObject(GLenum geotype,std::shared_ptr<GLenum> arrayId=std::shared_ptr<GLenum>((GLenum*)NULL),
                            GLenum usage=GL_STATIC_DRAW, bool warning=false);

	~GLVertexArrayObject(void);

	void enableVertex(bool flag){ m_hasVertex = flag; m_enabled = false;}
	void enableNormal(bool flag){ m_hasNormal = flag;m_enabled = false;}
	void enableColor(bool flag){ m_hasColor = flag;m_enabled = false;}
	void enableTexture(bool flag, GLenum texUnitId=0){ m_hasTextures[texUnitId] = flag;m_enabled = false;}

	void setVertexType(GLenum type=GL_FLOAT){ m_vertexType = type;m_enabled = false;}
	void setColorType(GLenum type=GL_FLOAT){ m_colorType = type;m_enabled = false;}
	void setNormalType(GLenum type=GL_FLOAT){ m_normalType = type;m_enabled = false;}
	void setTextureType(GLenum type=GL_FLOAT,GLenum texUnitId=0);//{ m_textureType = type;}

	void setVertexComponetSize(GLuint s){ m_vertexComponetCount = s;m_enabled = false;}
	void setColorComponentSize(GLuint s){ m_colorComponentCount = s;m_enabled = false;}
	//texUnitId: GL_TEXTURE0, GL_TEXTURE1,...GL_TEXTURE031
	void setTextureComponentSize(GLuint s, GLenum texUnitId=0);
	//The stride parameter defines the amount of bytes required
	//to get from one vertex to the next,
	void setVertexStrideInBytes(GLuint s){ m_vertexStrideInBytes = s;m_enabled = false;}

	//If you use GLVertexBufferObject as VBO, s is the offset into a
	//currently bound buffer object. If you use it as Vertex Array.
	//the offset should be the pointer to the client data buffer.
	void setVertexOffsetInBytes(GLuint s){ m_vertexOffsetInBytes = s;m_enabled = false; }
	void setNormalOffsetInBytes(GLuint s){ m_normalOffsetInBytes = s;m_enabled = false;}
	void setColorOffsetInBytes(GLuint s){ m_colorOffsetInBytes = s;m_enabled = false;}
	//texUnitId: 0<==>GL_TEXTURE0, 1<==>GL_TEXTURE1,...31 <==> GL_TEXTURE31
	void setTextureOffsetInBytes(GLuint s, GLenum texUnitId=0);//{}

	bool getEnableWarning() const { return m_enableWarning; }
	void setEnableWarning(bool val) { m_enableWarning = val; }
	//Add additional user defined attribute for the vertex.
	//Note:
	//On one hand, in lower GLSL version like 1.1 or compatibility mode of higher GLSL.
	//User can specified customized vertex attribute using the keyword
	//"attribute" in GLSL shader. addAttribute() is used to specified
	//the layout(component size, byte size, byte offset) of the corresponding
	//attribute in the currently binded GLVertexBufferObject.
	//Please refer to sample code: LilyTutorial3-VBO-IBO-Attribute-Shader
	//
	//On the other hand, the core mode of GLSL 4.0 or higher requires that standard vertex
	//attribute such as position, color, normal and texture be treated as
	//"in" attribute in shader. And the keyword "attribute", default gl_ModelViewProjectionMatrix
	//etc. are deprecated(even no longer available) in the shader.
	//Therefore, addAttribute can also be used to specify standard vertex
	//attributes.
	//Please refer to sample code: LilyTutorial4-VBO-IBO-Attribute-Shader4_3
	void addAttribute(
					GLuint shaderProgId, const std::string& name,GLenum type,
					GLuint nComponents, GLuint stride=0, GLuint offset=0, bool normalized=0,
					const GLVertexBufferObjectRef vbo = GLVertexBufferObjectRef((GLVertexBufferObject*)NULL) );
	void addAttribute(
					GLuint shaderProgId, const std::string& name,GLenum type,
					GLuint nComponents, GLuint stride=0, GLuint offset=0, bool normalized=0,
                    GLuint divisor=0,
					const GLVertexBufferObjectRef vbo = GLVertexBufferObjectRef((GLVertexBufferObject*)NULL) );
	void addAttribute(const GLAttributeRef attrib);

    //toggle attribute 'name'.
    void activateAttribute(const std::string& name, bool flag);
	GLAttributeRef getAttribute(const std::string& name);
	//Return the id of the associated shader program if set, since GLVertexBufferObject
	//can work w/o shader. Can only be set via AddAttribute().
	GLint getShaderProgId() const { return m_shaderProgId; }
	void  setShaderProgId(GLuint progId);
	//Attach a default VBO if you want to use OpenGL compatibility mode to set up
	//Vertex Array Object like using glEnableClientState, glVertexPointer,
	//glColorPointer,glNormalPointer,glTexCoordPointer.
	void attachDefaultVBO(const GLVertexBufferObjectRef& vbo){m_vboDefault=vbo;m_enabled = false;}
	GLVertexBufferObjectRef getAttachedDefaultVBO(){ return m_vboDefault;}
	//enable current VBO and enable its corresponding client states
	//(vertex/color/normal/texture) if GL compatibility mode is used or
	//enable glVertexPointer() if core mode is used.
	//Call this function once you have finished specifying the vertex format
	//and vertex attribute data.
	void enable();
	//disable current VBO and disable client states(vertex/color/normal/texture)
	//if GL compatibility mode is used or disable glVertexPointer() if core mode is used.
	void disable();
	bool isEnabled(){ return m_enabled;}
	//When draw() is called, it uses count sequential elements from each
	//enabled array to construct a sequence of geometric primitives,beginning
	//with element first. Or you can treat count as the number of vertices
	//involved in the draw().
	//if count is 0, simply draw the entire data.
	void draw(GLuint first=0, size_t count=0);
	void drawInstanced(size_t primcount, GLuint first=0, size_t count=0);
	void drawInstancedBaseInstance(size_t primcount, size_t baseInst, GLuint first=0, size_t count=0);

	std::shared_ptr<GLenum> getArrayId() const { return m_arrayId; }
protected:
	void checkEnable(size_t &count);
	//Reserve required device memory and then upload vertex attributes to GPU.
	//void upload();
private:
	//http://www.opengl.org/wiki/Vertex_Formats
	//http://www.opengl.org/wiki/Client-Side_Vertex_Arrays
	//************************************
	// Method:    enableGLClientState
	// Access:    private
	// Call this method if OpenGL compatibility mode is your only choice,
	// where gl[Vertex/Color/Normal/Texture]Pointer() calls are invoked	// .
	//************************************
	void enableGLClientState();
	void disableGLClientState();
	GLuint computeStride();
	void sanityCheckVBO();

	GLVertexBufferObjectRef m_vboDefault;

	std::shared_ptr<GLenum> m_arrayId;//vertex array id.
	//GLuint m_bindingIndex;
	GLenum m_geotype;
	bool m_enabled;
	bool m_hasVertex;
	bool m_hasNormal;
	bool m_hasColor;
	std::vector<bool> m_hasTextures;
	int  m_TextureCount;//number of active textures.

	GLenum m_vertexType;
	GLenum m_colorType;
	GLenum m_normalType;
	std::vector<GLenum> m_textureType;

	GLuint m_vertexComponetCount;
	GLuint m_colorComponentCount;
	std::vector<GLuint> m_textureComponentCount;

	GLuint m_vertexStrideInBytes;
	std::vector<GLuint> m_textureStride;

	GLuint m_vertexOffsetInBytes;
	GLuint m_normalOffsetInBytes;
	GLuint m_colorOffsetInBytes;
	std::vector<GLuint> m_textureOffset;;

	std::unordered_map<std::string, GLAttributeRef> m_attribs;
	GLsizeiptr     m_attribsSizeInBytes;
	//The id of the associated shader program if set.
	//Can merely be set via AddAttribute().
	GLint	m_shaderProgId;
	bool    m_enableWarning;

//public:
	std::unordered_map<GLenum, GLuint> g_GLTypeSizeInBytes;
	float m_GLVersion;
    static GLint g_maxAttrib;
};

typedef std::shared_ptr<GLVertexArrayObject> GLVertexArrayObjectRef;

}//end of namespace lily
#endif
