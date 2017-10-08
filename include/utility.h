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

#ifndef _UTILITIES_H
#define _UTILITIES_H
#include <stdint.h>
#include "vec3f.h"
#include "vec3i.h"
//Get the underlying data pointer of vector
//Is this
/*
template <class T>
T* data_ptr(svector<T>& vec)
{
    return vec.empty() ? 0 : &vec[0];
}

template <class T>
const T* data_ptr(const svector<T>& vec)
{
    return vec.empty() ? 0 : &vec[0];
}
*/

namespace davinci{

	inline bool POINT_INSIDE_SPHERE(const davinci::vec3f &pt_center, float radius, const davinci::vec3f& pt)
	{
		return (pt - pt_center).lengthSquared() <= radius*radius;
	}
	template <class T>
	inline void swapxor(const T &left, const T &right)
	{
		(int&)left ^= (int&)right;
		(int&)right ^= (int&)left;
		(int&)left ^= (int&)right;
	}

	inline int is_big_endian(void)
	{
		union {
			uint32_t i;
			char c[4];
		} bint = { 0x01020304 };

		return bint.c[0] == 1;
	}

	//! Byte swap unsigned short
	inline uint16_t swap_uint16(uint16_t val)
	{
		return (val << 8) | (val >> 8);
	}

	//! Byte swap short
	inline int16_t swap_int16(int16_t val)
	{
		return (val << 8) | ((val >> 8) & 0xFF);
	}

	//! Byte swap unsigned int
	inline uint32_t swap_uint32(uint32_t val)
	{
		val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
		return (val << 16) | (val >> 16);
	}

	//! Byte swap int
	inline int32_t swap_int32(int32_t val)
	{
		val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
		return (val << 16) | ((val >> 16) & 0xFFFF);
	}
	inline uint64_t swap_int64(int64_t val)
	{
		val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
		val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
		return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
	}

	inline uint64_t swap_uint64(uint64_t val)
	{
		val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
		val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
		return (val << 32) | (val >> 32);
	}
	template <typename T>
	T swap_endian(T u)
	{
		union
		{
			T u;
			unsigned char u8[sizeof(T)];
		} source, dest;

		source.u = u;

		for (size_t k = 0; k < sizeof(T); k++)
			dest.u8[k] = source.u8[sizeof(T) - k - 1];

		return dest.u;
	};
	//Convert color component format from BGRA (A is at low address and 
	// B is at high address) to ABGR ( A is at high address and R is at low address).
	inline uint32_t BGRA_to_ABGR(uint32_t bgra){
		// A << 24     |   (BGR)>>8
		uint32_t ret = (0x0FF & bgra) << 24 | (0xFFFFFF00 & bgra) >> 8;
		return ret;
	}

	//Load a 3d chunk of data in a volume from a file
	//start: start offset in 3d
	//size: size of the chunk in 3d
	//domain_size: size of the volume in 3d
	//ifs: input file stream where the volume data is stored.
	//chunk_ptr: preallocated buffer to store the chunk data.
	template<typename T>
	void readChunk(vec3i start, vec3i size, vec3i domain_size,
		ifstream &ifs, T *chunk_ptr)
	{
		size_t offset = vec3i::index3D_to_index1D(start, domain_size);
		size_t count = 0;
		size_t domain_size_xy = domain_size.x() * domain_size.y();
		size_t zLayerOffset, yRowOffset;
		for (int iz = 0; iz < size.z(); iz++)
		{
			size_t zLayerOffset = offset + iz * domain_size_xy;//z:layer
			for (int iy = 0; iy < size.y(); iy++)
			{
				yRowOffset = zLayerOffset + iy * domain_size.x();//y:row
				ifs.seekg(yRowOffset*sizeof(T));
				ifs.read(reinterpret_cast<char*>(&(chunk_ptr[count])), sizeof(T) * size.x());//a
				count += size.x();
				//pos += domain_size.x();
			}
			//pos += domain_size_xy;
		}
	}

}
#endif