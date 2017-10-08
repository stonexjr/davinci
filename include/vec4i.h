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

#ifndef _VEC4I_H_
#define _VEC4I_H_
#pragma once
#include <cmath>
#include <iostream>
#include "vec2i.h"
#include "vec3i.h"
#include <algorithm>
using namespace std;

namespace davinci{
class vec4f;

class vec4i
{
private:
    union{
        struct{
            int m_x, m_y, m_z, m_w;
        };
        struct{
            int m_v[4];
        };
    };

public:
    vec4i(void):m_x(0),m_y(0),m_z(0), m_w(0){};
    vec4i(int x, int y, int z, int w):m_x(x),m_y(y),m_z(z), m_w(w){};
    explicit vec4i(int s):m_x(s),m_y(s),m_z(s), m_w(s){};
    explicit vec4i(const vec4f &v);
    explicit vec4i(const int vs[4]):m_x(vs[0]),m_y(vs[1]),m_z(vs[2]), m_w(vs[3]){};

    int x() const {return m_x;}
    int y() const {return m_y;}
    int z() const {return m_z;}
    int w() const {return m_w;}
    int v(int i) const { return m_v[i];}
    void  setX(int x) {m_x = x;}
    void  setY(int y) {m_y = y;}
    void  setZ(int z) {m_z = z;}
    void  setW(int w) {m_w = w;}
    int max_extend1i() const{
        int dx, dy, dz, dw, val;
        dx = (int)std::abs(x());
        dy = (int)std::abs(y());
        dz = (int)std::abs(z());
        dw = (int)std::abs(w());
        val = (int)std::max(dx, std::max(dy, std::max(dz,dw)));
        return val;
    }
friend int max_extend2i(const vec4i &v1,const vec4i &v2){
        int dx, dy, dz, dw, v;
        dx = (int)std::abs(float(v1.m_x - v2.m_x));
        dy = (int)std::abs(float(v1.m_y - v2.m_y));
        dz = (int)std::abs(float(v1.m_z - v2.m_z));
        dw = (int)std::abs(float(v1.m_w - v2.m_w));
        v = std::max(dx, std::max(dy, std::max(dz,dw)));
        return v;
    }
    int max_norm(const vec4i& v){
        int dx, dy, dz, dw;
        dx = std::abs(m_x - v.x());
        dy = std::abs(m_y - v.y());
        dz = std::abs(m_z - v.z());
        dw = std::abs(m_w - v.w());
        return std::max(dx,std::max(dy,std::max(dz,dw)));
    }

    int  operator [](int i) const{return m_v[i];};
    int& operator [](int i){return m_v[i];};

    operator int*() { return m_v;};
    vec4i& operator = (const vec4f &v);
    vec4i operator + (const vec4i &v) const{
        return vec4i(x()+v.x(), y()+v.y(), z()+v.z(), w()+v.w());
    }
    vec4i operator + (const int s) const{
        return vec4i(x()+s, y()+s, z()+s, w()+s);
    }
    vec4i operator - (const vec4i &v) const{
        return vec4i(x()-v.x(), y()-v.y(), z()-v.z(), w()-v.w());
    }
    vec4i operator - (const int s) const{
        return vec4i(x()-s, y()-s, z()-s, w()-s);
    }
    vec4i operator - () const {
        return vec4i(-m_x, -m_y, -m_z, -m_w);
    }
    vec4i operator * (const vec4i &v) const{
        return vec4i(x()*v.x(), y()*v.y(), z()*v.z(), w()*v.w());
    }
    vec4i operator * (const int  s) const{
        return vec4i(x()*s, y()*s, z()*s, w()*s);
    }
    vec4i operator / (const vec4i &v) const{
        return vec4i(x()/v.x(), y()/v.y(), z()/v.z(), w()/v.w());
    }
    vec4i operator / (const int  s) const{
        return vec4i(x()/s, y()/s, z()/s, w()/s);
    }
    bool  operator == (const vec4i &v)const{
        return (x()==v.x() && y()==v.y() && z()==v.z() && w()==v.w());
    }
    bool  operator != (const vec4i &v)const{
        return !(*this == v);
    }
    vec4i& operator += (const vec4i &v){
        m_x += v.x();
        m_y += v.y();
        m_z += v.z();
        m_w += v.w();
        return *this;
    }
    vec4i& operator += (int s){
        m_x += s;
        m_y += s;
        m_z += s;
        m_w += s;
        return *this;
    }
    vec4i& operator -= (const vec4i &v){
        m_x -= v.x();
        m_y -= v.y();
        m_z -= v.z();
        m_w -= v.w();
        return *this;
    }
    vec4i& operator -= (int s){
        m_x -= s;
        m_y -= s;
        m_z -= s;
        m_w -= s;
        return *this;
    }
    vec4i& operator *= (const vec4i &v){
        m_x *= v.x();
        m_y *= v.y();
        m_z *= v.z();
        m_w *= v.w();
        return *this;
    }
    vec4i& operator *= (const int s){
        m_x *= s;
        m_y *= s;
        m_z *= s;
        m_w *= s;
        return *this;
    }
    vec4i& operator /= (const vec4i &v){
        m_x /= v.x();
        m_y /= v.y();
        m_z /= v.z();
        m_w /= v.w();
        return *this;
    }
    vec4i& operator /= (const int s){
        m_x /= s;
        m_y /= s;
        m_z /= s;
        m_w /= s;
        return *this;
    }
    //lexicographical order.
    bool operator < (const vec4i& v) const{
        if (m_x < v.m_x) return true;
        else if (m_x > v.m_x) return false;
        else if (m_y < v.m_y) return true;
        else if (m_y > v.m_y) return false;
        else if (m_z < v.m_z) return true;
        else if (m_z > v.m_z) return false;
        else return (m_w < v.m_w);
    }
    bool operator > (const vec4i& v) const{
        if (m_x > v.m_x) return true;
        else if (m_x < v.m_x) return false;
        else if (m_y > v.m_y) return true;
        else if (m_y < v.m_y) return false;
        else if (m_z > v.m_z) return true;
        else if (m_z < v.m_z) return false;
        else return (m_w > v.m_w);
    }

