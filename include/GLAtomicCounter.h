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
//CREATED: 2015-05-23
//UPDATED: 2017-10-15

#ifndef _GLATOMICCOUNTER_H_
#define _GLATOMICCOUNTER_H_
#include <memory>
#include <string>
#include "vec2f.h"
#include "vec3d.h"
#include "mat4.h"
#include "GLBufferObject.h"

namespace davinci
{
	//reference
	//http://www.lighthouse3d.com/tutorials/opengl-atomic-counters/
	//
	class GLAtomicCounter : public GLBufferObject
	{
		public:
			GLAtomicCounter(const std::string& name = "Untitled Atomic Counter");
			//allocate nCounter of atomic counters with initV as initial value.
			void alloc(int nCounter, GLuint initV=0);
			void reset(GLuint initV = 0);
			//read the value of atomic counters which are stored in buffer ptr.
			//count: returns how many counter values are returned. No value will
			//be returned if count=NULL.
			void readCounters(GLuint* ptr, int* count=NULL);
			//map the address of counter storage buffer to host side.
			void* map();
			//unmap the address of counter storage buffer.
			void unmap();
			//return the number of atomic counters.
			int  size() const { return m_count; }
			void printLimitation();
		protected:
			void upload(size_t totalSizeInBytes, const GLvoid* data);
			void upload(size_t offset, size_t totalSizeInBytes, const GLvoid* data);
			void* map(GLenum usage);
		protected:
				std::string m_name;
				int m_count;//# of atomic counters
	};
	typedef std::shared_ptr < GLAtomicCounter >
		GLAtomicCounterRef;
}

#endif

