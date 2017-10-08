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

///////////////////////////////////////////////////////////////////////////////
//AUTHOR: Jinrong Xie (stonexjr at gmail.com)
//CREATED: 2013-10-23
//UPDATED: 2014-02-07
// Matrice.h
// =========
// NxN Matrix Math classes(N=2,3,4)
//
// All matrices are row major. (OpenGL uses column-major matrix)
// | 0 1 |    | 0 1 2 |    |  0  1  2  3 |
// | 2 3 |    | 3 4 5 |    |  4  5  6  7 |
//            | 6 7 8 |    |  8  9 10 11 |
//                         | 12 13 14 15 |
///////////////////////////////////////////////////////////////////////////////

#ifndef MATH_MATRIX3_H
#define MATH_MATRIX3_H

#include "vec3f.h"

namespace davinci{
///////////////////////////////////////////////////////////////////////////
// 3x3 matrix
///////////////////////////////////////////////////////////////////////////
class mat4;

class mat3
{
	friend class mat4;
private:
	float m[9];
	float tm[9]; //transposed matrix.
public:
	// constructors
	mat3();  // init with identity
	mat3(const float src[9]);
	mat3(float xx, float xy, float xz,
		 float yx, float yy, float yz,
		 float zx, float zy, float zz);
	mat3(const vec3f& v1, const vec3f& v2, const vec3f& v3);

	void    set(const float src[9]);
	void    set(float xx, float xy, float xz,
				float yx, float yy, float yz,
				float zx, float zy, float zz);
	void    set(const vec3f& v1, const vec3f& v2, const vec3f& v3);

	void    setRow(int index, const float row[3]);
	void    setRow(int index, const vec3f& v);
	void    setColumn(int index, const float col[3]);
	void    setColumn(int index, const vec3f& v);

	const float* get() const;
	//get matrix[r][c] rth row and cth column
	float   get(int r, int c) const;
	vec3f	getRow(int r) const;//get rth row vector;

	const float* getTranspose();// return transposed matrix, the matrix itself will not change.
	float   getDeterminant();

	mat3&   identity();
	mat3&   transpose();// transpose itself and return reference
	mat3&   inverse();

	// operators
	mat3     operator+(const mat3& rhs) const;    // add rhs
	mat3     operator-(const mat3& rhs) const;    // subtract rhs
	mat3&    operator+=(const mat3& rhs);         // add rhs and update this object
	mat3&    operator-=(const mat3& rhs);         // subtract rhs and update this object
	vec3f    operator*(const vec3f& rhs) const;    // multiplication: v' = M * v
	mat3     operator*(const mat3& rhs) const;    // multiplication: M3 = M1 * M2
	mat3&    operator*=(const mat3& rhs);         // multiplication: M1' = M1 * M2
	bool     operator==(const mat3& rhs) const;   // exact compare, no epsilon
	bool     operator!=(const mat3& rhs) const;   // exact compare, no epsilon
	float    operator[](int index) const;            // subscript operator v[0], v[1]
	float&   operator[](int index);                  // subscript operator v[0], v[1]

	friend mat3 operator-(const mat3& m);                   // unary operator (-)
	friend mat3 operator*(float scalar, const mat3& m);     // pre-multiplication
	friend vec3f operator*(const vec3f& vec, const mat3& m);// pre-multiplication
	friend std::ostream& operator<<(std::ostream& os, const mat3& m);

protected:


};
///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix3
///////////////////////////////////////////////////////////////////////////
inline mat3::mat3()
{
	// initially identity matrix
	identity();
}

inline mat3::mat3(const float src[9])
{
	set(src);
}

inline mat3::mat3(float xx, float xy, float xz,
						float yx, float yy, float yz,
						float zx, float zy, float zz)
{
	set(xx, xy, xz,  yx, yy, yz,  zx, zy, zz);
}
inline mat3::mat3( const vec3f& v1, const vec3f& v2, const vec3f& v3 )
{
	set(v1,v2,v3);
}
inline void mat3::set(const float src[9])
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];
	m[3] = src[3];  m[4] = src[4];  m[5] = src[5];
	m[6] = src[6];  m[7] = src[7];  m[8] = src[8];
}

