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
// Matrice.cpp
// ===========
// NxN Matrix Math classes
//
// All matrices are row major. (OpenGL uses column-major matrix)
// | 0 1 |    | 0 1 2 |    |  0  1  2  3 |
// | 2 3 |    | 3 4 5 |    |  4  5  6  7 |
//            | 6 7 8 |    |  8  9 10 11 |
//                         | 12 13 14 15 |
//
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <algorithm>
#include "mat2.h"
#include "mat3.h"
#include "mat4.h"
using namespace davinci;

//const float DEG2RAD = 3.141593f / 180;
///////////////////////////////////////////////////////////////////////////////
// transpose 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::transpose()
{
    std::swap(m[1],  m[4]);
    std::swap(m[2],  m[8]);
    std::swap(m[3],  m[12]);
    std::swap(m[6],  m[9]);
    std::swap(m[7],  m[13]);
    std::swap(m[11], m[14]);

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// inverse 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::inverse()
{
    // If the 4th row is [0,0,0,1] then it is affine matrix and
    // it has no projective transformation.
    //if(// rows[3] == vec4f(0.0f,0.0f,0.0f,1.0f))
    if(m[12] == 0 && m[13] == 0 && m[14] == 0 && m[15] == 1)
        this->invertAffine();
    else
    {
        this->invertGeneral();
        /*@@ invertProjective() is not optimized (slower than generic one)
        if(fabs(m[0]*m[5] - m[1]*m[4]) > 0.00001f)
            this->invertProjective();   // inverse using matrix partition
        else
            this->invertGeneral();      // generalized inverse
        */
    }

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// compute the inverse of 4x4 Euclidean transformation matrix
//
// Euclidean transformation is translation, rotation, and reflection.
// With Euclidean transform, only the position and orientation of the object
// will be changed. Euclidean transform does not change the shape of an object
// (no scaling). Length and angle are reserved.
//
// Use inverseAffine() if the matrix has scale and shear transformation.
//
// M = [ R | T ]
//     [ --+-- ]    (R denotes 3x3 rotation/reflection matrix)
//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
//
// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^T*y - R^T*T
// (R is orthogonal,  R^-1 = R^T)
//
//  [ R | T ]-1    [ R^T | -R^T * T ]    (R denotes 3x3 rotation matrix)
//  [ --+-- ]   =  [ ----+--------- ]    (T denotes 1x3 translation)
//  [ 0 | 1 ]      [  0  |     1    ]    (R^T denotes R-transpose)
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::invertEuclidean()
{
    // transpose 3x3 rotation matrix part
    // | R^T | 0 |
    // | ----+-- |
    // |  0  | 1 |
    float tmp;
    tmp = m[1];  m[1] = m[4];  m[4] = tmp;
    tmp = m[2];  m[2] = m[8];  m[8] = tmp;
    tmp = m[6];  m[6] = m[9];  m[9] = tmp;

    // compute translation part -R^T * T
    // | 0 | -R^T x |
    // | --+------- |
    // | 0 |   0    |
    float x = m[3];
    float y = m[7];
    float z = m[11];
    m[3]  = -(m[0] * x + m[1] * y + m[2] * z);
    m[7]  = -(m[4] * x + m[5] * y + m[6] * z);
    m[11] = -(m[8] * x + m[9] * y + m[10]* z);

    // last row should be unchanged (0,0,0,1)

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// compute the inverse of a 4x4 affine transformation matrix
//
// Affine transformations are generalizations of Euclidean transformations.
// Affine transformation includes translation, rotation, reflection, scaling,
// and shearing. Length and angle are NOT preserved.
// M = [ R | T ]
//     [ --+-- ]    (R denotes 3x3 rotation/scale/shear matrix)
//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
//
// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^-1*y - R^-1*T
//
//  [ R | T ]-1   [ R^-1 | -R^-1 * T ]
//  [ --+-- ]   = [ -----+---------- ]
//  [ 0 | 1 ]     [  0   +     1     ]
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::invertAffine()
{
    // R^-1
    mat3 r(m[0],m[1],m[2], m[4],m[5],m[6], m[8],m[9],m[10]);
    r.inverse();
    
    setRow(0, vec4f(r.getRow(0), m[3]));//rows[0] = vec4f(r.rows[0], m[3]);
    setRow(1, vec4f(r.getRow(1), m[7]));//rows[1] = vec4f(r.rows[1], m[7]);
    setRow(2, vec4f(r.getRow(2), m[11]));//rows[2] = vec4f(r.rows[2], m[11]);

    // -R^-1 * T
    float x = m[3];
    float y = m[7];
    float z = m[11];
    m[3]  = -(r[0] * x + r[1] * y + r[2] * z);
    m[7]  = -(r[3] * x + r[4] * y + r[5] * z);
    m[11] = -(r[6] * x + r[7] * y + r[8] * z);

    // last row should be unchanged (0,0,0,1)
    //m[12] = m[13] = m[14] = 0.0f;
    //m[15] = 1.0f;

    return * this;
}

///////////////////////////////////////////////////////////////////////////////
// inverse matrix using matrix partitioning (block-wise inverse)
// It divides a 4x4 matrix into 4 of 2x2 matrices. It works in case of where
// det(A) != 0. If not, use the generic inverse method
// inverse formula.
// M = [ A | B ]    A, B, C, D are 2x2 matrix blocks
//     [ --+-- ]    det(M) = |A| * |D - ((C * A^-1) * B)|
//     [ C | D ]
//
// M^-1 = [ A' | B' ]   A' = A^-1 - (A^-1 * B) * C'
//        [ ---+--- ]   B' = (A^-1 * B) * -D'
//        [ C' | D' ]   C' = -D' * (C * A^-1)
//                      D' = (D - ((C * A^-1) * B))^-1
//
// NOTE: I wrap with () if it it used more than once.
//       The matrix is invertible even if det(A)=0, so must check det(A) before
//       calling this function, and use invertGeneric() instead.
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::invertProjective()
{
    // partition
    mat2 a(m[0], m[1], m[4], m[5]);
    mat2 b(m[2], m[3], m[6], m[7]);
    mat2 c(m[8], m[9], m[12], m[13]);
    mat2 d(m[10], m[11], m[14], m[15]);

    // pre-compute repeated parts
    a.inverse();         // A^-1
    mat2 ab = a * b;     // A^-1 * B
    mat2 ca = c * a;     // C * A^-1
    mat2 cab = ca * b;   // C * A^-1 * B
    mat2 dcab = d - cab; // D - C * A^-1 * B

    // check determinant if |D - C * A^-1 * B| = 0
    //NOTE: this function assumes det(A) is already checked. if |A|=0 then,
    //      cannot use this function.
    float determinant = dcab[0] * dcab[3] - dcab[1] * dcab[2];
    if(fabs(determinant) <= 0.00001f)
    {
        return identity();
    }

    // compute D' and -D'
    mat2 d1 = dcab;      //  (D - C * A^-1 * B)
    d1.inverse();        //  (D - C * A^-1 * B)^-1
    mat2 d2 = -d1;       // -(D - C * A^-1 * B)^-1

    // compute C'
    mat2 c1 = d2 * ca;   // -D' * (C * A^-1)

    // compute B'
    mat2 b1 = ab * d2;   // (A^-1 * B) * -D'

    // compute A'
    mat2 a1 = a - (ab * c1); // A^-1 - (A^-1 * B) * C'

    // assemble inverse matrix
    m[0] = a1[0];  m[1] = a1[1];  m[2] = b1[0];  m[3] = b1[1];
    m[4] = a1[2];  m[5] = a1[3];  m[6] = b1[1];  m[2] = b1[3];
    m[8] = c1[0];  m[9] = c1[1];  m[10]= d1[0];  m[11]= d1[1];
    m[12]= c1[2];  m[13]= c1[3];  m[14]= d1[2];  m[15]= d1[3];

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// compute the inverse of a general 4x4 matrix using Cramer's Rule
// If cannot find inverse, return identity matrix
// M^-1 = adj(M) / det(M)
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::invertGeneral()
{
    // get cofactors of minor matrices
    float cofactor0 = getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]);
    float cofactor1 = getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]);
    float cofactor2 = getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]);
    float cofactor3 = getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);

    // get determinant
    float determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
    if(fabs(determinant) <= 0.00001f)
    {
        return identity();
    }

    // get rest of cofactors for adj(M)
    float cofactor4 = getCofactor(m[1],m[2],m[3], m[9],m[10],m[11], m[13],m[14],m[15]);
    float cofactor5 = getCofactor(m[0],m[2],m[3], m[8],m[10],m[11], m[12],m[14],m[15]);
    float cofactor6 = getCofactor(m[0],m[1],m[3], m[8],m[9], m[11], m[12],m[13],m[15]);
    float cofactor7 = getCofactor(m[0],m[1],m[2], m[8],m[9], m[10], m[12],m[13],m[14]);

    float cofactor8 = getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[13],m[14],m[15]);
    float cofactor9 = getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[12],m[14],m[15]);
    float cofactor10= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[12],m[13],m[15]);
    float cofactor11= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[12],m[13],m[14]);

    float cofactor12= getCofactor(m[1],m[2],m[3], m[5],m[6], m[7],  m[9], m[10],m[11]);
    float cofactor13= getCofactor(m[0],m[2],m[3], m[4],m[6], m[7],  m[8], m[10],m[11]);
    float cofactor14= getCofactor(m[0],m[1],m[3], m[4],m[5], m[7],  m[8], m[9], m[11]);
    float cofactor15= getCofactor(m[0],m[1],m[2], m[4],m[5], m[6],  m[8], m[9], m[10]);

    // build inverse matrix = adj(M) / det(M)
    // adjugate of M is the transpose of the cofactor matrix of M
    float invDeterminant = 1.0f / determinant;
    m[0] =  invDeterminant * cofactor0;
    m[1] = -invDeterminant * cofactor4;
    m[2] =  invDeterminant * cofactor8;
    m[3] = -invDeterminant * cofactor12;

    m[4] = -invDeterminant * cofactor1;
    m[5] =  invDeterminant * cofactor5;
    m[6] = -invDeterminant * cofactor9;
    m[7] =  invDeterminant * cofactor13;

    m[8] =  invDeterminant * cofactor2;
    m[9] = -invDeterminant * cofactor6;
    m[10]=  invDeterminant * cofactor10;
    m[11]= -invDeterminant * cofactor14;

    m[12]= -invDeterminant * cofactor3;
    m[13]=  invDeterminant * cofactor7;
    m[14]= -invDeterminant * cofactor11;
    m[15]=  invDeterminant * cofactor15;

    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// return determinant of 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
