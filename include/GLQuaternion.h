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

#ifndef _QUATERNION_H_
#define _QUATERNION_H_
#pragma once
#include "vec3f.h"
#include "vec4f.h"
/*
 * Quaternions always obey:  a^2 + b^2 + c^2 + d^2 = 1.0
 * If they don't add up to 1.0, dividing by their magnitude will
 * re-normalize them.
 *
 * Note: See the following for more information on quaternions:
 *
 * - Shoemake, K., Animating rotation with quaternion curves, Computer
 *   Graphics 19, No 3 (Proc. SIGGRAPH'85), 245-254, 1985.
 * - Pletinckx, D., Quaternion calculus as a basic tool in computer
 *   graphics, The Visual Computer 5, 2-13, 1989.
 */
namespace davinci{
class Quaternion
{
public:
    Quaternion(void);
    Quaternion(float w, float x, float y, float z);
    Quaternion(float w, const vec3f &nv);
    ~Quaternion(void);
    void setW(float w){ m_w = w;}
    void setX(float x){ m_vec3d.setX(x);}
    void setY(float y){ m_vec3d.setY(y);}
    void setZ(float z){ m_vec3d.setZ(z);}
    void setVector(const vec3f& v){
        m_vec3d[0] = v.x(); m_vec3d[1] = v.y(); m_vec3d[2] = v.z();
    }
    void setIdentity(){
        setVector(vec3f(0.0f));
        setW(1.0f);
    }
    float x() const{ return m_vec3d.x();}
    float y() const{ return m_vec3d.y();}
    float z() const{ return m_vec3d.z();}
    float w() const{ return m_w;}
    vec3f vector3d() const { return m_vec3d;}
    vec4f vector4d() const;
    void   normalized();
    Quaternion conjugate() const;
//    Quaternion& operator*=(float factor);
    //
/*
 * Quaternion multiplication
 * http://en.wikipedia.org/wiki/Quaternion#Multiplication_of_basis_elements
 *
 * Given two quaternions, multiply them together to get a third quaternion.
 * Multiplying quaternions to get a compound rotation is analogous to adding
 * translations to get a compound translation.  When incrementally
 * Multiplying rotations, the first argument here should be the new
 * rotation, the return quaternion is the total rotation (which will be
 * over-written with the resulting new total rotation).
 */
    Quaternion& operator*=(const Quaternion &q);
    Quaternion& operator+=(const Quaternion &q);
    Quaternion  operator* (const float factor) const;
    Quaternion  operator+ (const Quaternion &q) const;
    friend ostream& operator << (ostream& os, const Quaternion& q)
    {
        os<<"("<<q.x()<<", "<<q.y()<<", "<<q.z()<<q.w()<<")\n";
        return os;
    };
static Quaternion fromAxisAndAngle( const vec3f& axis, float angle);
static Quaternion fromAxisAndAngle( float x, float y, float z, float angle);
static Quaternion nlerp(const Quaternion& q1, const Quaternion& q2, float t);
static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);

private:
    vec3f  m_vec3d;
    float  m_w;
};
}
#endif