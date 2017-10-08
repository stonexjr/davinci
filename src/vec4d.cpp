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
#include "vec3d.h"
#include "vec4f.h"
#include "vec4d.h"
//using namespace mathtool;
namespace davinci{

vec4d::vec4d( const vec3d& v, double w )
{
    m_x = v.x(); m_y = v.y(); m_z = v.z();
    m_w = w;
}

vec4d::vec4d( const vec4i& v )
{
    m_x = (double)v.x();
    m_y = (double)v.y();
    m_z = (double)v.z();
    m_w = (double)v.w();
}

vec4d& vec4d::operator=( const vec4i &v )
{
    m_x = (double)v.x();
    m_y = (double)v.y();
    m_z = (double)v.z();
    m_w = (double)v.w();
    return *this;
}

vec4d& vec4d::operator=( const vec4f &v )
{
    m_x = (double)v.x();
    m_y = (double)v.y();
    m_z = (double)v.z();
    m_w = (double)v.w();
    return *this;
}

}

