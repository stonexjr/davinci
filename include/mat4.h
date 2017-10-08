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
//
// Matrice.h
// =========
// NxN Matrix Math classes(N=2,3,4)
//
// All matrices are row major. (OpenGL uses column-major matrix)
// | 0 1 |    | 0 1 2 |    |  0  1  2  3 |
// | 2 3 |    | 3 4 5 |    |  4  5  6  7 |
//            | 6 7 8 |    |  8  9 10 11 |
//                         | 12 13 14 15 |
//
// One thing to note is the matrix model discrepancy between OpenGL build-in matrix and mat4.
// OpenGL build-in matrix is column major, and use matrix post-multiplication;
// That means any subsequent transformations matrices will be multiplied to the
// right of current matrix.
// while mat4 preserves row major order, which is commonly used, and uses matrix
// pre-multiplication. That means any subsequent transformations matrices will be multiplied to the
// left of current matrix.
// Example: Suppose we first rotate a object and then push the object away from camera so that we
// can see it from a proper distance with a proper orientation. The respective transformation matrices are R and T.
// And suppose v is a column vector that denotes the coordinates of one of the vertices on the object.
// In OpenGL matrix model the transformed coordinates of the vertex is v'R*T (where v' is the transpose of v)
// which is a row vector.
// However, in mat4 mode, the transformed coordinates of the vertex is T*R*v. It is a column vector.
// Therefore, the order of applying translate/rotateXYZ in mat4 is in the reverse order of the one in
// OpenGL mode and transpose your final mat4 before assigning it to vertex shader.
// Because  mat4(T*R)' = OpenGL(R'*T').
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef MATH_MATRIX4_H
#define MATH_MATRIX4_H

#include "vec3f.h"
#include "vec4f.h"
#include "mat3.h"

namespace davinci{
///////////////////////////////////////////////////////////////////////////
// 4x4 matrix
///////////////////////////////////////////////////////////////////////////
class mat4
{
public:
	float m[16];
	float tm[16]; // transpose matrix
public:
	// constructors
	mat4();  // initialize with identity
	mat4(const float src[16]);
	mat4(const float src[4][4]);
	mat4(float xx, float xy, float xz, float xw,
		 float yx, float yy, float yz, float yw,
		 float zx, float zy, float zz, float zw,
		 float wx, float wy, float wz, float ww);

	mat4(const vec4f& v0, const vec4f& v1,const vec4f& v2,const vec4f& v3);

	void   set(const float src[16]);
	void   set(const float src[4][4]);
	void   set(float xx, float xy, float xz, float xw,
			   float yx, float yy, float yz, float yw,
			   float zx, float zy, float zz, float zw,
			   float wx, float wy, float wz, float ww);

	void   set(const vec4f& v0, const vec4f& v1,const vec4f& v2,const vec4f& v3);

	void   setRow(int index, const float row[4]);
	void   setRow(int index, const vec4f& v);
	void   setRow(int index, const vec3f& v);
	void   setColumn(int index, const float col[4]);
	void   setColumn(int index, const vec4f& v);
	void   setColumn(int index, const vec3f& v);

	const float* get() const;
	//get matrix[r][c] rth row and cth column
	float   get(int r, int c) const;
	float&  get(int r, int c);
	vec4f	getRow(int r) const;//get rth row vector;

	//const float* getTranspose();  // return transposed matrix, the matrix itself will change.
	float        getDeterminant();
	static mat4 createPerpProjMatrix(float l, float r, float b, float t, float n, float f);
	//fovY: in degree unit
	static mat4 createPerpProjMatrix( float fovY, float aspect, float front, float back ); 
	static mat4 createOrthoProjMatrix(float l, float r, float b, float t, float n, float f);

	mat4&    identity();
	mat4&    transpose();         // transpose itself and return reference
	mat4&    inverse();           // check best inverse method before inverse
	mat4&    invertEuclidean();   // inverse of Euclidean transform matrix
	mat4&    invertAffine();      // inverse of affine transform matrix
	mat4&    invertProjective();  // inverse of projective matrix using partitioning
	mat4&    invertGeneral();     // inverse of generic matrix

