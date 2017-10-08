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
using namespace davinci;

const float DEG2RAD = 3.141593f / 180;

///////////////////////////////////////////////////////////////////////////////
// return the determinant of 2x2 matrix
///////////////////////////////////////////////////////////////////////////////
float mat2::getDeterminant()
{
    return m[0] * m[3] - m[1] * m[2];
}

///////////////////////////////////////////////////////////////////////////////
// inverse of 2x2 matrix
// If cannot find inverse, set identity matrix
///////////////////////////////////////////////////////////////////////////////
mat2& mat2::inverse()
{
    float determinant = m[0] * m[3] - m[1] * m[2];
    if(fabs(determinant) <= 0.00001f)
    {
        return identity();
    }

    float tmp = m[0];   // copy the first element
    float invDeterminant = 1.0f / determinant;
    m[0] =  invDeterminant * m[3];
    m[1] = -invDeterminant * m[1];
    m[2] = -invDeterminant * m[2];
    m[3] =  invDeterminant * tmp;

    return *this;
}