inline void mat3::set(float xx, float xy, float xz,
						 float yx, float yy, float yz,
						 float zx, float zy, float zz)
{
	m[0] = xx;  m[1] = xy;  m[2] = xz;
	m[3] = yx;  m[4] = yy;  m[5] = yz;
	m[6] = zx;  m[7] = zy;  m[8] = zz;
}

inline void mat3::set(const vec3f& v1,const vec3f& v2,const vec3f& v3)
{
	//rows[0]=v1; rows[1]=v2; rows[2]=v3;
	m[0] = v1[0]; m[1] = v1[1]; m[2] = v1[2];
	m[3] = v2[0]; m[4] = v2[1]; m[5] = v2[2];
	m[6] = v3[0]; m[7] = v3[1]; m[8] = v3[2];
}

inline void mat3::setRow(int index, const float row[3])
{
	//m[index*3] = row[0];  m[index*3 + 1] = row[1];  m[index*3 + 2] = row[2];
	//rows[index][0] = row[0]; rows[index][1] = row[1]; rows[index][2] = row[2];
	memcpy(&m[index*3], row, sizeof(float)*3);
}

inline void mat3::setRow(int index, const vec3f& v)
{
	m[index*3] = v.x();  m[index*3 + 1] = v.y();  m[index*3 + 2] = v.z();
	//rows[index] = v;
}

inline void mat3::setColumn(int index, const float col[3])
{
	m[index] = col[0];  m[index + 3] = col[1];  m[index + 6] = col[2];
	//rows[0][index] = col[0]; rows[1][index] = col[1]; rows[2][index] = col[2];
}

inline void mat3::setColumn(int index, const vec3f& v)
{
	m[index] = v.x();  m[index + 3] = v.y();  m[index + 6] = v.z();
	//rows[0][index] = v.x(); rows[1][index] = v.y(); rows[2][index] = v.z();
}

inline const float* mat3::get() const
{
	return m;
}

inline float mat3::get(int r, int c) const
{
	//return rows[r][c];
	return m[r*3+c];
}

inline vec3f mat3::getRow(int r) const
{
	//return rows[r];
	const float *row = &m[r*3];
	return vec3f(row[0], row[1], row[2]);
}

inline mat3& mat3::identity()
{
	m[0] = m[4] = m[8] = 1.0f;
	m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0.0f;
	return *this;
}
inline const float* mat3::getTranspose()
{
	tm[0] = m[0];   tm[1] = m[3];   tm[2] = m[6];
	tm[3] = m[1];   tm[4] = m[4];   tm[5] = m[7];
	tm[6] = m[2];   tm[7] = m[5];   tm[8] = m[8];
	return tm;
}
inline mat3 mat3::operator+(const mat3& rhs) const
{
	return mat3(m[0]+rhs[0], m[1]+rhs[1], m[2]+rhs[2],
				m[3]+rhs[3], m[4]+rhs[4], m[5]+rhs[5],
				m[6]+rhs[6], m[7]+rhs[7], m[8]+rhs[8]);
	/*
	return mat3(rows[0]+rhs.rows[0],
				rows[1]+rhs.rows[1],
				rows[2]+rhs.rows[2]);
	*/
}

inline mat3 mat3::operator-(const mat3& rhs) const
{
	return mat3(m[0]-rhs[0], m[1]-rhs[1], m[2]-rhs[2],
				m[3]-rhs[3], m[4]-rhs[4], m[5]-rhs[5],
				m[6]-rhs[6], m[7]-rhs[7], m[8]-rhs[8]);
	/*
	return mat3(rows[0]-rhs.rows[0],
				rows[1]-rhs.rows[1],
				rows[2]-rhs.rows[2]);
	*/
}

inline mat3& mat3::operator+=(const mat3& rhs)
{
	m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];
	m[3] += rhs[3];  m[4] += rhs[4];  m[5] += rhs[5];
	m[6] += rhs[6];  m[7] += rhs[7];  m[8] += rhs[8];
	/*
	rows[0]+=rhs.rows[0];
	rows[1]+=rhs.rows[1];
	rows[2]+=rhs.rows[2];
	*/
	return *this;
}

