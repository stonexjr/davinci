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
#include <sstream>
#include "GLError.h"
#include "GLVertexBufferObject.h"

namespace davinci{


GLVertexBufferObject::GLVertexBufferObject(GLenum usage/*=GL_STATIC_DRAW*/)
	:GLBufferObject(GL_ARRAY_BUFFER,usage),
	 m_vtxCount(0)
{
}

void GLVertexBufferObject::upload(size_t totalSizeInBytes, 
						size_t vtxCount, const GLvoid* data )
{
	m_vtxCount = vtxCount;
	GLBufferObject::upload(totalSizeInBytes,data);
}

GLVertexBufferObject::~GLVertexBufferObject()
{
	releaseBindingIndex();
	/*
    if(m_bindingIndex >= g_availBindingIndex.size())
        return;

    if(!g_availBindingIndex[m_bindingIndex])
        g_availBindingIndex[m_bindingIndex]=1;
    else{
        std::stringstream ss;
        ss << __func__
            <<": There is inconsistency at binding index. \nMaybe some of your GLVertexBufferObject creation does not use GLBufferObject::getNexAvaibleBindingIndex() to get available binding index."
            <<m_bindingIndex<<std::endl;
        std::string msg=ss.str();
        std::cerr << msg;
        GLError::ErrorMessage(msg);
    }
	*/
}

}