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
//UPDATED: 2014-04-17
#ifndef _GL_INDEXBUFFEROBJECT_H_
#define _GL_INDEXBUFFEROBJECT_H_
#pragma once
#include <vector>
#include "GLBufferObject.h"
#include "vec3i.h"
#include "vec4i.h"
#include "GLVertexArray.h"

namespace davinci{

class GLIndexBufferObject :
                public GLBufferObject
{
public:
    GLIndexBufferObject(GLenum geotype, std::shared_ptr<GLenum> arrayid = std::shared_ptr<GLenum>((GLenum*)NULL),
                        GLenum usage=GL_STATIC_DRAW);

    ~GLIndexBufferObject(void);

    void upload();
    void enable();
    void disable();
    void attachVAO(const GLVertexArrayObjectRef& vbo);
    void attachVAO(std::shared_ptr<GLenum> vaoid);
    void draw(GLsizeiptr count=0);
    void drawInstanced(GLsizei primcount, GLsizeiptr count=0);
    void enableRestart(bool f){ m_bUseRestart = f; }
    bool isEnableRestart() { return m_bUseRestart;}
    void setRestartIndex(GLuint idx) { m_restartIndex = idx;}
    std::vector<GLuint> getIndexData() const { return m_indexData; }
    void setIndexData(const std::vector<GLuint>& val) { m_indexData = val; }
    void clear();

    GLIndexBufferObject& operator << (const GLuint i);
    GLIndexBufferObject& operator << (const vec3i& i);
    GLIndexBufferObject& operator << (const vec4i& i);

    bool m_enabled;
    GLVertexArrayObjectRef m_attachedVAO;
    std::shared_ptr<GLenum> m_arrayId;
    GLenum m_type;//type of geometry the object represent. GL_TRIANGLES, GL_QUADS...
    bool m_bUseRestart;
    GLuint m_restartIndex;
    std::vector<GLuint> m_indexData;
    size_t m_indexDataSize;
};

typedef std::shared_ptr<GLIndexBufferObject> GLIndexBufferObjectRef;

}
#endif
