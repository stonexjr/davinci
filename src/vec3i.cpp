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

#include "vec3i.h"
#include "vec3f.h"
//using namespace mathtool;
namespace davinci{
vec3i::vec3i( const vec3f &v )
{
	m_x = (int)v.x();
	m_y = (int)v.y();
	m_z = (int)v.z();
}

vec3i& davinci::vec3i::operator=( const vec3f &v )
{
	m_x = (int)v.x();
	m_y = (int)v.y();
	m_z = (int)v.z();
	return *this;
}

vec3i vec3i::index1D_to_index3D_uint64( size_t index1D,const vec3i &size3d )
{
	vec3i pxyz;
	pxyz.setZ(index1D / (size3d.x() * size3d.y()));

	size_t tmp = pxyz.z(); 
	tmp *= size3d.x(); tmp *= size3d.y();

	pxyz.setY((index1D - tmp)/(size_t(size3d.x())) );
	pxyz.setX(index1D % size3d.x());
	return pxyz;
}

vec3i vec3i::index1D_to_index3D( int index1D,const vec3i &size3d )
{
	vec3i pxyz;
	pxyz.setZ(index1D / (size3d.x() * size3d.y()));
	pxyz.setY((index1D - pxyz.z()*size3d.x()*size3d.y())/size3d.x());
	pxyz.setX(index1D % size3d.x());
	return pxyz;
}

size_t vec3i::index3D_to_index1D(const vec3i &idx3D, const vec3i &size3D)
{
	size_t ret =  size_t(idx3D.z()) * size_t(size3D.y()) * size3D.x() 
				+ size_t(idx3D.y()) * size3D.x() 
				+ idx3D.x();
	return ret;
}

}