	// transform matrix
	mat4&    translate(float x, float y, float z);   // translation by (x,y,z)
	mat4&    translate(const vec3f& v);            //
	//Rotation angle in radian 
	mat4&    rotate(float angle, const vec3f& axis); // rotate angle(radian) along the given axis
	// rotate angle(radian) along the given vector(x,y,z)
	mat4&    rotate(float angle, float x, float y, float z);
	mat4&    rotateX(float angle);                   // rotate on X-axis with degree
	mat4&    rotateY(float angle);                   // rotate on Y-axis with degree
	mat4&    rotateZ(float angle);                   // rotate on Z-axis with degree
	//Rotation angle in degree
	mat4&    rotated(float angle, const vec3f& axis); // rotate angle(degree) along the given axis
	mat4&    rotated(float angle, float x, float y, float z);
	mat4&    rotateXd(float angle);                   // rotate on X-axis with degree
	mat4&    rotateYd(float angle);                   // rotate on Y-axis with degree
	mat4&    rotateZd(float angle);                   // rotate on Z-axis with degree

	mat4&    scale(float scale);                     // uniform scale
	mat4&    scale(float sx, float sy, float sz);    // scale by (sx, sy, sz) on each axis

	mat3     getNormalMatrix();//return matrix used for transforming normal vector.
	// operators
	mat4     operator+(const mat4& rhs) const;    // add rhs
	mat4     operator-(const mat4& rhs) const;    // subtract rhs
	mat4&    operator+=(const mat4& rhs);         // add rhs and update this object
	mat4&    operator-=(const mat4& rhs);         // subtract rhs and update this object
	vec4f	 operator*(const vec4f& rhs) const;    // multiplication: v' = M * v
	vec3f	 operator*(const vec3f& rhs) const;    // multiplication: v' = M * v
	mat4     operator*(const mat4& rhs) const;    // multiplication: M3 = M1 * M2
	mat4&    operator*=(const mat4& rhs);         // multiplication: M1' = M1 * M2
	bool     operator==(const mat4& rhs) const;   // exact compare, no epsilon
	bool     operator!=(const mat4& rhs) const;   // exact compare, no epsilon
	//float    operator[](int index) const;         // subscript operator v[0], v[1]
	//float&   operator[](int index);               // subscript operator v[0], v[1]
	vec4f    operator[](int index) const;
	float*   operator[](int index);
	//vec4f&   operator[](int index);

	friend mat4 operator-(const mat4& m);                    // unary operator (-)
	friend mat4 operator*(float scalar, const mat4& m);      // pre-multiplication
	friend vec3f operator*(const vec3f& vec, const mat4& m); // pre-multiplication
	friend vec4f operator*(const vec4f& vec, const mat4& m); // pre-multiplication
	friend std::ostream& operator<<(std::ostream& os, const mat4& m);

protected:

private:
	float       getCofactor(float m0, float m1, float m2,
							float m3, float m4, float m5,
							float m6, float m7, float m8);
};
///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix4
///////////////////////////////////////////////////////////////////////////
inline mat4::mat4()
{
	// initially identity matrix
	identity();
}

inline mat4::mat4(const float src[16])
{
	set(src);
}

inline mat4::mat4(const float src[4][4])
{
	set(src);
}

inline mat4::mat4(float xx, float xy, float xz, float xw,
				  float yx, float yy, float yz, float yw,
				  float zx, float zy, float zz, float zw,
				  float wx, float wy, float wz, float ww)
{
	set(xx, xy, xz, xw,  yx, yy, yz, yw,  zx, zy, zz, zw,  wx, wy, wz, ww);
}

inline mat4::mat4(const vec4f& v0, const vec4f& v1,const vec4f& v2,const vec4f& v3)
{
	set(v0,v1,v2,v3);
}

inline void mat4::set(const float src[4][4])
{
	m[0] = src[0][0];  m[1] = src[0][1];  m[2] = src[0][2];  m[3] = src[0][3];
	m[4] = src[1][0];  m[5] = src[1][1];  m[6] = src[1][2];  m[7] = src[1][3];
	m[8] = src[2][0];  m[9] = src[2][1];  m[10]= src[2][2];  m[11]= src[2][3];
	m[12]= src[3][0];  m[13]= src[3][1];  m[14]= src[3][2];  m[15]= src[3][3];
}

inline void mat4::set(const float src[16])
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
	m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
	m[8] = src[8];  m[9] = src[9];  m[10]= src[10]; m[11]= src[11];
	m[12]= src[12]; m[13]= src[13]; m[14]= src[14]; m[15]= src[15];
}

