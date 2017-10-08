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

#include <sstream>
#include <iostream>
#include "GLBufferObject.h"
#include "GLError.h"
#ifdef ENABLE_CUDA_GL_INTEROP
#include <cuda_runtime_api.h>
#include <helper_cuda.h>
#include <cuda_gl_interop.h>
#endif
using namespace std;

namespace davinci{

std::shared_ptr < std::vector<int> >
GLBufferObject::g_availBindingIndex;

GLBufferObject::GLBufferObject(
	GLenum target, GLenum usage,
	const std::string &name /*= "Untitled GLBufferObject"*/)
    :m_target(target),m_usage(usage), m_reservedBytes(0),
	m_firstTime(true), m_name(name), m_bindingIndex(-1)
#ifdef ENABLE_CUDA_GL_INTEROP
    ,m_cudaResource(NULL),m_cudaAccessHint(cudaGraphicsMapFlagsNone)//
    ,m_cudaMappedPtr(NULL)
#endif
{
    glGenBuffers(1, &m_id);
	if (!g_availBindingIndex)
	{
		g_availBindingIndex = std::shared_ptr<std::vector<int> >(new std::vector<int>);
	}

	m_availBindingIndex = g_availBindingIndex;
}

GLBufferObject::~GLBufferObject(void)
{
    unbindBufferObject();
    deleteBuffer();
    GLError::glCheckError(__func__);
}

void GLBufferObject::bindBufferObject()
{
    glBindBuffer(m_target, m_id);
}

void GLBufferObject::unbindBufferObject()
{
    glBindBuffer(m_target, 0);
}

void GLBufferObject::upload(size_t totalSizeInBytes, const GLvoid* data )
{
	bindBufferObject();
    if (m_firstTime)
    {
        m_reservedBytes = totalSizeInBytes;
        glBufferData(m_target, m_reservedBytes, data, m_usage);
        m_firstTime = false;
    }else{
        if (m_reservedBytes < totalSizeInBytes)
        {
            cout << "*************************\n";
            cout << "original GLBufferObject size="<<m_reservedBytes
                 <<"current totalSizeInBytes="<<totalSizeInBytes<<endl;
            cout << "*************************\n";
            m_reservedBytes = totalSizeInBytes;
            //allocate bigger buffer.
            glBufferData(m_target, m_reservedBytes, data, m_usage);
        }else{
            upload(0, totalSizeInBytes, data);
        }
    }
	unbindBufferObject();
	GLError::glCheckError(__func__);
}

void GLBufferObject::upload( size_t offset, size_t totalSizeInBytes,
                             const GLvoid* data )
{
    glBufferSubData(m_target, offset, totalSizeInBytes, data );
}

void GLBufferObject::copy(GLBufferObject &dest, size_t offsetRead, size_t offsetWrite, size_t size)
{
	if (size < 0)
	{
		size = m_reservedBytes;
	}
	if (m_reservedBytes<offsetRead+size)
	{
		GLError::glCheckError("GLBufferObject::copy(): source reserved size < offsetRead+size!\n");
	}
	if (dest.getSizeInBytes()<offsetWrite+size)
	{
		GLError::glCheckError("GLBufferObject::copy(): dest reserved size < offsetWrite+size!\n");
	}
	glBindBuffer(GL_COPY_READ_BUFFER, m_id);
	glBindBuffer(GL_COPY_WRITE_BUFFER, dest.getId());
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offsetRead, offsetWrite, size);
}

void GLBufferObject::deleteBuffer()
{
    if (m_id)
    {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }
    
#ifdef ENABLE_CUDA_GL_INTEROP
    unmapCudaPointer();
    unregisterToCudaResource();
#endif
}

bool GLBufferObject::unmap()
{
	bindBufferObject();
    bool ok = (glUnmapBuffer(m_target)==GL_TRUE);
	unbindBufferObject();
	return ok;
}

void* GLBufferObject::map(GLenum usage)
{
    GLError::purgePreviousGLError();
	bindBufferObject();
    void* buffer = glMapBuffer(m_target,usage);
    GLError::glCheckError(string(__func__)+"()");
	unbindBufferObject();
    return buffer;
}

int GLBufferObject::getNextAvaibleBindingIndex()
{
	static GLint maxBindingIndices = 0;
	if (g_availBindingIndex->empty())
	{//access g_availBindingIndex for the first time. Initialize it.
#if defined(__APPLE__) || defined(MACOSX)
		//sorry, mac user don't have GL_MAX_VERTEX_ATTRIB_BINDINGS macro defined.
		maxBindingIndices = 16;//
#else
		//This will almost certainly be 16.
		PRINT_GL_CAPABILITY(GL_MAX_VERTEX_ATTRIB_BINDINGS, maxBindingIndices);
#endif
		//linear search the next available binding index.
		g_availBindingIndex->resize(maxBindingIndices, 1);
	}
	for (int i = 0; i < g_availBindingIndex->size(); i++)
	{
		if ((*g_availBindingIndex)[i])
		{
			(*g_availBindingIndex)[i] = 0;//It is taken now.
			return i;
		}
	}
	std::stringstream ss;
	ss << __func__ << ": You're running out of vertex buffer object binding index(maximum binding indices=";
	ss << maxBindingIndices;
	std::string msg = ss.str();
	GLError::ErrorMessage(msg);
	return -1;
}

