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

//#include <stdio.h>
#include <cstdio>
#include <GL/glew.h>
#include "GLFrameBufferObject.h"
#include "GLError.h"
#ifdef ENABLE_QT
#include <QOpenGLWidget>
#endif

namespace davinci{

GLFrameBufferObject::GLFrameBufferObject(GLenum target/*=GL_FRAMEBUFFER*/)
    :m_target(target),m_isBinded(false)
{
    glGenFramebuffers(1, &m_fboId);

    GLint nMaxAttachs=0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &nMaxAttachs);
	m_attachedTextureND.resize(nMaxAttachs);
    GLError::glCheckError("GLFrameBufferObject(): glGenFramebuffers() failed!");
}

GLFrameBufferObject::~GLFrameBufferObject(void)
{
    deleteBuffer();
    m_attachedTextureND.clear();
    GLError::glCheckError("~GLFrameBufferObject(): glDeleteFramebuffers() failed!");
}

void GLFrameBufferObject::deleteBuffer()
{
    if (m_fboId){
        glDeleteFramebuffers(1, &m_fboId);
        m_fboId = 0;
    }
}

void GLFrameBufferObject::bind()
{
    glBindFramebuffer(m_target, m_fboId);
    m_isBinded = true;
}

void GLFrameBufferObject::unbind()
{
    glBindFramebuffer(m_target, 0);
    m_isBinded = false;
}

void GLFrameBufferObject::attachColorBuffer(GLuint attachId, GLuint tex2dId)
{
    if(!m_isBinded) 
        GLError::ErrorMessage(string(__func__)+string(": Please bind the GLFrameBufferObject before attaching color buffer."));

    GLenum GLAttachmentID = GL_COLOR_ATTACHMENT0+attachId;
    glFramebufferTexture2D(m_target, GLAttachmentID, GL_TEXTURE_2D, tex2dId, 0);
	GLError::glCheckError(string(__func__)+": attachColorBuffer() failed!");
}
/*
void GLFrameBufferObject::attachColorBuffer( GLuint attachId, GLTexture2d &tex2d )
{
    attachColorBuffer(attachId, tex2d.getTextureId());
}
*/

void GLFrameBufferObject::attachColorBuffer( GLuint attachId, const GLTexture2DRef& tex2dRef )
{
	if (!tex2dRef)
	{
		cerr << __func__<< ": attached texture2d is NULL!\n";
		exit(1);
	}
    attachColorBuffer(attachId, tex2dRef->getTextureId());
    m_attachedTextureND[attachId] = &*tex2dRef;
}

void GLFrameBufferObject::attachDepthBuffer( const GLRenderbufferObjectRef &buffer )
{
	if (!buffer)
	{
		cerr << __func__<< ": attached renderbuffer is NULL!\n";
		exit(1);
	}
    attachDepthBuffer(*buffer);
}

void GLFrameBufferObject::attachDepthBuffer( const GLTexture2DRef &tex2d )
{
	if (!tex2d)
	{
		cerr << __func__<< ": attached texture2d is NULL!\n";
		exit(1);
	}

    attachDepthBuffer(*tex2d);
}

void GLFrameBufferObject::attachTextureND( GLuint attachId, const GLTextureAbstract& tex, int zLayer/*=0*/, int mipLevel/*=0*/ )
{
    if(!m_isBinded) 
        GLError::ErrorMessage(string(__func__)+string(": Please bind the GLFrameBufferObject before attaching color buffer."));
	if (attachId>=m_attachedTextureND.size())
	{
		stringstream ss;
		ss << __func__ <<": attachId="<<attachId<<">= maximum attachment id!";
		string msg = ss.str();
		GLError::ErrorMessage(msg);
	}
    GLenum GLAttachmentID = GL_COLOR_ATTACHMENT0+attachId;

    if (tex.getTarget() == GL_TEXTURE_1D)
    {
        glFramebufferTexture1D(m_target, GLAttachmentID, GL_TEXTURE_1D, tex.getTextureId(), mipLevel);
    }else if (tex.getTarget() == GL_TEXTURE_2D)
    {
		glFramebufferTexture2D(m_target, GLAttachmentID, GL_TEXTURE_2D, tex.getTextureId(),mipLevel);
    }else if (tex.getTarget() == GL_TEXTURE_3D)
    {
		glFramebufferTexture3D(m_target,GLAttachmentID,GL_TEXTURE_3D,tex.getTextureId(),mipLevel,zLayer);
    }else{
		GLError::ErrorMessage(string(__func__)+"Unrecognized texture target, make sure it is one of GL_TEXTURE_1D, GL_TEXTURE_2D and GL_TEXTURE_3D.");
	}
	m_attachedTextureND[attachId] = const_cast<GLTextureAbstract*>(&tex);
	GLError::glCheckError(string(__func__)+": failed!");
}

vec3f GLFrameBufferObject::getTextureDimension(int attachId/*=-1*/) const
{
    GLTextureAbstract* attachedImage=NULL;

	if (-1 == attachId)
	{
		for (int i = 0 ; i < m_attachedTextureND.size() ; i++)
		{
			if (m_attachedTextureND[i])
			{//break upon the first hit of valid attached texture.
				attachedImage = m_attachedTextureND[i];
				break;
			}
		}
	}else if (attachId < m_attachedTextureND.size())
	{
		attachedImage = m_attachedTextureND[attachId];
	}else{
		stringstream ss;
		ss << "GLFrameBufferObject::getImageDimension(): no texture attached to slot "<<attachId;
		string msg = ss.str();
        GLError::ErrorMessage(msg);
		return vec3f(0,0,0);
	}

    if (attachedImage)
    {
       return vec3f(attachedImage->getWidth(), attachedImage->getHeight(), attachedImage->getDepth());
    }else{
		GLError::ErrorMessage(string("GLFrameBufferObject::getImageDimension(): no texture attached to current FBO! Cannot query the image size."));
		return vec3f(0,0,0);
    }
}

