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

#ifndef _VEC4F_H_
#define _VEC4F_H_
#pragma once
#include <cmath>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "vec2f.h"
#include "vec3f.h"
using namespace std;

namespace davinci{
    class vec4i;
    class vec3f;
    class vec4f
    {
    private:
        union{
            struct{
                float m_x, m_y, m_z, m_w;
            };
            struct{
                float m_r, m_g, m_b, m_a;
            };
            struct{
                float m_v[4];
            };
        };
    public:
        vec4f(void):m_x(0),m_y(0),m_z(0), m_w(0){};
        vec4f(float x, float y, float z, float w):m_x(x),m_y(y),m_z(z), m_w(w){};
        vec4f(const vec2f& v, float z, float w);
        vec4f(const vec3f& v, float w);
        explicit vec4f(float s):m_x(s),m_y(s),m_z(s), m_w(s){};
        explicit vec4f(const vec4i& v);
        explicit vec4f(const float vs[4]):m_x(vs[0]),m_y(vs[1]),m_z(vs[2]), m_w(vs[3]){};

        float x() const {return m_x;}
        float y() const {return m_y;}
        float z() const {return m_z;}
        float w() const {return m_w;}
        float r() const {return m_r;}
        float g() const {return m_g;}
        float b() const {return m_b;}
        float a() const {return m_a;}
        float v(int i) const { return m_v[i];}
        void  setX(float x) {m_x = x;}
        void  setY(float y) {m_y = y;}
        void  setZ(float z) {m_z = z;}
        void  setW(float w) {m_w = w;}
        void  setR(float r) {m_r = r;}
        void  setG(float g) {m_g = g;}
        void  setB(float b) {m_b = b;}
        void  setA(float a) {m_a = a;}

        float lengthSquared() const {
            return (m_x*m_x+m_y*m_y+m_z*m_z+m_w*m_w);
        }
        float length() const {
            return sqrtf(lengthSquared());
        }
        float max_norm(const vec4f& v){
            float dx, dy, dz, dw;
            dx = fabs(m_x - v.x());
            dy = fabs(m_y - v.y());
            dz = fabs(m_z - v.z());
            dw = fabs(m_w - v.w());
            return max(dx,max(dy,max(dz,dw)));
        }
        void  normalize(){
            float inv = length();
            if (inv==0.0f){
                return;
            }
            inv = 1.0f/inv;
            m_x *= inv;
            m_y *= inv;
            m_z *= inv;
            m_w *= inv;
        }
        float dot(const vec4f &v) const { return x()*v.x()+y()*v.y()+z()*v.z()+w()*v.w();};
        float max_extend1f() const{
            float dx, dy, dz, dw, val;
            dx = fabsf(x());
            dy = fabsf(y());
            dz = fabsf(z());
            dw = fabsf(w());
            val = max(dx, max(dy, max(dz, dw)));
            return val;
        }
        friend  float max_extend2f(const vec4f &v1,const vec4f &v2){
            float dx, dy, dz, dw, v;
            dx = fabsf(v1.m_x - v2.m_x);
            dy = fabsf(v1.m_y - v2.m_y);
            dz = fabsf(v1.m_z - v2.m_z);
            dw = fabsf(v1.m_w - v2.m_w);
            v = max(dx, max(dy, max(dz,dw)));
            return v;
        }
        operator float*() { return m_v;};
        float  operator [](int i) const{return m_v[i];};
        float& operator [](int i){return m_v[i];};

