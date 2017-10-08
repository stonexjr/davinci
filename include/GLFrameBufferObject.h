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
//UPDATED: 2014-02-07
#ifndef _GLFRAMEBUFFEROBJECT_H_
#define _GLFRAMEBUFFEROBJECT_H_

#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif
#include <memory>

#include "vec3f.h"
#include "GLRenderBufferObject.h"
#include "GLTexture2D.h"
//For information on how to utilize FBO, please refer to
//tutorial 7 and 8.
//or 
//http://www.songho.ca/opengl/gl_fbo.html
//
//http://www.opengl.org/wiki/Framebuffer_Object
// 
//https://www.opengl.org/wiki/GLSL_Predefined_Variables_By_Version
//
// http://gamedev.stackexchange.com/questions/49667/what-is-the-recommended-way-to-output-values-to-fbo-targets-opengl-3-3-glsl

class QOpenGLWidget;

namespace davinci{

    class GLFrameBufferObject
    {
        public:
             GLFrameBufferObject(GLenum target=GL_FRAMEBUFFER);
            ~GLFrameBufferObject(void);
            void bind();
            void unbind();
#ifdef ENABLE_QT
            void bindQtDefaultFramebuffer(QOpenGLWidget* widget);
#endif
            //Attach a 2d texture to FBO color attachment point
            //attachId: integer value from 0,1,2....glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS)-1.
            // 0 : GL_COLOR_ATTACHMENT0_EXT
            // 1 : GL_COLOR_ATTACHMENT1_EXT
            // and so on....
            //Each draw buffers must either specify color attachment points
            //that have images attached or must be GL_NONE. 
            void attachColorBuffer(GLuint attachId, GLuint tex2dId);
            void attachColorBuffer(GLuint attachId, const GLTexture2DRef& tex2dRef);
            // Attach a 2D texture to the depth attachment point of FBO.
            void attachDepthBuffer(const GLTexture2DRef &tex2d);
            void attachDepthBuffer(const GLRenderbufferObjectRef &buffer);
            //Attach generic texture to FBO color attachment point
            //params: zLayer: only effective for 3D texture to specify texture layer id.
            void attachTextureND(GLuint attachId, const GLTextureAbstract& tex,int zLayer=0, int mipLevel=0 );
        protected:
            //void attachColorBuffer(GLuint attachId, GLTexture2d &tex2d);
            void attachDepthBuffer(const GLTexture2d &tex2d);
            // Attach a Render buffer object to the depth attachment point of FBO.
            void attachDepthBuffer(const GLRenderbufferObject &buffer);
        public:
            //************************************
            // Description: Enable the target of color attachment for rendering.
            // If you need to render to one of color attachment or Multiple Target Rendering,
            // it is the method you need to call right before you call the GL drawing functions.
            // Parameter: GLuint * targetList: The list of target sequence number
            // 0: GL_COLOR_ATTACHMENT0_EXT;
            // 1: GL_COLOR_ATTACHMENT1_EXT;
            // ...so on
            // Parameter: GLuint size: The number of targetList elements.
            //************************************
            void enableMultipleRenderTarget(const GLuint *targetList, GLuint count);
            //************************************
            // Description: Disable render target and restore to GL default rendering
            // target(GL_BACK)
            //************************************
            void disableMultipleRenderTarget();
            // Print Frame buffer Limitations.
            void printFramebufferLimits();
            void printFramebufferInfo();
            //************************************
            // Description: Check if the framebuffer is being set correctly upon
            // the invocation of this method.
            //************************************
            bool checkFramebufferStatus();
            //Get the width and height of the attached texture image.
            //Assume all the attached color texture shared the same dimension.
            //params: color attachment Id. if not given a value, it by default would 
            //iterate through from color attachment 0 until a valid attached texture 
            //is found and return its dimension.
            vec3f getTextureDimension(int attachId=-1) const;
            //GLTexture2DRef getAttachedColorTexture(int attachId);
            GLTextureAbstract* getAttachedTextureND(int attachId);
        private:
            void deleteBuffer();
            GLuint m_fboId;
            GLenum m_target;//GL_FRAMEBUFFER,GL_DRAW_FRAMEBUFFER,GL_READ_FRAMEBUFFER
            bool   m_isBinded;
            std::vector<GLTextureAbstract*> m_attachedTextureND;
    };

    typedef std::shared_ptr<GLFrameBufferObject> GLFrameBufferObjectRef;
}
#endif