float mat4::getDeterminant()
{
    return m[0] * getCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]) -
           m[1] * getCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]) +
           m[2] * getCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]) -
           m[3] * getCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);
}

///////////////////////////////////////////////////////////////////////////////
// compute cofactor of 3x3 minor matrix without sign
// input params are 9 elements of the minor matrix
// NOTE: The caller must know its sign.
///////////////////////////////////////////////////////////////////////////////
float mat4::getCofactor(float m0, float m1, float m2,
                        float m3, float m4, float m5,
                        float m6, float m7, float m8)
{
    return m0 * (m4 * m8 - m5 * m7) -
           m1 * (m3 * m8 - m5 * m6) +
           m2 * (m3 * m7 - m4 * m6);
}

///////////////////////////////////////////////////////////////////////////////
// translate this matrix by (x, y, z)
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::translate(const vec3f& v)
{
    return translate(v.x(), v.y(), v.z());
}

mat4& mat4::translate(float x, float y, float z)
{
    m[0] += m[12]*x;   m[1] += m[13]*x;   m[2] += m[14]*x;   m[3] += m[15]*x;
    m[4] += m[12]*y;   m[5] += m[13]*y;   m[6] += m[14]*y;   m[7] += m[15]*y;
    m[8] += m[12]*z;   m[9] += m[13]*z;   m[10]+= m[14]*z;   m[11]+= m[15]*z;
    /*
    rows[0]+=(rows[3]*x); 
    rows[1]+=(rows[3]*y);
    rows[2]+=(rows[3]*z);
    */
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// uniform scale
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::scale(float s)
{
    return scale(s, s, s);
}

mat4& mat4::scale(float x, float y, float z)
{
    /*
    rows[0]*=x;
    rows[1]*=y;
    rows[2]*=z;
    */
    setRow(0, getRow(0)*x);
    setRow(1, getRow(1)*y);
    setRow(2, getRow(2)*z);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// build a rotation matrix with given angle(degree) and rotation axis(through origin),
// then multiply it with this object
// Reference:
// http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
// http://science.kennesaw.edu/~plaval/math4490/rotgen.pdf
// http://en.wikipedia.org/wiki/Rotation_matrix
///////////////////////////////////////////////////////////////////////////////
mat4& mat4::rotate(float angle, const vec3f& axis)
{
    return rotate(angle, axis.x(), axis.y(), axis.z());
}

mat4& mat4::rotate(float angle, float x, float y, float z)
{
    float c = cosf(angle);    // cosine
    float s = sinf(angle);    // sine
    //normalize rotation vector
    float inv_sqrt = 1.0f/sqrt(x*x+y*y+z*z);
    x*=inv_sqrt; y*=inv_sqrt; z*=inv_sqrt;

    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float yy = y * y;
    float yz = y * z;
    float zz = z * z;

    // build rotation matrix
    mat4 m;
    m.m[0] = xx * (1 - c) + c;
    m.m[1] = xy * (1 - c) - z * s;
    m.m[2] = xz * (1 - c) + y * s;
    m.m[3] = 0;
    m.m[4] = xy * (1 - c) + z * s;
    m.m[5] = yy * (1 - c) + c;
    m.m[6] = yz * (1 - c) - x * s;
    m.m[7] = 0;
    m.m[8] = xz * (1 - c) - y * s;
    m.m[9] = yz * (1 - c) + x * s;
    m.m[10]= zz * (1 - c) + c;
    m.m[11]= 0;
    m.m[12]= 0;
    m.m[13]= 0;
    m.m[14]= 0;
    m.m[15]= 1;

    // multiply it
    *this = m * (*this);

    return *this;
}

mat4& mat4::rotateX(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    
    vec4f r1=getRow(1);//rows[1];
    vec4f r2=getRow(2);
    /*
    rows[1] = rows[1]*c-rows[2]*s;
    rows[2] = r1	* s+rows[2]*c;
    */
    setRow(1, r1*c-r2*s);
    setRow(2, r1*s+r2*c);
    return *this;
}

mat4& mat4::rotateY(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    
    vec4f r0 = getRow(0);//rows[0];
    vec4f r2 = getRow(2);
    /*
    rows[0] =  rows[0]*c+rows[2]*s;
    rows[2] = -r0	*  s+rows[2]*c;
    */
    setRow(0,  r0*c+r2*s);
    setRow(2, -r0*s+r2*c);

    return *this;
}

mat4& mat4::rotateZ(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    
    vec4f r0= getRow(0);//rows[0];
    vec4f r1= getRow(1);//rows[0];
    /*
    rows[0] = rows[0]*c - rows[1]*s;
    rows[1] = r0	* s + rows[1]*c;
    */
    setRow(0, r0*c - r1*s);
    setRow(1, r0*s + r1*c);
    return *this;
}

mat4& davinci::mat4::rotated( float angle, const vec3f& axis )
{
    return rotate(angle*DEG2RAD, axis);
}

mat4& davinci::mat4::rotated( float angle, float x, float y, float z )
{
    return rotate(angle*DEG2RAD, x, y, z);
}

mat4& davinci::mat4::rotateXd( float angle )
{
    return rotateX(angle * DEG2RAD);
}

mat4& davinci::mat4::rotateYd( float angle )
{
    return rotateY(angle * DEG2RAD);
}

mat4& davinci::mat4::rotateZd( float angle )
{
    return rotateZ(angle * DEG2RAD);
}

//http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
davinci::mat3 davinci::mat4::getNormalMatrix()
{
    //Extract upper 3x4 sub-matrix.
    //mat3 normalMatrix(rows[0].xyz(), rows[1].xyz(), rows[2].xyz());
    mat3 normalMatrix(getRow(0).xyz(), getRow(1).xyz(), getRow(2).xyz());
    return normalMatrix.inverse().transpose();
}

mat4 mat4::createPerpProjMatrix( float l, float r, float b, float t, float n, float f )
{
    mat4 projMtx;
    projMtx.identity();
    projMtx[0][0] = 2.0f * n / (r - l);
    projMtx[0][2] = (r + l) / (r - l);
    projMtx[1][1] = 2.0f * n / (t - b);
    projMtx[1][2] = (t + b) / (t - b);
    projMtx[2][2]= -(f + n) / (f - n);
    projMtx[2][3]= - (2.0f * f * n) /(f - n);
    projMtx[3][2]= -1.0f;
    projMtx[3][3]=  0.0f;
    
    return projMtx;
}

mat4 mat4::createPerpProjMatrix( float fovY, float aspect, float front, float back )
{
    float tangent = tanf(fovY*0.5f * DEG2RAD);
    float height = front * tangent;//half height of near plane
    float width = height * aspect;//half width of near plane

    return createPerpProjMatrix(-width, width, -height, height, front, back);
}

mat4 mat4::createOrthoProjMatrix( float l, float r, float b, float t, float n, float f )
{
    mat4 projMtx;
    projMtx.identity();
    projMtx[0][0]  = 2.0f / (r - l);
    projMtx[0][3]  = -(r + l) / (r - l);
    projMtx[1][1]  = 2.0f / (t - b);
    projMtx[1][3]  = -(t + b) / (t - b);
    projMtx[2][2] = -2.0f / (f - n);
    projMtx[2][3] = -(f + n) / (f - n);
    
    return projMtx;
}

