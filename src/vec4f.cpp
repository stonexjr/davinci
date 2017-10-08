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

#include "vec4i.h"
#include "vec3f.h"
#include "vec4f.h"
//using namespace mathtool;
namespace davinci{

vec4f::vec4f( const vec3f& v, float w )
{
    m_x = v.x(); m_y = v.y(); m_z = v.z();
    m_w = w;
}

vec4f::vec4f( const vec4i& v )
{
    m_x = (float)v.x();
    m_y = (float)v.y();
    m_z = (float)v.z();
    m_w = (float)v.w();
}

vec4f::vec4f(const vec2f& v, float z, float w)
{
    m_x = v.x(), m_y = v.y(), m_z = z, m_w = w;
}

vec4f& vec4f::operator=( const vec4i &v )
{
    m_x = (float)v.x();
    m_y = (float)v.y();
    m_z = (float)v.z();
    m_w = (float)v.w();
    return *this;
}

}