inline mat3& mat3::operator-=(const mat3& rhs)
{
	m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];
	m[3] -= rhs[3];  m[4] -= rhs[4];  m[5] -= rhs[5];
	m[6] -= rhs[6];  m[7] -= rhs[7];  m[8] -= rhs[8];
	/*
	rows[0]-=rhs.rows[0];
	rows[1]-=rhs.rows[1];
	rows[2]-=rhs.rows[2];
	*/
	return *this;
}



inline vec3f mat3::operator*(const vec3f& rhs) const
{
	return vec3f(m[0]*rhs.x() + m[1]*rhs.y() + m[2]*rhs.z(),
				 m[3]*rhs.x() + m[4]*rhs.y() + m[5]*rhs.z(),
				 m[6]*rhs.x() + m[7]*rhs.y() + m[8]*rhs.z());
	/*
	return vec3f(rows[0].dot(rhs), rows[1].dot(rhs), rows[2].dot(rhs));
	*/
}



inline mat3 mat3::operator*(const mat3& rhs) const
{
	return mat3(m[0]*rhs[0] + m[1]*rhs[3] + m[2]*rhs[6],  m[0]*rhs[1] + m[1]*rhs[4] + m[2]*rhs[7],  m[0]*rhs[2] + m[1]*rhs[5] + m[2]*rhs[8],
				m[3]*rhs[0] + m[4]*rhs[3] + m[5]*rhs[6],  m[3]*rhs[1] + m[4]*rhs[4] + m[5]*rhs[7],  m[3]*rhs[2] + m[4]*rhs[5] + m[5]*rhs[8],
				m[6]*rhs[0] + m[7]*rhs[3] + m[8]*rhs[6],  m[6]*rhs[1] + m[7]*rhs[4] + m[8]*rhs[7],  m[6]*rhs[2] + m[7]*rhs[5] + m[8]*rhs[8]);
}

inline mat3& mat3::operator*=(const mat3& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool mat3::operator==(const mat3& rhs) const
{
	return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) &&
		   (m[3] == rhs[3]) && (m[4] == rhs[4]) && (m[5] == rhs[5]) &&
		   (m[6] == rhs[6]) && (m[7] == rhs[7]) && (m[8] == rhs[8]);
	/*
	return (rows[0] == rhs.rows[0])&&
		   (rows[1] == rhs.rows[1])&&
		   (rows[2] == rhs.rows[2]);
	*/
}

inline bool mat3::operator!=(const mat3& rhs) const
{
	return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) ||
		   (m[3] != rhs[3]) || (m[4] != rhs[4]) || (m[5] != rhs[5]) ||
		   (m[6] != rhs[6]) || (m[7] != rhs[7]) || (m[8] != rhs[8]);
	/*
	return (rows[0] != rhs.rows[0])||
		   (rows[1] != rhs.rows[1])||
		   (rows[2] != rhs.rows[2]);
	*/
}

inline float mat3::operator[](int index) const
{
	return m[index];
}

inline float& mat3::operator[](int index)
{
	return m[index];
}

inline mat3 operator-(const mat3& rhs)
{
	return mat3(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8]);
	//return mat3(-(rhs.rows[0]), -(rhs.rows[1]), -(rhs.rows[2]));
}



inline mat3 operator*(float s, const mat3& rhs)
{
	return mat3(s*rhs[0], s*rhs[1], s*rhs[2],
				s*rhs[3], s*rhs[4], s*rhs[5],
				s*rhs[6], s*rhs[7], s*rhs[8]);
	//return mat3(s*rhs.rows[0], s*rhs.rows[1], s*rhs.rows[2]);
}

inline vec3f operator*(const vec3f& v, const mat3& m)
{
	return vec3f(v.x()*m[0] + v.y()*m[3] + v.z()*m[6],
				 v.x()*m[1] + v.y()*m[4] + v.z()*m[7],
				 v.x()*m[2] + v.y()*m[5] + v.z()*m[8]);
}



inline std::ostream& operator<<(std::ostream& os, const mat3& m)
{
	os << "(" << m[0] << ",\t" << m[1] << ",\t" << m[2] << ")\n"
	   << "(" << m[3] << ",\t" << m[4] << ",\t" << m[5] << ")\n"
	   << "(" << m[6] << ",\t" << m[7] << ",\t" << m[8] << ")\n";
	return os;
}
// END OF MATRIX3 INLINE //////////////////////////////////////////////////////

}//end of namespace
#endif