GLint GLBufferObject::getBindingIndex()
{
	if (m_bindingIndex == -1)
	{
		m_bindingIndex = getNextAvaibleBindingIndex();
	}
	//if m_bindingIndex is already allocated, simply return it.
	return m_bindingIndex;
}

void GLBufferObject::releaseBindingIndex()
{
	if (m_bindingIndex == -1) return;//no need to release multiple times.

	if (m_bindingIndex < g_availBindingIndex->size())
	{
		(*g_availBindingIndex)[m_bindingIndex] = 1;//available now.
		m_bindingIndex = -1;

	}
	else if (g_availBindingIndex->size() > 0)
	{
		std::stringstream ss;
		ss << __func__ << ": Released VBO binding index" << m_bindingIndex
			<< "out of bound! Maximum available binding index is "
			<< g_availBindingIndex->size();
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}
	else{
		std::stringstream ss;
		ss << __func__ << ": Release VBO binding index(" << m_bindingIndex
			<< ") failed! g_availBindingIndex is empty(). Please try to"
			<< " try create GLVertexBufferObject as GLVertexBufferObjectRef.\n";
		std::string msg = ss.str();
		GLError::ErrorMessage(msg);
	}
}


#ifdef ENABLE_CUDA_GL_INTEROP
cudaGraphicsResource* GLBufferObject::registerToCudaResource(unsigned int cudaAccessHint/*=cudaGraphicsMapFlagsNone*/)
{
    unregisterToCudaResource();
    m_cudaAccessHint = cudaAccessHint;
    checkCudaErrors(cudaGetDevice(&m_cudaDeviceId));
    checkCudaErrors(cudaGraphicsGLRegisterBuffer(&m_cudaResource, m_id, m_cudaAccessHint));
    return m_cudaResource;
}
void* GLBufferObject::getCudaMappedPointer()
{
    if (!m_cudaResource)
	{
		std::stringstream ss;
        ss<<"******"<<__func__<<"*****\n\
Please register cuda resource before map pointer.\n\
**********************************************************\n";
		std::string msg=ss.str();
        std::cerr<<msg;
        GLError::ErrorMessage(msg);

    }else if (m_cudaMappedPtr)
	{
		std::stringstream ss;
        ss<<"*****"<<__func__<<"******\n\
You must have forget to call unmapCudaPointer() to\n\
release the cuda mapped pointer before requiring a new one!\n\
***********************************************************\n";
		std::string msg=ss.str();
        std::cerr << msg;
        GLError::ErrorMessage(msg);
    }else
    {
        //1. PBO is registered as CUDA resource.
        //2. The resources has never been mapped before.
        //Then mapped the cuda pointer.
        checkCudaErrors(cudaGraphicsMapResources(1, &m_cudaResource, 0));
        checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void**)&m_cudaMappedPtr,
                        &m_reservedBytes, m_cudaResource));
    }
    return m_cudaMappedPtr;
}
void GLBufferObject::unregisterToCudaResource()
{
    if (m_cudaMappedPtr){
        //cuda mapped pointer is not release.
		std::stringstream ss;
        ss << "#####"<<__func__<<"()#####\n\
Call unmapCudaPointer() to release the cuda \n\
mapped pointer before calling unregisterToCudaResource()!\n\
################################################\n";
		std::string msg=ss.str();
        GLError::ErrorMessage(msg);
    }
    if (m_cudaResource){

        int curCudaDeviceId = -1;
        checkCudaErrors(cudaGetDevice(&curCudaDeviceId));
        if (curCudaDeviceId != m_cudaDeviceId)
        {
            std::stringstream ss;
            ss << __func__<<": the buffer resource was registered on device"
                <<m_cudaDeviceId<<" while current active CUDA device is "<<curCudaDeviceId
                <<std::endl;

            cerr << ss.str();
            //switch back to the CUDA device where the buffer object resides.
            checkCudaErrors(cudaSetDevice(m_cudaDeviceId));
            //GLError::ErrorMessage(ss.str());
        }
        //restore back to previous CUDA device.
        checkCudaErrors(cudaSetDevice(curCudaDeviceId));
        checkCudaErrors(cudaGraphicsUnregisterResource(m_cudaResource));
        m_cudaResource = NULL;
    }
}
void GLBufferObject::unmapCudaPointer()
{
    if (m_cudaResource && m_cudaMappedPtr){
        int curCudaDeviceId = -1;
        checkCudaErrors(cudaGetDevice(&curCudaDeviceId));
        if (curCudaDeviceId != m_cudaDeviceId)
        {
            std::stringstream ss;
            ss <<__func__<<": the buffer resource was registered on device"
               <<m_cudaDeviceId<<" while current active CUDA device is "<<curCudaDeviceId
               <<std::endl;
            cerr << ss.str();
            //switch back to the CUDA device where the buffer object resides.
            checkCudaErrors(cudaSetDevice(m_cudaDeviceId));
            //GLError::ErrorMessage(ss.str());
        }
        checkCudaErrors(cudaGraphicsUnmapResources(1, &m_cudaResource,0));
        //restore back to previous CUDA device.
        checkCudaErrors(cudaSetDevice(curCudaDeviceId));
        m_cudaMappedPtr = NULL;
    }
}
#endif

}//end of namespace
