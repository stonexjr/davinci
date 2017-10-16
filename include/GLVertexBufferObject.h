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
//UPDATED: 2017-10-15

#ifndef _GL_ARRAY_BUFFER_OBJECT_H_
#define _GL_ARRAY_BUFFER_OBJECT_H_

#include <memory>
#include "GLBufferObject.h"

namespace davinci{

class GLVertexBufferObject : public GLBufferObject
{
public:
	//GLVertexBufferObject is a linear buffer that stores the contents of vertex attributes.
	//usage:
	// GL_STATIC_DRAW_ARB,GL_STATIC_READ_ARB,GL_STATIC_COPY_ARB
	// GL_DYNAMIC_DRAW_ARB,GL_DYNAMIC_READ_ARB,GL_DYNAMIC_COPY_ARB
	// GL_STREAM_DRAW_ARB,GL_STREAM_READ_ARB,GL_STREAM_COPY_ARB
	GLVertexBufferObject(GLenum usage=GL_STATIC_DRAW);
	~GLVertexBufferObject();
	

	size_t getVertexCount() const { return m_vtxCount; }
	void   setVertexCount(size_t n){m_vtxCount = n;}
	//Upload the contents in its build-in array to GPU.
	void upload(size_t totalSizeInBytes, size_t vtxCount, const GLvoid* data);

private:
	size_t m_vtxCount;
};//end of namespace

typedef std::shared_ptr<GLVertexBufferObject> GLVertexBufferObjectRef;

}
#endif