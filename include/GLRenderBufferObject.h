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

#ifndef _GLRENDERBUFFEROBJECT_H_
#define _GLRENDERBUFFEROBJECT_H_
#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include <memory>

namespace davinci{

    class GLRenderbufferObject
    {
        public:
            GLRenderbufferObject(void);
            GLRenderbufferObject(GLsizei width, GLsizei height);
            ~GLRenderbufferObject(void);
            void    resize(GLsizei width, GLsizei height);
            void    bind();
            void    unbind();
            GLuint  getBufferId() const;
            GLsizei getWidth() const;
            GLsizei getHeight() const;
        private:
            void deleteBuffer();
            GLuint m_rboId;
            GLsizei m_width, m_height;
    };

    #ifdef BOOST_REFERENCE_COUNT
    typedef boost::shared_ptr<GLRenderbufferObject> GLRenderbufferObjectRef;
    #else
    typedef std::shared_ptr<GLRenderbufferObject> GLRenderbufferObjectRef;
    #endif
}

#endif