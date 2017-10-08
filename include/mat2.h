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

#ifndef MATH_MATRIX2_H
#define MATH_MATRIX2_H

#include "vec2f.h"

namespace davinci{
///////////////////////////////////////////////////////////////////////////
// 2x2 matrix
///////////////////////////////////////////////////////////////////////////

class mat2
{
private:
	float m[4];

public:
	// constructors
	mat2();  // initialize with identity
	mat2(const float src[4]);
	mat2(float xx, float xy, float yx, float yy);
	mat2(const vec2f& r1, const vec2f& r2);
	void    set(const float src[4]);
	void    set(float xx, float xy, float yx, float yy);
	void	set(const vec2f& r1, const vec2f& r2);
	void    setRow(int index, const float row[2]);
	void    setRow(int index, const vec2f& v);
	void    setColumn(int index, const float col[2]);
	void    setColumn(int index, const vec2f& v);

	const float* get() const;
	float   get(int r, int c) const;
	vec2f	get(int r) const;//get rth row vector;
	float   getDeterminant();

	mat2&   identity();
	mat2&   transpose();                         // transpose itself and return reference
	mat2&   inverse();

	// operators
			 operator float *();
	mat2     operator+(const mat2& rhs) const;    // add rhs
	mat2     operator-(const mat2& rhs) const;    // subtract rhs
	mat2&    operator+=(const mat2& rhs);         // add rhs and update this object
	mat2&    operator-=(const mat2& rhs);         // subtract rhs and update this object
	vec2f    operator*(const vec2f& rhs) const;   // multiplication: v' = M * v
	mat2     operator*(const mat2& rhs) const;    // multiplication: M3 = M1 * M2
	mat2&    operator*=(const mat2& rhs);         // multiplication: M1' = M1 * M2
	bool     operator==(const mat2& rhs) const;   // exact compare, no epsilon
	bool     operator!=(const mat2& rhs) const;   // exact compare, no epsilon
	float    operator[](int index) const;         // subscript operator v[0], v[1]
	float&   operator[](int index);               // subscript operator v[0], v[1]

	friend mat2 operator-(const mat2& m);                    // unary operator (-)
	friend mat2 operator*(float scalar, const mat2& m);      // pre-multiplication
	friend vec2f operator*(const vec2f& vec, const mat2& m); // pre-multiplication
	friend std::ostream& operator<<(std::ostream& os, const mat2& m);

};


///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix2
///////////////////////////////////////////////////////////////////////////

inline mat2& mat2::identity()
{
	m[0] = m[3] = 1.0f;
	m[1] = m[2] = 0.0f;
	return *this;
}

inline mat2::mat2()
{
	// initially identity matrix
	identity();
}

inline mat2::mat2(const float src[4])
{
	set(src);
}

inline mat2::mat2(float xx, float xy, float yx, float yy)
{
	set(xx, xy, yx, yy);
}

inline mat2::mat2( const vec2f& r1, const vec2f& r2 )
{
	set(r1,r2);
}

inline void mat2::set(const float src[4])
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
}

inline void mat2::set(float xx, float xy, float yx, float yy)
{
	m[0]= xx;  m[1] = xy;  m[2] = yx;  m[3]= yy;
}

inline void mat2::set(const vec2f& r1, const vec2f& r2)
{
	//rows[0] = r1; rows[1] = r2;
	setRow(0, r1); setRow(1, r2);
}

inline void mat2::setRow(int index, const float row[2])
{
	//m[index*2] = row[0];  m[index*2 + 1] = row[1];
	m[index<<1] = row[0];  m[(index<<1) + 1] = row[1];
	//rows[index][0] = row[0]; rows[index][1] = row[1];
}

inline void mat2::setRow(int index, const vec2f& v)
{
	//m[index*2] = v.x();  m[index*2 + 1] = v.y();
	m[index<<1] = v.x();  m[(index<<1) + 1] = v.y();
	//rows[index] = v;
}

