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

#include "GLQuaternion.h"
using namespace davinci;

Quaternion::Quaternion(void)
    :m_vec3d(vec3f(0.0f)),m_w(1.0f)
{
}

Quaternion::Quaternion( float w, float x, float y, float z )
    :m_vec3d(vec3f(x,y,z)),m_w(w)
{
}

Quaternion::Quaternion( float w, const vec3f &nv )
    :m_vec3d(nv),m_w(w)
{
}

Quaternion::~Quaternion(void)
{
}

vec4f Quaternion::vector4d() const
{
    return vec4f(m_vec3d,m_w);
}

void Quaternion::normalized()
{
    float inv_mag;
    inv_mag    = 1.0f/vector4d().length();
    m_vec3d[0] *= inv_mag;
    m_vec3d[1] *= inv_mag;
    m_vec3d[2] *= inv_mag;
    m_w        *= inv_mag;
}

// Quaternion& Quaternion::operator*=( float factor )
// {
//     return *this;
// }

Quaternion& Quaternion::operator*=( const Quaternion &q )
{
    float  W   = m_w*q.w() - m_vec3d.dot(q.vector3d());
    vec3f VEC = m_w*q.vector3d()+q.w()*m_vec3d+m_vec3d.cross(q.vector3d());
    m_w = W;
    m_vec3d = VEC;
    normalized();
    return *this;
}

Quaternion& Quaternion::operator+=( const Quaternion &q )
{
    m_vec3d += q.vector3d();
    m_w     += q.w();
    return *this;
}
Quaternion Quaternion::operator*(const float factor) const
{
    return Quaternion(w()*factor, vector3d()*factor);
}
Quaternion Quaternion::operator+( const Quaternion &q ) const
{
    return Quaternion(x()+q.x(), vector3d()+q.vector3d());
}

Quaternion Quaternion::fromAxisAndAngle( const vec3f& axis, float angle )
{
    vec3f vec  = axis;//normalize(axis);
    vec.normalize();
    vec *= sin(angle*0.5f);
    return Quaternion(cos(angle*0.5f),vec);
}

Quaternion Quaternion::fromAxisAndAngle( float x, float y, float z, float angle )
{
    return Quaternion::fromAxisAndAngle(vec3f(x,y,z),angle);
}

Quaternion Quaternion::conjugate() const
{
    return Quaternion(w(), -x(),-y(),-z());
}

//
// Quaternion Quaternion::nlerp( const Quaternion& q1, const Quaternion& q2, float t )
// {
// }
//
Quaternion Quaternion::slerp( const Quaternion& q1, const Quaternion& q2, float t )
{
    Quaternion ret = q1*(1.0f - t) + q2*t;
    ret.normalized();
    return ret;
}