void GLFrameBufferObject::attachDepthBuffer(const GLRenderbufferObject &buffer)
{
    if(!m_isBinded) 
        GLError::ErrorMessage(string(__func__)+string(": Please bind the GLFrameBufferObject before attaching depth buffer."));
    glFramebufferRenderbuffer(m_target, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, buffer.getBufferId());
}
//https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glFramebufferTexture.xml
void GLFrameBufferObject::attachDepthBuffer( const GLTexture2d &tex2d )
{
    if(!m_isBinded) 
        GLError::ErrorMessage(string(__func__)+string(": Please bind the GLFrameBufferObject before attaching depth buffer."));
	/*
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                         tex2d.getTextureId(), 0);
						 */
	glFramebufferTexture2D(m_target,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, tex2d.getTextureId(),0);
	GLError::glCheckError(string(__func__)+": attachDepthBuffer() failed!");
}

void GLFrameBufferObject::enableMultipleRenderTarget(const GLuint *targetList, GLuint count )
{
    if(!m_isBinded) 
        GLError::ErrorMessage(string("GLFrameBufferObject::enableMultipleRenderTarget(): Please bind the GLFrameBufferObject before enabling multiple render targets."));

    GLenum *buffers = new GLenum[count];
    if (!buffers){
        GLError::ErrorMessage(string("Memory allocation error!"));
    }
    for (GLuint i = 0; i < count ; i++){
        buffers[i] = GL_COLOR_ATTACHMENT0+ targetList[i];

        if (buffers[i]>GL_COLOR_ATTACHMENT15){
            stringstream ss;
            ss << "The value of buffer[" << i << "]=" << buffers[i] << " exceed the maximum color attachment id!";
            GLError::ErrorMessage(ss.str());
        }
    }
    glDrawBuffers(count, buffers);
    delete [] buffers;
}

void GLFrameBufferObject::disableMultipleRenderTarget()
{
    //glDrawBuffer(GL_BACK);//GL_BACK in not avail. in GLFrameBufferObject.
}

void GLFrameBufferObject::printFramebufferLimits() {

    int res;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &res);
    printf("Max Color Attachments: %d\n", res);

#if defined(__APPLE__) || defined(MACOSX)
    glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &res);
    printf("Max Framebuffer Width: %d\n", res);

    glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &res);
    printf("Max Framebuffer Height: %d\n", res);

    glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &res);
    printf("Max Framebuffer Samples: %d\n", res);

    glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &res);
    printf("Max Framebuffer Layers: %d\n", res);
#endif

}
void GLFrameBufferObject::printFramebufferInfo() {

    int res, i = 0;
    GLint buffer;

    glBindFramebuffer(m_target,m_fboId);

    do {
        glGetIntegerv(GL_DRAW_BUFFER0+i, &buffer);

        if (buffer != GL_NONE) {

            printf("Shader Output Location %d - color attachment %d\n",
                i, buffer - GL_COLOR_ATTACHMENT0);

            glGetFramebufferAttachmentParameteriv(m_target, buffer,
                GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &res);
            printf("\tAttachment Type: %s\n",
                res==GL_TEXTURE?"Texture":"Render Buffer");
            glGetFramebufferAttachmentParameteriv(m_target, buffer,
                GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &res);
            printf("\tAttachment object name: %d\n",res);
        }
        ++i;

    } while (buffer != GL_NONE);

    glBindFramebuffer(m_target,0);
}

bool GLFrameBufferObject::checkFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(m_target);
    switch(status)
    {
        case GL_FRAMEBUFFER_COMPLETE:
            printf( "Framebuffer complete.\n" );
            return true;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            GLError::ErrorMessage(string("[ERROR] Framebuffer incomplete: Attachment is NOT complete.\n") );
            return false;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            GLError::ErrorMessage(string("[ERROR] Framebuffer incomplete: No image is attached to FBO.\n"));
            return false;

        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            GLError::ErrorMessage(string("[ERROR] Framebuffer incomplete: Attached images have different dimensions.\n"));
            return false;

        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            GLError::ErrorMessage(string("[ERROR] Framebuffer incomplete: Color attached images have different internal formats.\n") );
            return false;

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            GLError::ErrorMessage(string("[ERROR] Framebuffer incomplete: Draw buffer.\n") );
            return false;

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            GLError::ErrorMessage(string("[ERROR] Framebuffer incomplete: Read buffer.\n") );
            return false;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            GLError::ErrorMessage(string("[ERROR] Unsupported by FBO implementation.\n") );
            return false;

        default:
            GLError::ErrorMessage(string("[ERROR] Unknown error.\n") );
            return false;
    }
}

/*
GLTexture2DRef GLFrameBufferObject::getAttachedColorTexture( int attachId )
{
    if (attachId < m_attachedTextureND.size())
    {
        return m_attachedTextureND[attachId];
    }else{
        return NULL;
    }
}
*/

GLTextureAbstract* GLFrameBufferObject::getAttachedTextureND( int attachId )
{
	if (attachId < m_attachedTextureND.size())
	{
		return m_attachedTextureND[attachId];
	}else{
		stringstream ss;
		ss << __func__<<": no texture attached to slot "<<attachId;
		string msg = ss.str();
		GLError::ErrorMessage(msg);
		return NULL;
	}
}

#ifdef ENABLE_QT
void GLFrameBufferObject::bindQtDefaultFramebuffer(QOpenGLWidget *widget)
{
    glBindFramebuffer(GL_FRAMEBUFFER, widget->defaultFramebufferObject());
}
#endif


}//end of namespace
