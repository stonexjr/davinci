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

#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include "GLError.h"
#include "GLAtomicCounter.h"

namespace davinci{

	GLAtomicCounter::GLAtomicCounter(
		const std::string& name/*="Untitled UBBO"*/)
		:GLBufferObject(GL_ATOMIC_COUNTER_BUFFER, GL_DYNAMIC_DRAW, name)
	{	
		
	}

	void GLAtomicCounter::alloc(int nCounter, GLuint initV/*=0*/)
	{
		if (nCounter<=0)
		{
			GLError::ErrorMessage(string("Number of Atomic Counter<=0!"));
		}
		m_count = nCounter;
		std::vector<GLuint> init(nCounter, initV);
		GLBufferObject::upload(sizeof(init[0])*init.size(), init.data());
		glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
		glFinish();
	}

	void GLAtomicCounter::reset(GLuint initV /*= 0*/)
	{
		bindBufferObject();
		std::vector<GLuint> init(m_count, initV);
		glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(init[0]) * init.size(), init.data());
		glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
		unbindBufferObject();
	}

	void GLAtomicCounter::readCounters(GLuint* ptr, int *count/*=NULL*/)
	{
		bindBufferObject();
		glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
		glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * m_count, ptr);
		unbindBufferObject();
		if (count)
		{
			*count = m_count;
		}
	}

	void* GLAtomicCounter::map()
	{
		bindBufferObject();
		glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
		// map the buffer, userCounters will point to the buffers data
		GLuint* userCounters = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0,
															sizeof(GLuint) * m_count,
															GL_MAP_WRITE_BIT | 
															GL_MAP_INVALIDATE_BUFFER_BIT |
															GL_MAP_UNSYNCHRONIZED_BIT);
		GLError::glCheckError("Failed on GLAtomicCounter::map()!\n");
		return userCounters;
	}

	void GLAtomicCounter::printLimitation()
	{
		int val = 0;
		PRINT_GL_CAPABILITY(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_COMBINED_ATOMIC_COUNTERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_VERTEX_ATOMIC_COUNTERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_GEOMETRY_ATOMIC_COUNTERS, val);
		PRINT_GL_CAPABILITY(GL_MAX_FRAGMENT_ATOMIC_COUNTERS, val);
	}

}