inline void mat2::setColumn(int index, const float col[2])
{
	m[index] = col[0];  m[index + 2] = col[1];
	//rows[0][index] = col[0]; rows[1][index] = col[1];
}

inline void mat2::setColumn(int index, const vec2f& v)
{
	m[index] = v.x();  m[index + 2] = v.y();
	//rows[0][index] = v[0]; rows[1][index] = v[1];
}

inline const float* mat2::get() const
{
	return m;
}

inline float mat2::get(int r, int c) const
{
	return m[(r<<1)+c];//rows[r][c];
}

inline vec2f mat2::get(int r) const
{
	//return rows[r];
	const float *row = &m[r<<1];
	return vec2f(row[0], row[1]);
}

inline mat2::operator float *()
{
	return m;
}

inline mat2 mat2::operator+(const mat2& rhs) const
{
	return mat2(m[0]+rhs[0], m[1]+rhs[1], m[2]+rhs[2], m[3]+rhs[3]);
	//return mat2(rows[0]+rhs.rows[0], rows[1]+rhs.rows[1]);
}

inline mat2 mat2::operator-(const mat2& rhs) const
{
	return mat2(m[0]-rhs[0], m[1]-rhs[1], m[2]-rhs[2], m[3]-rhs[3]);
	//return mat2(rows[0]-rhs.rows[0], rows[1]-rhs.rows[1]);
}

inline mat2& mat2::operator+=(const mat2& rhs)
{
	m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];  m[3] += rhs[3];
	//rows[0]+=rhs.rows[0]; rows[1]+=rhs.rows[1];
	return *this;
}

inline mat2& mat2::operator-=(const mat2& rhs)
{
	m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];  m[3] -= rhs[3];
	return *this;
}

inline vec2f mat2::operator*(const vec2f& rhs) const
{
	return vec2f(m[0]*rhs.x() + m[1]*rhs.y(),  m[2]*rhs.x() + m[3]*rhs.y());
	//return vec2f(rows[0].dot(rhs), rows[1].dot(rhs));
}

inline mat2 mat2::operator*(const mat2& rhs) const
{
	return mat2(m[0]*rhs[0] + m[1]*rhs[2],  m[0]*rhs[1] + m[1]*rhs[3],
				m[2]*rhs[0] + m[3]*rhs[2],  m[2]*rhs[1] + m[3]*rhs[3]);
}

inline mat2& mat2::operator*=(const mat2& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool mat2::operator==(const mat2& rhs) const
{
	return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) && (m[3] == rhs[3]);
}

inline bool mat2::operator!=(const mat2& rhs) const
{
	return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) || (m[3] != rhs[3]);
}

inline float mat2::operator[](int index) const
{
	return m[index];
}

inline float& mat2::operator[](int index)
{
	return m[index];
}

inline mat2 operator-(const mat2& rhs)
{
	return mat2(-rhs[0], -rhs[1], -rhs[2], -rhs[3]);
	//return mat2(-(rhs.rows[0]), -(rhs.rows[1]));
}

inline mat2 operator*(float s, const mat2& rhs)
{
	return mat2(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3]);
	//return mat2(s*rhs.rows[0], s*rhs.rows[1]);
}

inline vec2f operator*(const vec2f& v, const mat2& rhs)
{
	return vec2f(v.x()*rhs[0] + v.y()*rhs[2],  v.x()*rhs[1] + v.y()*rhs[3]);
}

inline std::ostream& operator<<(std::ostream& os, const mat2& m)
{
	os << "(" << m[0] << ",\t" << m[1] << ")\n"
	   << "(" << m[2] << ",\t" << m[3] << ")\n";
	return os;
}
// END OF MATRIX2 INLINE //////////////////////////////////////////////////////

}//end of namespace
// END OF MATRIX4 INLINE //////////////////////////////////////////////////////
#endif