inline void mat4::set(float xx, float xy, float xz, float xw,
					  float yx, float yy, float yz, float yw,
					  float zx, float zy, float zz, float zw,
					  float wx, float wy, float wz, float ww)
{
	m[0] = xx;  m[1] = xy;  m[2] = xz;  m[3] = xw;
	m[4] = yx;  m[5] = yy;  m[6] = yz;  m[7] = yw;
	m[8] = zx;  m[9] = zy;  m[10]= zz;  m[11]= zw;
	m[12]= wx;  m[13]= wy;  m[14]= wz;  m[15]= ww;
}

inline void  mat4::set(const vec4f& v0, const vec4f& v1,const vec4f& v2,const vec4f& v3)
{
	//rows[0]=v0; rows[1]=v1; rows[2]=v2; rows[3]=v3;
	setRow(0,v0); setRow(1,v1); setRow(2,v2); setRow(3,v3);
}

inline void mat4::setRow(int index, const float row[4])
{
	m[index<<2] = row[0];  m[(index<<2) + 1] = row[1];  m[(index<<2) + 2] = row[2];  m[(index<<2) + 3] = row[3];
	/*
	rows[index][0]=row[0];	rows[index][1]=row[1];	
	rows[index][2]=row[2];  rows[index][3]=row[3];
	*/
}



inline void mat4::setRow(int index, const vec4f& v)
{
	//m[index*4] = v.x;  m[index*4 + 1] = v.y;  m[index*4 + 2] = v.z;  m[index*4 + 3] = v.w;
	m[index<<2] = v.x();  m[(index<<2) + 1] = v.y();  m[(index<<2) + 2] = v.z();  m[(index<<2) + 3] = v.w();
	//rows[index] = v;
}

inline void mat4::setRow(int index, const vec3f& v)
{
	//m[index*4] = v.x;  m[index*4 + 1] = v.y;  m[index*4 + 2] = v.z;
	m[index<<2] = v.x();  m[(index<<2) + 1] = v.y();  m[(index<<2) + 2] = v.z();
	//rows[index] = vec4f(v,rows[index].w());
}

inline void mat4::setColumn(int index, const float col[4])
{
	m[index] = col[0];  m[index + 4] = col[1];  m[index + 8] = col[2];  m[index + 12] = col[3];
}

inline void mat4::setColumn(int index, const vec4f& v)
{
	m[index] = v.x();  m[index + 4] = v.y();  m[index + 8] = v.z();  m[index + 12] = v.w();
}

inline void mat4::setColumn(int index, const vec3f& v)
{
	m[index] = v.x();  m[index + 4] = v.y();  m[index + 8] = v.z();
}

inline const float* mat4::get() const
{
	return m;
}

inline float mat4::get(int r, int c) const
{
	//return rows[r][c];
	return m[(r<<2)+c];
}

inline float& mat4::get( int r, int c )
{
	//return rows[r][c];
	return m[(r<<2)+c];
}

inline	vec4f mat4::getRow(int r) const
{
	//return rows[r];
	return vec4f(m[r<<2], m[(r<<2)+1], m[(r<<2)+2], m[(r<<2)+3]);
}

/*
inline const float* mat4::getTranspose()
{
	tm[0] = m[0];   tm[1] = m[4];   tm[2] = m[8];   tm[3] = m[12];
	tm[4] = m[1];   tm[5] = m[5];   tm[6] = m[9];   tm[7] = m[13];
	tm[8] = m[2];   tm[9] = m[6];   tm[10]= m[10];  tm[11]= m[14];
	tm[12]= m[3];   tm[13]= m[7];   tm[14]= m[11];  tm[15]= m[15];
	return tm;
}
*/

inline mat4& mat4::identity()
{
	m[0] = m[5] = m[10] = m[15] = 1.0f;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	return *this;
}