        vec4f& operator = (const vec4i &v);
        vec4f operator + (const vec4f &v) const{
            return vec4f(x()+v.x(), y()+v.y(), z()+v.z(), w()+v.w());
        }
        vec4f operator + (const float s) const{
            return vec4f(x()+s, y()+s, z()+s, w()+s);
        }
        vec4f operator - (const vec4f &v) const{
            return vec4f(x()-v.x(), y()-v.y(), z()-v.z(), w()-v.w());
        }
        vec4f operator - (const float s) const{
            return vec4f(x()-s, y()-s, z()-s, w()-s);
        }
        vec4f operator - () const {
            return vec4f(-m_x, -m_y, -m_z, -m_w);
        }
        vec4f operator * (const vec4f &v) const{
            return vec4f(x()*v.x(), y()*v.y(), z()*v.z(), w()*v.w());
        }
        vec4f operator * (const float  s) const{
            return vec4f(x()*s, y()*s, z()*s, w()*s);
        }
        vec4f operator / (const vec4f &v) const{
            return vec4f(x()/v.x(), y()/v.y(), z()/v.z(), w()/v.w());
        }
        vec4f operator / (const float  s) const{
            return vec4f(x()/s, y()/s, z()/s, w()/s);
        }
        bool  operator == (const vec4f &v)const{
            return (x()==v.x() && y()==v.y() && z()==v.z() && w()==v.w());
        }
        bool  operator != (const vec4f &v)const{
            return !(*this == v);
        }
        vec4f& operator += (const vec4f &v){
            m_x += v.x();
            m_y += v.y();
            m_z += v.z();
            m_w += v.w();
            return *this;
        }
        vec4f& operator += (float s){
            m_x += s;
            m_y += s;
            m_z += s;
            m_w += s;
            return *this;
        }
        vec4f& operator -= (const vec4f &v){
            m_x -= v.x();
            m_y -= v.y();
            m_z -= v.z();
            m_w -= v.w();
            return *this;
        }
        vec4f& operator -= (float s){
            m_x -= s;
            m_y -= s;
            m_z -= s;
            m_w -= s;
            return *this;
        }
        vec4f& operator *= (const vec4f &v){
            m_x *= v.x();
            m_y *= v.y();
            m_z *= v.z();
            m_w *= v.w();
            return *this;
        }
        vec4f& operator *= (const float  s){
            m_x *= s;
            m_y *= s;
            m_z *= s;
            m_w *= s;
            return *this;
        }
        vec4f& operator /= (const vec4f &v){
            m_x /= v.x();
            m_y /= v.y();
            m_z /= v.z();
            m_w /= v.w();
            return *this;
        }
        vec4f& operator /= (const float  s){
            m_x /= s;
            m_y /= s;
            m_z /= s;
            m_w /= s;
            return *this;
        }
        //lexicographical order.
        bool operator < (const vec4f& v) const{
                 if (m_x < v.m_x) return true;
            else if (m_x > v.m_x) return false;
            else if (m_y < v.m_y) return true;
            else if (m_y > v.m_y) return false;
            else if (m_z < v.m_z) return true;
            else if (m_z > v.m_z) return false;
            else return (m_w < v.m_w);
        }
        bool operator > (const vec4f& v) const{
                 if (m_x > v.m_x) return true;
            else if (m_x < v.m_x) return false;
            else if (m_y > v.m_y) return true;
            else if (m_y < v.m_y) return false;
            else if (m_z > v.m_z) return true;
            else if (m_z < v.m_z) return false;
            else return (m_w > v.m_w);
        }
        friend vec4f operator * (const float s, const vec4f &v){
            return v*s;
        }
        //for standard I/O and File input
        friend ostream& operator << (ostream& os, const vec4f& v)
        {
            os<<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w()<<")\n";
            return os;
        };
        friend istream& operator >> (istream& is, vec4f& v)
        {
            is >> v[0] >> v[1] >> v[2] >> v[3];
            return is;
        };
        //for File Output
        friend ofstream& operator << (ofstream& os, const vec4f& v)
        {
            os<<" "<<v.x()<<" "<<v.y()<<" "<<v.z()<<" "<<v.w()<<"\n";
            return os;
        };
        friend string& operator << (string& str, const vec4f& v)
        {
            stringstream ss;
            ss <<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w()<<")";
            str += ss.str();
            return str;
        };
        //swizzling operation
        vec2f xy() const { return vec2f(m_x, m_y);}
        vec2f yx() const { return vec2f(m_y, m_x);}
        vec2f yz() const { return vec2f(m_y, m_z);}
        vec2f zy() const { return vec2f(m_z, m_y);}
        vec2f xz() const { return vec2f(m_x, m_z);}
        vec2f zx() const { return vec2f(m_z, m_x);}
        vec2f rg() const { return xy();}
        vec2f gr() const { return yx();}
        vec2f gb() const { return yz();}
        vec2f bg() const { return zy();}
        vec2f rb() const { return xz();}
        vec2f br() const { return zx();}
        vec3f zyx() const { return vec3f (m_z,m_y,m_x);}
        vec3f yzx() const { return vec3f (m_y,m_z,m_x);}
        vec3f yxz() const { return vec3f (m_y,m_x,m_z);}
        vec3f zxy() const { return vec3f (m_z,m_x,m_y);}
        vec3f xzy() const { return vec3f (m_x,m_z,m_y);}
        vec3f xyz() const { return vec3f (m_x,m_y,m_z);}
        vec4f wzyx() const { return vec4f (m_w,m_z,m_y,m_x);}
        vec4f zwyx() const { return vec4f (m_z,m_w,m_y,m_x);}
        vec4f zywx() const { return vec4f (m_z,m_y,m_w,m_x);}
        vec4f zyxw() const { return vec4f (m_z,m_y,m_x,m_w);}
        vec4f wyzx() const { return vec4f (m_w,m_y,m_z,m_x);}
        vec4f ywzx() const { return vec4f (m_y,m_w,m_z,m_x);}
        vec4f yzwx() const { return vec4f (m_y,m_z,m_w,m_x);}
        vec4f yzxw() const { return vec4f (m_y,m_z,m_x,m_w);}
        vec4f wyxz() const { return vec4f (m_w,m_y,m_x,m_z);}
        vec4f ywxz() const { return vec4f (m_y,m_w,m_x,m_z);}
        vec4f yxwz() const { return vec4f (m_y,m_x,m_w,m_z);}
        vec4f yxzw() const { return vec4f (m_y,m_x,m_z,m_w);}
        vec4f wzxy() const { return vec4f (m_w,m_z,m_x,m_y);}
        vec4f zwxy() const { return vec4f (m_z,m_w,m_x,m_y);}
        vec4f zxwy() const { return vec4f (m_z,m_x,m_w,m_y);}
        vec4f zxyw() const { return vec4f (m_z,m_x,m_y,m_w);}
        vec4f wxzy() const { return vec4f (m_w,m_x,m_z,m_y);}
        vec4f xwzy() const { return vec4f (m_x,m_w,m_z,m_y);}
        vec4f xzwy() const { return vec4f (m_x,m_z,m_w,m_y);}
        vec4f xzyw() const { return vec4f (m_x,m_z,m_y,m_w);}
        vec4f wxyz() const { return vec4f (m_w,m_x,m_y,m_z);}
        vec4f xwyz() const { return vec4f (m_x,m_w,m_y,m_z);}
        vec4f xywz() const { return vec4f (m_x,m_y,m_w,m_z);}
        vec4f xyzw() const { return vec4f (m_x,m_y,m_z,m_w);}
        vec3f bgr() const { return vec3f (m_b,m_g,m_r);}
        vec3f gbr() const { return vec3f (m_g,m_b,m_r);}
        vec3f grb() const { return vec3f (m_g,m_r,m_b);}
        vec3f brg() const { return vec3f (m_b,m_r,m_g);}
        vec3f rbg() const { return vec3f (m_r,m_b,m_g);}
        vec3f rgb() const { return vec3f (m_r,m_g,m_b);}
        vec4f abgr() const { return vec4f (m_a,m_b,m_g,m_r);}
        vec4f bagr() const { return vec4f (m_b,m_a,m_g,m_r);}
        vec4f bgar() const { return vec4f (m_b,m_g,m_a,m_r);}
        vec4f bgra() const { return vec4f (m_b,m_g,m_r,m_a);}
        vec4f agbr() const { return vec4f (m_a,m_g,m_b,m_r);}
        vec4f gabr() const { return vec4f (m_g,m_a,m_b,m_r);}
        vec4f gbar() const { return vec4f (m_g,m_b,m_a,m_r);}
        vec4f gbra() const { return vec4f (m_g,m_b,m_r,m_a);}
        vec4f agrb() const { return vec4f (m_a,m_g,m_r,m_b);}
        vec4f garb() const { return vec4f (m_g,m_a,m_r,m_b);}
        vec4f grab() const { return vec4f (m_g,m_r,m_a,m_b);}
        vec4f grba() const { return vec4f (m_g,m_r,m_b,m_a);}
        vec4f abrg() const { return vec4f (m_a,m_b,m_r,m_g);}
        vec4f barg() const { return vec4f (m_b,m_a,m_r,m_g);}
        vec4f brag() const { return vec4f (m_b,m_r,m_a,m_g);}
        vec4f brga() const { return vec4f (m_b,m_r,m_g,m_a);}
        vec4f arbg() const { return vec4f (m_a,m_r,m_b,m_g);}
        vec4f rabg() const { return vec4f (m_r,m_a,m_b,m_g);}
        vec4f rbag() const { return vec4f (m_r,m_b,m_a,m_g);}
        vec4f rbga() const { return vec4f (m_r,m_b,m_g,m_a);}
        vec4f argb() const { return vec4f (m_a,m_r,m_g,m_b);}
        vec4f ragb() const { return vec4f (m_r,m_a,m_g,m_b);}
        vec4f rgab() const { return vec4f (m_r,m_g,m_a,m_b);}
        vec4f rgba() const { return vec4f (m_r,m_g,m_b,m_a);}
    };//end vec4f
};//end math namespace

#endif

