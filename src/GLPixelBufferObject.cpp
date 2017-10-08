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
#include <sstream>
#include "GLPixelBufferObject.h"
#include "GLError.h"
#include "DError.h"
using namespace davinci;

GLPixelBufferObject::GLPixelBufferObject(
    int x/*=16*/, int y/*=16*/, GLenum format/*=GL_RGBA*/,
    GLenum type/*=GL_UNSIGNED_BYTE*/, GLenum usage/*=GL_STATIC_READ*/)
    :GLBufferObject(GL_PIXEL_PACK_BUFFER, usage)
    ,m_width(x),m_height(y),m_format(format),m_type(type)
    //m_target will be changed based on the direction of pixel stream flow.
    //GL_PIXEL_PACK_BUFFER_ARB: bind the buffer object as input buffer
    //GL_PIXEL_UNPACK_BUFFER_ARB: bind the buffer object as output buffer 
{
    m_reservedBytes = getBufferSize();
    GLError::purgePreviousGLError();
    GLBufferObject::bindBufferObject();
    glBufferData(m_target, m_reservedBytes, NULL, m_usage);//allocate but no initialization.
    unbindBufferObject();
    GLError::glCheckError(__func__);
}

GLPixelBufferObject::~GLPixelBufferObject(void)
{
    deleteBuffer();
}

void GLPixelBufferObject::resize( int w, int h )
{
    GLError::purgePreviousGLError();
    deleteBuffer();
    m_width = w;
    m_height= h;
    m_reservedBytes = getBufferSize();
    glGenBuffers(1, &m_id);
    GLBufferObject::bindBufferObject();
    //reallocation, previous content is undefined.
    glBufferData(m_target, m_reservedBytes, NULL, m_usage);
    unbindBufferObject();
    GLError::glCheckError(__func__);
}

void GLPixelBufferObject::copyFrom(GLTexture2d &tex)
{
    if (tex.getWidth()>m_width || tex.getHeight()>m_height)
    {//in case PBO has smaller buffer size to store the texture content.
        resize(tex.getWidth(), tex.getHeight());
        cerr << __func__ <<" PBO initial size is smaller than the texture size.\n"
            << "resizing PBO.\n";
    }
    // bind the texture and PBO
    tex.bindTexture();
    GLError::purgePreviousGLError();
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_id);
    // copy pixels from texture object to PBO
    // Use offset instead of pointer.
    glGetTexImage(tex.getTarget(), 0, tex.getFormat(), tex.getType(), NULL);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    GLError::glCheckError(string(__func__)+string("(tex)"));
    tex.unbindTexture();
}

void GLPixelBufferObject::copyFrom(GLTexture1d &tex)
{
    if (tex.getWidth()*1 > m_width * m_height)
    {//in case PBO has smaller buffer size to store the texture content.
        resize(tex.getWidth(), 1);
        cerr << __func__ <<" PBO initial size is smaller than the texture size.\n"
            << "resizing PBO.\n";
    }
    if(tex.getFormat() != getFormat()
        || tex.getType() != getType())
    {
        GLError::ErrorMessage(string(__func__)+": texture1d format or type does not match with the PBO's!\n");
    }

    // bind the texture and PBO
    tex.bindTexture();
    GLError::purgePreviousGLError();
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_id);
    // copy pixels from texture object to PBO
    // Use offset instead of pointer.
    glGetTexImage(tex.getTarget(), 0, tex.getFormat(), tex.getType(), NULL);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    GLError::glCheckError(string(__func__)+"(tex)");
    tex.unbindTexture();
}
void GLPixelBufferObject::copyFrom(GLFrameBufferObject &fbo, GLuint colorAttachId)
{
    if (colorAttachId<0 || colorAttachId >15){
        GLError::ErrorMessage(string(__func__)+string("(fbo,cAttachId) attachId out of bound."));
    }
#ifdef ENABLE_CUDA_GL_INTEROP
    if (m_cudaMappedPtr){
        //Make sure the PBO mapped pointer is released from CUDA before
        //it can be used by OpenGL.
        std::stringstream ss;
        ss << "############"<<__func__<<"############\n"
           << "Call unmapCudaPointer() to release PBO mapped pointer\n"
           << "from CUDA  before using PBO in OpenGL!\n"
           << "################################################\n";
        std::string msg = ss.str();
        cerr << msg;
        GLError::ErrorMessage(msg);
    }
#endif
    // bind the texture and PBO
    fbo.bind();
    GLError::purgePreviousGLError();
    glReadBuffer(GL_COLOR_ATTACHMENT0+ colorAttachId);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_id);
    // copy pixels from FBO attachment(attachId) to PBO
    glReadPixels(0, 0, m_width, m_height, m_format, m_type, NULL);
    GLError::glCheckError(string(__func__)+string("(fbo,cAttachId)"));
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    fbo.unbind();
}

