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

#include "vec2i.h"
#include "vec2f.h"
//using namespace mathtool;
namespace davinci{

vec2i::vec2i( const vec2f &v )
{
	m_x = (int)v.x();
	m_y = (int)v.y();
}

vec2i& davinci::vec2i::operator=( const vec2f &v )
{
	m_x = (int)v.x();
	m_y = (int)v.y();
	return *this;
}

vec2i vec2i::index1D_to_index2D_uint64( size_t index1D,const vec2i &size2d )
{
	vec2i pxy;
	pxy.setY(index1D / size_t(size2d.y()));
	pxy.setX(index1D % size_t(size2d.x()));
	return pxy;
}

vec2i vec2i::index1D_to_index2D( int index1D,const vec2i &size2d )
{
	vec2i pxy;
	pxy.setY(index1D / size2d.y());
	pxy.setX(index1D % size2d.x());
	return pxy;
}

size_t vec2i::index2D_to_index1D(const vec2i &idx2D, const vec2i &size2D)
{
	size_t ret = size_t(idx2D.y()) * size2D.x() + idx2D.x();
	return ret;
}

}
