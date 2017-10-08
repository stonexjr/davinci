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

#ifndef _GLPIXELBUFFEROBJECT_H_
#define _GLPIXELBUFFEROBJECT_H_

#include "GLBufferObject.h"
#include <GLTexture1D.h>
#include <GLTexture2D.h>
#include <GLFrameBufferObject.h>

#pragma once
namespace davinci{
    //Functions that perform an upload operation, a pixel unpack, will use
    //the buffer object bound to the target GL_PIXEL_UNPACK_BUFFER.
    //Functions that perform a download operation, a pixel pack, will use
    //the buffer object bound to the GL_PIXEL_PACK_BUFFER.
    //http://www.opengl.org/wiki/Pixel_Buffer_Object#Process
    //
    class GLPixelBufferObject : public GLBufferObject
    {
        public:
            //******************************************************************
            // Create PBO and corresponding device memory is allocated upon
            // invocation of constructor.
            // Parameter: GLenum usage:GL_STATIC_READ,GL_STREAM_DRAW_ARB,GL_DYNAMIC_COPY.
            // Parameter: int x : The width of the image that PBO represents.
            // Parameter: int y : The height of the image that PBO represents.
            // Parameter: int format : The number of pixel components(3,4)
            // Parameter: int type : The type size(in byte) of each component.
            //******************************************************************
            GLPixelBufferObject(int x=16, int y=16, GLenum format = GL_RGBA,
                                GLenum type  = GL_UNSIGNED_BYTE, 
                                GLenum usage = GL_STATIC_READ);

            ~GLPixelBufferObject(void);
            void resize(int w, int h);
            int  getWidth() { return m_width;}
            int  getHeight(){ return m_height;}
            int  getType()  { return m_type;}
            int  getFormat(){ return m_format;}
            //target:
            //GL_PIXEL_PACK_BUFFER: data flows into PBO;
            //GL_PIXEL_UNPACK_BUFFER: data flows out of PBO;
            void bindBufferObject(GLenum target);
            void copyFrom(GLTexture2d &tex);
            void copyFrom(GLTexture1d &tex);
            void copyTo(GLTexture2d &tex);
            //copy the entire content of PBO to the CPU side buffer.
            //user is responsible to make sure the CPU side buffer is 
            //big enough to hold the PBO content.
            void copyTo(void* pCPUDest);
            //copy pixel contents from default framebuffer to PBO
            //whichbuffer: GL_FRONT(default), GL_LEFT, GL_RIGHT, GL_BACK.
            void copyFrom(GLenum whichBuffer=GL_FRONT);
            //copy pixel contents from FBO color attachment(0~15)
            void copyFrom(GLFrameBufferObject &fbo, GLuint colorAttachId);
            //copy content from CPU side to PBO(GPU side).
            //If the PBO has insufficient storage space, if will trigger
            //an GLError.
            void copyFrom(void* pSrc, int sizeInBytes);
            //PBO provides a memory mapping mechanism to map the OpenGL 
            //controlled buffer object to the client's memory address space.
            // So, the client can modify a portion of the buffer object or 
            // the entire buffer.
            //target:
            //GL_PIXEL_PACK_BUFFER: data flows into PBO;
            //GL_PIXEL_UNPACK_BUFFER: data flows out of PBO;
            //access: GL_READ_ONLY_ARB, GL_WRITE_ONLY_ARB or GL_READ_WRITE_ARB.
            void* getMappedAddress(GLenum target, GLenum access);
            //target:
            //GL_PIXEL_PACK_BUFFER: data flows into PBO;
            //GL_PIXEL_UNPACK_BUFFER: data flows out of PBO;
            GLboolean unMappedAddress(GLenum target);
        private:
            GLuint getBufferSize() { return m_width*m_height*getComponents(m_format)*getTypeSize(m_type);}
            //Get number of components from GL format like GL_RGBA returns 4.
            GLenum getComponents(GLenum format);
            //Get size of each component of pixel from GL type like GL_UNSIGNED_BYTE returns 1
            GLenum getTypeSize(GLuint type);
            GLuint  m_width, m_height;
            GLenum  m_format;
            GLenum  m_type;
    };
    typedef std::shared_ptr<GLPixelBufferObject> GLPixelBufferObjectRef;
}
#endif