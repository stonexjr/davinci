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

#ifndef _GL_TRIANGLE_T_H_
#define _GL_TRIANGLE_T_H_
#include "vec3f.h"

namespace davinci{

	class GLTriangle{
	public:
		davinci::vec3f v[3];
		davinci::vec3f vn[3];//normal vector directly imported from model file.
		davinci::vec3f normal;//normal vector of this triangle computed using v1v2 cross v1v3
	public:
		GLTriangle(void){};
		~GLTriangle(){};
		bool triangle_overlap_sphere(davinci::vec3f &pt_center, float radius) const;
		
		float vertex_triangle_distance(const davinci::vec3f &v0) const;
		bool  point_in_triangle_barycentric(const davinci::vec3f &P ) const;
		bool  point_in_triangle_barycentric_double(const davinci::vec3f &Pf) const;

	};
}
#endif