inline mat4 mat4::operator+(const mat4& rhs) const
{
	/*
	return mat4(m[0]+rhs[0],   m[1]+rhs[1],   m[2]+rhs[2],   m[3]+rhs[3],
				   m[4]+rhs[4],   m[5]+rhs[5],   m[6]+rhs[6],   m[7]+rhs[7],
				   m[8]+rhs[8],   m[9]+rhs[9],   m[10]+rhs[10], m[11]+rhs[11],
				   m[12]+rhs[12], m[13]+rhs[13], m[14]+rhs[14], m[15]+rhs[15]);
	return mat4(rows[0]+rhs.rows[0],
				rows[1]+rhs.rows[1],
				rows[2]+rhs.rows[2],
				rows[3]+rhs.rows[3]);
	*/
	return mat4(getRow(0)+rhs.getRow(0),
				getRow(1)+rhs.getRow(1),
				getRow(2)+rhs.getRow(2),
				getRow(3)+rhs.getRow(3));
}



inline mat4 mat4::operator-(const mat4& rhs) const
{
	/*
	return mat4(m[0]-rhs[0],   m[1]-rhs[1],   m[2]-rhs[2],   m[3]-rhs[3],
				   m[4]-rhs[4],   m[5]-rhs[5],   m[6]-rhs[6],   m[7]-rhs[7],
				   m[8]-rhs[8],   m[9]-rhs[9],   m[10]-rhs[10], m[11]-rhs[11],
				   m[12]-rhs[12], m[13]-rhs[13], m[14]-rhs[14], m[15]-rhs[15]);
	return mat4(rows[0]-rhs.rows[0],
				rows[1]-rhs.rows[1],
				rows[2]-rhs.rows[2],
				rows[3]-rhs.rows[3]);
	*/
	return mat4(getRow(0)-rhs.getRow(0),
				getRow(1)-rhs.getRow(1),
				getRow(2)-rhs.getRow(2),
				getRow(3)-rhs.getRow(3));
}

inline mat4& mat4::operator+=(const mat4& rhs)
{
	/*
	m[0] += rhs[0];    m[1] += rhs[1];    m[2] += rhs[2];    m[3] += rhs[3];
	m[4] += rhs[4];    m[5] += rhs[5];    m[6] += rhs[6];    m[7] += rhs[7];
	m[8] += rhs[8];    m[9] += rhs[9];    m[10] += rhs[10];  m[11] += rhs[11];
	m[12] += rhs[12];  m[13] += rhs[13];  m[14] += rhs[14];  m[15] += rhs[15];
	rows[0]+=rhs.rows[0];
	rows[1]+=rhs.rows[1];
	rows[2]+=rhs.rows[2];
	rows[3]+=rhs.rows[3];
	*/
	setRow(0, getRow(0)+rhs.getRow(0));
	setRow(1, getRow(1)+rhs.getRow(1));
	setRow(2, getRow(2)+rhs.getRow(2));
	setRow(3, getRow(3)+rhs.getRow(3));
	return *this;
}

inline mat4& mat4::operator-=(const mat4& rhs)
{
	/*
	m[0] -= rhs[0];    m[1] -= rhs[1];    m[2] -= rhs[2];    m[3] -= rhs[3];
	m[4] -= rhs[4];    m[5] -= rhs[5];    m[6] -= rhs[6];    m[7] -= rhs[7];
	m[8] -= rhs[8];    m[9] -= rhs[9];    m[10] -= rhs[10];  m[11] -= rhs[11];
	m[12] -= rhs[12];  m[13] -= rhs[13];  m[14] -= rhs[14];  m[15] -= rhs[15];
	rows[0]-=rhs.rows[0];
	rows[1]-=rhs.rows[1];
	rows[2]-=rhs.rows[2];
	rows[3]-=rhs.rows[3];
	*/
	setRow(0, getRow(0)-rhs.getRow(0));
	setRow(1, getRow(1)-rhs.getRow(1));
	setRow(2, getRow(2)-rhs.getRow(2));
	setRow(3, getRow(3)-rhs.getRow(3));
	return *this;
}



