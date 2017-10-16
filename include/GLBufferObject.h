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

#ifndef _GL_BUFFER_OBJECT_H_
#define _GL_BUFFER_OBJECT_H_

#include <stddef.h>
#include <vector>
#include <memory>
#include <string>

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

//#define ENABLE_CUDA_GL_INTEROP

#ifdef ENABLE_CUDA_GL_INTEROP
#include <cuda_gl_interop.h>
#endif

namespace davinci{
#define I_DONT_CARE -1

class GLBufferObject
{
public:
	//Provide a Base class for generating/deleting OpenGL buffer object
	//using glGenBuffers()/glDeleteBuffers()
	//target:
	// GL_ARRAY_BUFFER_ARB:VBO with glEnableClientState(GL_VERTEX_ARRAY);
	// GL_ELEMENT_ARRAY_BUFFER_ARB:VBO+IndexBO with glDrawElements();
	// GL_PIXEL_PACK_BUFFER_ARB,GL_PIXEL_UNPACK_BUFFER_ARB: Pixel Buffer Object
	// GL_UNIFORM_BUFFER
	//usage:
	// GL_STATIC_DRAW_ARB,GL_STATIC_READ_ARB,GL_STATIC_COPY_ARB
	// GL_DYNAMIC_DRAW_ARB,GL_DYNAMIC_READ_ARB,GL_DYNAMIC_COPY_ARB
	// GL_STREAM_DRAW_ARB,GL_STREAM_READ_ARB,GL_STREAM_COPY_ARB
	GLBufferObject(GLenum target, GLenum usage, const std::string &name="Untitled GLBufferObject");
	virtual ~GLBufferObject(void);

	size_t getSizeInBytes() const { return m_reservedBytes;}

	//GLsizei	   getStride() const { return m_stride; }
	GLuint getId()   const { return m_id;}
	std::string getName() const { return m_name; }
	GLenum getTarget() const { return m_target; }

	void   deleteBuffer();
	void   bindBufferObject();
	void   unbindBufferObject();
	
	//************************************
	// Method:    upload
	// Parameter: size_t sizeInBytes: How many bytes of data to upload to GPU.
	// Parameter: const GLvoid * data: The address of the data on CPU memory.
	// Parameter: GLenum usage: GL_STATIC_DRAW_ARB,GL_STATIC_READ_ARB,GL_STATIC_COPY_ARB
	// GL_DYNAMIC_DRAW_ARB,GL_DYNAMIC_READ_ARB,GL_DYNAMIC_COPY_ARB
	// GL_STREAM_DRAW_ARB,GL_STREAM_READ_ARB,GL_STREAM_COPY_ARB
	//************************************
	void upload(size_t totalSizeInBytes,const GLvoid* data);
	//copy data from current buffer object to 'dest' buffer object directly on GPU.
	void copy(GLBufferObject &dest, size_t offsetRead, size_t offsetWrite, size_t size);
	//GLuint getBindingIndex(){return m_bindingIndex;}
	GLint  getBindingIndex();
	//Once finished with current draw shader call,
	//VBO should release the allocated binding index so that
	//it can be reused by other shader draw calls.
	GLvoid releaseBindingIndex();

protected:
	void upload(size_t offset, size_t totalSizeInBytes,const GLvoid* data);

	static int getNextAvaibleBindingIndex();
public:
	//void upload(GLsizeiptr count, GLsizei stride, const GLvoid* data);
	//************************************
	// Returns:   void*: returned pointer to the buffer object's GPU memroy that can be manipulated by CPU client.
	// Parameter: GLenum usage: hint for usage:GL_READ_WRITE(default), GL_READ_ONLY_ARB
	//************************************
	void*  map(GLenum usage=GL_READ_WRITE);
	bool   unmap();
#ifdef ENABLE_CUDA_GL_INTEROP

	int getCudaDeviceId() const { return m_cudaDeviceId;}
	//************************************
	// Description:
	// Register buffer data into CUDA address space, so that CUDA kernel
	// can access the buffer data.
	// Parameter: 
	// unsigned int cudaAccessHin    // t: Hint for how you gonna access the
	// mapped buffer. Available options are:
	// cudaGraphicsMapFlagsNone,
	// cudaGraphicsMapFlagsReadOnly,
	// cudaGraphicsMapFlagsWriteDiscard
	// For their specific meaning, please refer to CUDA reference manual.
	//************************************
	cudaGraphicsResource* registerToCudaResource(unsigned int cudaAccessHint=cudaGraphicsMapFlagsNone);
	void  unregisterToCudaResource();
	bool isRegisteredToCudaResource() const { return m_cudaResource != NULL;}
	//************************************
	// Returns: void*: The device pointer that points to the mapped PBO
	// buffer in CUDA address space.
	// Description: Get the address of mapped PBO buffer in CUDA address space.
	// Parameter: size_t * numBytes: The size in bytes of the mapped buffer object.
	//************************************
	void* getCudaMappedPointer();
	//************************************
	// Description: User must call this function to release cuda resource,
	// once they have finish using mapped cuda pointer.
	//************************************
	void  unmapCudaPointer();
	bool isCudaPointerMapped() const { return m_cudaMappedPtr != NULL;}

#endif

protected:
	size_t m_reservedBytes;//size of the buffer in bytes.
	bool   m_firstTime;
	//GLsizei m_stride;
	GLenum m_target;
	GLenum m_usage;//GL_STATIC_DRAW_ARB,GL_STATIC_READ_ARB,GL_STATIC_COPY_ARB
				   //GL_DYNAMIC_DRAW_ARB,GL_DYNAMIC_READ_ARB,GL_DYNAMIC_COPY_ARB
				   //GL_STREAM_DRAW_ARB,GL_STREAM_READ_ARB,GL_STREAM_COPY_ARB
	GLuint m_id;
	GLint  m_bindingIndex;
	std::string m_name;
	//Each instance of GLTextureAbstract(or its subclass) has 
	//reference to the global static variable g_availIdsRef such
	//that g_availBindingIndex is guaranteed the be the last one to be
	//released.

	std::shared_ptr < std::vector<int> >
		m_availBindingIndex;
	//Entry i indicate the availability of the binding index. 
	//1: available, 0: taken.
	static std::shared_ptr < std::vector<int> >
		g_availBindingIndex;

#ifdef ENABLE_CUDA_GL_INTEROP
	int    m_cudaDeviceId;//device id where the buffer object is shared.
	cudaGraphicsResource *m_cudaResource;
	GLuint m_cudaAccessHint;//cudaGraphicsMapFlagsNone, cudaGraphicsMapFlagsReadOnly,cudaGraphicsMapFlagsWriteDiscard
	void*  m_cudaMappedPtr;
#endif

};
}//end of namespace
#endif