    friend vec4i operator * (const int s, const vec4i &v){
        return v*s;
    }
    friend ostream& operator << (ostream& os, const vec4i& v)
    {
           os<<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w()<<")\n";
           return os;
    };
    //swizzling operation
    vec2i xy() const { return vec2i(m_x, m_y);}
    vec2i yx() const { return vec2i(m_y, m_x);}
    vec2i yz() const { return vec2i(m_y, m_z);}
    vec2i zy() const { return vec2i(m_z, m_y);}
    vec2i xz() const { return vec2i(m_x, m_z);}
    vec2i zx() const { return vec2i(m_z, m_x);}
    vec3i zyx() const { return vec3i (m_z,m_y,m_x);}
    vec3i yzx() const { return vec3i (m_y,m_z,m_x);}
    vec3i yxz() const { return vec3i (m_y,m_x,m_z);}
    vec3i zxy() const { return vec3i (m_z,m_x,m_y);}
    vec3i xzy() const { return vec3i (m_x,m_z,m_y);}
    vec3i xyz() const { return vec3i (m_x,m_y,m_z);}
    vec4i wzyx() const { return vec4i (m_w,m_z,m_y,m_x);}
    vec4i zwyx() const { return vec4i (m_z,m_w,m_y,m_x);}
    vec4i zywx() const { return vec4i (m_z,m_y,m_w,m_x);}
    vec4i zyxw() const { return vec4i (m_z,m_y,m_x,m_w);}
    vec4i wyzx() const { return vec4i (m_w,m_y,m_z,m_x);}
    vec4i ywzx() const { return vec4i (m_y,m_w,m_z,m_x);}
    vec4i yzwx() const { return vec4i (m_y,m_z,m_w,m_x);}
    vec4i yzxw() const { return vec4i (m_y,m_z,m_x,m_w);}
    vec4i wyxz() const { return vec4i (m_w,m_y,m_x,m_z);}
    vec4i ywxz() const { return vec4i (m_y,m_w,m_x,m_z);}
    vec4i yxwz() const { return vec4i (m_y,m_x,m_w,m_z);}
    vec4i yxzw() const { return vec4i (m_y,m_x,m_z,m_w);}
    vec4i wzxy() const { return vec4i (m_w,m_z,m_x,m_y);}
    vec4i zwxy() const { return vec4i (m_z,m_w,m_x,m_y);}
    vec4i zxwy() const { return vec4i (m_z,m_x,m_w,m_y);}
    vec4i zxyw() const { return vec4i (m_z,m_x,m_y,m_w);}
    vec4i wxzy() const { return vec4i (m_w,m_x,m_z,m_y);}
    vec4i xwzy() const { return vec4i (m_x,m_w,m_z,m_y);}
    vec4i xzwy() const { return vec4i (m_x,m_z,m_w,m_y);}
    vec4i xzyw() const { return vec4i (m_x,m_z,m_y,m_w);}
    vec4i wxyz() const { return vec4i (m_w,m_x,m_y,m_z);}
    vec4i xwyz() const { return vec4i (m_x,m_w,m_y,m_z);}
    vec4i xywz() const { return vec4i (m_x,m_y,m_w,m_z);}
    vec4i xyzw() const { return vec4i (m_x,m_y,m_z,m_w);}
    
};//end of vec4i
};//end of namespace math
#endif