inline vec4f mat4::operator*(const vec4f& rhs) const
{
	/*
	return vec4f(m[0]*rhs.x  + m[1]*rhs.y  + m[2]*rhs.z  + m[3]*rhs.w,
				   m[4]*rhs.x  + m[5]*rhs.y  + m[6]*rhs.z  + m[7]*rhs.w,
				   m[8]*rhs.x  + m[9]*rhs.y  + m[10]*rhs.z + m[11]*rhs.w,
				   m[12]*rhs.x + m[13]*rhs.y + m[14]*rhs.z + m[15]*rhs.w);
	return vec4f(rows[0].dot(rhs),
				 rows[1].dot(rhs),
				 rows[2].dot(rhs),
				 rows[3].dot(rhs));
	*/
	return vec4f(getRow(0).dot(rhs),
				 getRow(1).dot(rhs),
				 getRow(2).dot(rhs),
				 getRow(3).dot(rhs));
}



inline vec3f mat4::operator*(const vec3f& rhs) const
{
	return vec3f(m[0]*rhs.x() + m[1]*rhs.y() + m[2]*rhs.z(),
				 m[4]*rhs.x() + m[5]*rhs.y() + m[6]*rhs.z(),
				 m[8]*rhs.x() + m[9]*rhs.y() + m[10]*rhs.z());
}



inline mat4 mat4::operator*(const mat4& n) const
{
	return mat4(m[0]*n.m[0]  + m[1]*n.m[4]  + m[2]*n.m[8]  + m[3]*n.m[12],   m[0]*n.m[1]  + m[1]*n.m[5]  + m[2]*n.m[9]  + m[3]*n.m[13],   m[0]*n.m[2]  + m[1]*n.m[6]  + m[2]*n.m[10]  + m[3]*n.m[14],   m[0]*n.m[3]  + m[1]*n.m[7]  + m[2]*n.m[11]  + m[3]*n.m[15],
				m[4]*n.m[0]  + m[5]*n.m[4]  + m[6]*n.m[8]  + m[7]*n.m[12],   m[4]*n.m[1]  + m[5]*n.m[5]  + m[6]*n.m[9]  + m[7]*n.m[13],   m[4]*n.m[2]  + m[5]*n.m[6]  + m[6]*n.m[10]  + m[7]*n.m[14],   m[4]*n.m[3]  + m[5]*n.m[7]  + m[6]*n.m[11]  + m[7]*n.m[15],
				m[8]*n.m[0]  + m[9]*n.m[4]  + m[10]*n.m[8] + m[11]*n.m[12],  m[8]*n.m[1]  + m[9]*n.m[5]  + m[10]*n.m[9] + m[11]*n.m[13],  m[8]*n.m[2]  + m[9]*n.m[6]  + m[10]*n.m[10] + m[11]*n.m[14],  m[8]*n.m[3]  + m[9]*n.m[7]  + m[10]*n.m[11] + m[11]*n.m[15],
				m[12]*n.m[0] + m[13]*n.m[4] + m[14]*n.m[8] + m[15]*n.m[12],  m[12]*n.m[1] + m[13]*n.m[5] + m[14]*n.m[9] + m[15]*n.m[13],  m[12]*n.m[2] + m[13]*n.m[6] + m[14]*n.m[10] + m[15]*n.m[14],  m[12]*n.m[3] + m[13]*n.m[7] + m[14]*n.m[11] + m[15]*n.m[15]);
}