void GLPixelBufferObject::copyFrom( GLenum whichBuffer/*=GL_FRONT*/ )
{
    glReadBuffer(whichBuffer);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_id);
    glReadPixels(0, 0, m_width, m_height, m_format, m_type, NULL);
    GLError::glCheckError(string(__func__)+string("(whichBuffer)"));
}

void GLPixelBufferObject::copyFrom( void* pCPUSrc, int sizeInBytes )
{
    void* pDest = getMappedAddress(GL_PIXEL_PACK_BUFFER, GL_WRITE_ONLY);
    if (!pDest)
    {
        GLError::ErrorMessage(string(__func__)+string("pDest==NULL!"));
    }
    memcpy(pDest, pCPUSrc, sizeInBytes);
}

void GLPixelBufferObject::bindBufferObject(GLenum target)
{
    m_target = target;
    GLBufferObject::bindBufferObject();
}

void* GLPixelBufferObject::getMappedAddress( GLenum target, GLenum access )
{
    if (target!= GL_PIXEL_PACK_BUFFER || target != GL_PIXEL_UNPACK_BUFFER)
    {
        GLError::ErrorMessage(string(__func__)+
            string(": target parameter expects GL_PIXEL_PACK_BUFFER or GL_PIXEL_UNPACK_BUFFER."));
    }
    void* address = glMapBuffer(target, access);
    return address;
}

GLboolean GLPixelBufferObject::unMappedAddress(GLenum target)
{
    return glUnmapBuffer(target);
}

void GLPixelBufferObject::copyTo( void* pCPUDest )
{
    void* pSrc = getMappedAddress(GL_PIXEL_UNPACK_BUFFER, GL_READ_ONLY);
    if (!pSrc)
    {
        GLError::ErrorMessage(string(__func__)+string("pSrc==NULL!"));
    }
    memcpy(pCPUDest, pSrc, getBufferSize());
}


void GLPixelBufferObject::copyTo(GLTexture2d &tex)
{
    // bind the texture and PBO
    tex.bindTexture();
    GLError::purgePreviousGLError();
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_id);
    // copy pixels from PBO to texture object
    // Use offset instead of pointer.
    if (tex.getWidth()!=m_width || tex.getHeight()!=m_height)
    {
        std::stringstream ss;
        ss << __func__ <<": the PBO size["<<m_width<<","<<m_height
           <<"] does not match with the texture size["<<tex.getWidth()
           <<","<<tex.getHeight()<<endl;
        std::string msg = ss.str();
        GLError::ErrorMessage(msg);
    }
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,tex.getWidth(),tex.getHeight(),
                    tex.getFormat(), tex.getType(), NULL);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    GLError::glCheckError(string(__func__)+string("(tex)"));
    tex.unbindTexture();
}

GLenum GLPixelBufferObject::getComponents( GLenum format )
{
    switch (format)
    {
        case GL_RED:
        case GL_RED_INTEGER:
        case GL_GREEN:
        case GL_BLUE:
        case GL_ALPHA:
        case GL_STENCIL_INDEX:
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_STENCIL:
            return 1;
            break;
        case GL_RG:
        case GL_RG_INTEGER:
        case GL_BGR:
        case GL_RGB:
        case GL_RGB_INTEGER:
        case GL_BGR_INTEGER:
            return 3;
            break;
        case GL_RGBA:
        case GL_BGRA:
        case GL_RGBA_INTEGER:
        case GL_BGRA_INTEGER:
            return 4;
            break;
        default:
            GLError::ErrorMessage(string(__func__)+" Unsupported GLFormat");
    }
    return -1;
}

GLenum GLPixelBufferObject::getTypeSize( GLenum type)
{
    switch (type)
    {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);//1;
            break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            return sizeof(short);
            break;
        case GL_FLOAT:
        case GL_INT:
            return sizeof(float);//4
            break;
        case GL_DOUBLE:
            return sizeof(double);//8
        default:
            GLError::ErrorMessage(string(__func__)+" Unsupported GLType");
    }
    return -1;
}
