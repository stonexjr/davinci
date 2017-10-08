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
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <algorithm>
#include "mat3.h"
using namespace davinci;
//const float DEG2RAD = 3.141593f / 180;


///////////////////////////////////////////////////////////////////////////////
// return determinant of 3x3 matrix
///////////////////////////////////////////////////////////////////////////////
float mat3::getDeterminant()
{
    return m[0] * (m[4] * m[8] - m[5] * m[7]) -
           m[1] * (m[3] * m[8] - m[5] * m[6]) +
           m[2] * (m[3] * m[7] - m[4] * m[6]);
}

///////////////////////////////////////////////////////////////////////////////
// inverse 3x3 matrix
// If cannot find inverse, set identity matrix
///////////////////////////////////////////////////////////////////////////////
mat3& mat3::inverse()
{
    float determinant, invDeterminant;
    float tmp[9];

    tmp[0] = m[4] * m[8] - m[5] * m[7];
    tmp[1] = m[2] * m[7] - m[1] * m[8];
    tmp[2] = m[1] * m[5] - m[2] * m[4];
    tmp[3] = m[5] * m[6] - m[3] * m[8];
    tmp[4] = m[0] * m[8] - m[2] * m[6];
    tmp[5] = m[2] * m[3] - m[0] * m[5];
    tmp[6] = m[3] * m[7] - m[4] * m[6];
    tmp[7] = m[1] * m[6] - m[0] * m[7];
    tmp[8] = m[0] * m[4] - m[1] * m[3];

    // check if determinant is 0
    determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
    if(fabs(determinant) <= 0.00001f)
    {
        return identity(); // cannot inverse, make it identity matrix
    }

    // divide by the determinant
    invDeterminant = 1.0f / determinant;
    m[0] = invDeterminant * tmp[0];
    m[1] = invDeterminant * tmp[1];
    m[2] = invDeterminant * tmp[2];
    m[3] = invDeterminant * tmp[3];
    m[4] = invDeterminant * tmp[4];
    m[5] = invDeterminant * tmp[5];
    m[6] = invDeterminant * tmp[6];
    m[7] = invDeterminant * tmp[7];
    m[8] = invDeterminant * tmp[8];

    return *this;
}

mat3& davinci::mat3::transpose()
{
//    0,1,2;
//    3,4,5;
//    6,7,8.
    std::swap(m[1],  m[3]);
    std::swap(m[2],  m[6]);
    std::swap(m[5],  m[7]);

    return *this;
}