inline mat4& mat4::operator*=(const mat4& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool mat4::operator==(const mat4& n) const
{
	/*
	return (m[0] == n[0])   && (m[1] == n[1])   && (m[2] == n[2])   && (m[3] == n[3]) &&
		   (m[4] == n[4])   && (m[5] == n[5])   && (m[6] == n[6])   && (m[7] == n[7]) &&
		   (m[8] == n[8])   && (m[9] == n[9])   && (m[10] == n[10]) && (m[11] == n[11]) &&
		   (m[12] == n[12]) && (m[13] == n[13]) && (m[14] == n[14]) && (m[15] == n[15]);
	return  (rows[0]==n.rows[0]) &&
			(rows[1]==n.rows[1]) &&
			(rows[2]==n.rows[2]) &&
			(rows[3]==n.rows[3]); 
	*/
	return  (getRow(0)==n.getRow(0)) &&
			(getRow(1)==n.getRow(1)) &&
			(getRow(2)==n.getRow(2)) &&
			(getRow(3)==n.getRow(3)); 
}

inline bool mat4::operator!=(const mat4& n) const
{
	/*
	return (m[0] != n[0])   || (m[1] != n[1])   || (m[2] != n[2])   || (m[3] != n[3]) ||
		   (m[4] != n[4])   || (m[5] != n[5])   || (m[6] != n[6])   || (m[7] != n[7]) ||
		   (m[8] != n[8])   || (m[9] != n[9])   || (m[10] != n[10]) || (m[11] != n[11]) ||
		   (m[12] != n[12]) || (m[13] != n[13]) || (m[14] != n[14]) || (m[15] != n[15]);
	return  (rows[0]!=n.rows[0]) || 
			(rows[1]!=n.rows[1]) ||
			(rows[2]!=n.rows[2]) ||
			(rows[3]!=n.rows[3]); 
	*/
	return  (getRow(0)!=n.getRow(0)) ||
			(getRow(1)!=n.getRow(1)) ||
			(getRow(2)!=n.getRow(2)) ||
			(getRow(3)!=n.getRow(3)); 
}

/*
inline float mat4::operator[](int index) const
{
	return m[index];
}

inline float& mat4::operator[](int index)
{
	return m[index];
}
*/

inline vec4f mat4::operator[](int index) const
{
	return getRow(0);//rows[index];
}

inline float* mat4::operator[](int index)
{
	//return rows[index];
	return &m[index<<2];
}

inline mat4 operator-(const mat4& rhs)
{
	//return mat4(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
	//return mat4(-rhs.rows[0], -rhs.rows[1], -rhs.rows[2], -rhs.rows[3]);
	return mat4(-rhs.getRow(0), -rhs.getRow(1), -rhs.getRow(2), -rhs.getRow(3));
}



inline mat4 operator*(float s, const mat4& rhs)
{
	//return mat4(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8], s*rhs[9], s*rhs[10], s*rhs[11], s*rhs[12], s*rhs[13], s*rhs[14], s*rhs[15]);
	//return mat4(s*rhs.rows[0],s*rhs.rows[1],s*rhs.rows[2],s*rhs.rows[3]);
	return mat4(s*rhs.getRow(0),s*rhs.getRow(1),s*rhs.getRow(2),s*rhs.getRow(3));
}

inline vec4f operator*(const vec4f& v, const mat4& m)
{
	return vec4f(v.x()*m.m[0] + v.y()*m.m[4] + v.z()*m.m[8]  + v.w()*m.m[12],
				 v.x()*m.m[1] + v.y()*m.m[5] + v.z()*m.m[9]  + v.w()*m.m[13],
				 v.x()*m.m[2] + v.y()*m.m[6] + v.z()*m.m[10] + v.w()*m.m[14],
				 v.x()*m.m[3] + v.y()*m.m[7] + v.z()*m.m[11] + v.w()*m.m[15]);
}

inline vec3f operator*(const vec3f& v, const mat4& m)
{
	return vec3f(v.x()*m.m[0] + v.y()*m.m[4] + v.z()*m.m[8],
				 v.x()*m.m[1] + v.y()*m.m[5] + v.z()*m.m[9],
				 v.x()*m.m[2] + v.y()*m.m[6] + v.z()*m.m[10]);
}

inline std::ostream& operator<<(std::ostream& os, const mat4& m)
{
	os << "(" << m.m[0]  << ",\t" << m.m[1]  << ",\t" << m.m[2]  <<  ",\t" << m.m[3] << ")\n"
	   << "(" << m.m[4]  << ",\t" << m.m[5]  << ",\t" << m.m[6]  <<  ",\t" << m.m[7] << ")\n"
	   << "(" << m.m[8]  << ",\t" << m.m[9]  << ",\t" << m.m[10] <<  ",\t" << m.m[11] << ")\n"
	   << "(" << m.m[12] << ",\t" << m.m[13] << ",\t" << m.m[14] <<  ",\t" << m.m[15] << ")\n";
	return os;
}
}//end of namespace
// END OF MATRIX4 INLINE //////////////////////////////////////////////////////
#endif
