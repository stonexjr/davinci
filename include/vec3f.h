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

#ifndef _VEC3F_H_
#define _VEC3F_H_
#pragma once
#include <cmath>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "constant.h"
#include "vec2f.h"
using namespace std;

namespace davinci{
    class vec3i;

    class vec3f
    {
    private:
        union{
            struct{
                float m_x, m_y, m_z;
            };
            struct{
                float m_r, m_g, m_b;
            };
            struct{
                float m_v[3];
            };
        };
    public:
        vec3f(void) :m_x(0.0f),m_y(0.0f),m_z(0.0f) {};
        vec3f(float x, float y, float z):m_x(x),m_y(y),m_z(z){};
        explicit vec3f(const vec2f& v2, float v):m_x(v2[0]),m_y(v2[1]),m_z(v){};
        explicit vec3f(const float vs[3]):m_x(vs[0]),m_y(vs[1]),m_z(vs[2]){};
        explicit vec3f(float s):m_x(s),m_y(s),m_z(s) {};
        explicit vec3f(const vec3i& v);

        float x() const {return m_x;}
        float y() const {return m_y;}
        float z() const {return m_z;}
        float v(int i) const { return m_v[i];}
        float r() const {return m_r;}
        float g() const {return m_g;}
        float b() const {return m_b;}
        void  setX(float x) {m_x = x;}
        void  setY(float y) {m_y = y;}
        void  setZ(float z) {m_z = z;}
        void  setR(float r) {m_r = r;}
        void  setG(float g) {m_g = g;}
        void  setB(float b) {m_b = b;}
        
        //
        float lengthSquared() const {
            return (m_x*m_x+m_y*m_y+m_z*m_z);
        }
        float length() const {
            return sqrtf(lengthSquared());
        }
        float max_norm(const vec3f& v){
            float dx, dy, dz;
            dx = fabsf(m_x - v.x());
            dy = fabsf(m_y - v.y());
            dz = fabsf(m_z - v.z());
            return max(dx,max(dy,dz));
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
        }
        float dot(const vec3f &v) const { return x()*v.x()+y()*v.y()+z()*v.z();};
        vec3f cross(const vec3f &v) const{
            return vec3f(
                (y()*v.z() - z()*v.y()),
                (z()*v.x() - x()*v.z()),
                (x()*v.y() - y()*v.x())
                );
        };
        float max_extend1f() const{
            float dx, dy, dz, val;
            dx = fabsf(x());
            dy = fabsf(y());
            dz = fabsf(z());
            val = max(dx, max(dy, dz));
            return val;
        }
        friend  float max_extend2f(const vec3f &v1,const vec3f &v2){
            float dx, dy, dz, v;
            dx = fabsf(v1.m_x - v2.m_x);
            dy = fabsf(v1.m_y - v2.m_y);
            dz = fabsf(v1.m_z - v2.m_z);
            v = max(dx, max(dy, dz));
            return v;
        }
        float min_extend1f() const{
            float dx, dy, dz, val;
            dx = fabsf(x());
            dy = fabsf(y());
            dz = fabsf(z());
            val = min(dx, min(dy, dz));
            return val;
        }
        friend  float min_extend2f(const vec3f &v1,const vec3f &v2){
            float dx, dy, dz, v;
            dx = fabsf(v1.m_x - v2.m_x);
            dy = fabsf(v1.m_y - v2.m_y);
            dz = fabsf(v1.m_z - v2.m_z);
            v = min(dx, min(dy, dz));
            return v;
        }
        operator float*() { return m_v;};
        float  operator [](int i) const { return m_v[i];};
        float& operator [](int i) { return m_v[i];};
        vec3f& operator = (const vec3i &v);
        vec3f operator + (const vec3f &v) const{
            return vec3f(x()+v.x(), y()+v.y(), z()+v.z());
        }
        vec3f operator + (const float s) const{
            return vec3f(x()+s, y()+s, z()+s);
        }
        vec3f operator - (const vec3f &v) const{
            return vec3f(x()-v.x(), y()-v.y(), z()-v.z());
        }
        vec3f operator - (const float s) const{
            return vec3f(x()-s, y()-s, z()-s);
        }
        vec3f operator - () const {
            return vec3f(-m_x,-m_y,-m_z);
        }
        vec3f operator * (const vec3f &v) const{
            return vec3f(x()*v.x(), y()*v.y(), z()*v.z());
        }
        vec3f operator * (const float  s) const{
            return vec3f(x()*s, y()*s, z()*s);
        }
        vec3f operator / (const vec3f &v) const{
            return vec3f(x()/v.x(), y()/v.y(), z()/v.z());
        }
        vec3f operator / (const float  s) const{
            return vec3f(x()/s, y()/s, z()/s);
        }
        bool  operator == (const vec3f &v)const{
            return (x()==v.x() && y()==v.y() && z()==v.z());
        }
        //Component-wise comparison, if equal return 1.0, or else 0.0
        //tor: tolerance
        vec3f equal(const vec3f& v, float tor=0.0f)const{
            vec3f diff = *this - v;
            return vec3f(abs(diff.x())<tor, abs(diff.y())<tor, abs(diff.z())<tor);
        }
        bool  operator != (const vec3f &v)const{
            return !(*this == v);
        }
        vec3f& operator += (const vec3f &v){
            m_x += v.x();
            m_y += v.y();
            m_z += v.z();
            return *this;
        }
        vec3f& operator += (float s){
            m_x += s;
            m_y += s;
            m_z += s;
            return *this;
        }
        vec3f& operator -= (const vec3f &v){
            m_x -= v.x();
            m_y -= v.y();
            m_z -= v.z();
            return *this;
        }
        vec3f& operator -= (float s){
            m_x -= s;
            m_y -= s;
            m_z -= s;
            return *this;
        }
        vec3f& operator *= (const vec3f &v){
            m_x *= v.x();
            m_y *= v.y();
            m_z *= v.z();
            return *this;
        }
        vec3f& operator *= (const float  s){
            m_x *= s;
            m_y *= s;
            m_z *= s;
            return *this;
        }
        vec3f& operator /= (const vec3f &v){
            m_x /= v.x();
            m_y /= v.y();
            m_z /= v.z();
            return *this;
        }
        vec3f& operator /= (const float  s){
            m_x /= s;
            m_y /= s;
            m_z /= s;
            return *this;
        }
        //lexicographical order.
        bool operator < (const vec3f& v) const{
                 if (m_x < v.m_x) return true;
            else if (m_x > v.m_x) return false;
            else if (m_y < v.m_y) return true;
            else if (m_y > v.m_y) return false;
            else return (m_z < v.m_z);
        }
        bool operator > (const vec3f& v) const{
                 if (m_x > v.m_x) return true;
            else if (m_x < v.m_x) return false;
            else if (m_y > v.m_y) return true;
            else if (m_y < v.m_y) return false;
            else return (m_z > v.m_z);
        }
        friend vec3f operator * (const float s, const vec3f &v){
            return v*s;
        }
        //for standard I/O and File input
        friend ostream& operator << (ostream& os, const vec3f& v)
        {
            os<<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<")\n";
            return os;
        };
        friend istream& operator >> (istream& is, vec3f& v)
        {
            is >> v[0] >> v[1] >> v[2];
            return is;
        };
        //for File Output
        friend ofstream& operator << (ofstream& os, const vec3f& v)
        {
            os<<" "<<v.x()<<" "<<v.y()<<" "<<v.z()<<"\n";
            return os;
        };
        friend string& operator << (string& str, const vec3f& v)
        {
            stringstream ss;
            ss <<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<")";
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
        vec3f zyx() const { return vec3f (m_z,m_y,m_x);}
        vec3f yzx() const { return vec3f (m_y,m_z,m_x);}
        vec3f yxz() const { return vec3f (m_y,m_x,m_z);}
        vec3f zxy() const { return vec3f (m_z,m_x,m_y);}
        vec3f xzy() const { return vec3f (m_x,m_z,m_y);}
        vec3f xyz() const { return vec3f (m_x,m_y,m_z);}

        vec2f rg() const { return xy();}
        vec2f gr() const { return yx();}
        vec2f gb() const { return yz();}
        vec2f bg() const { return zy();}
        vec2f rb() const { return xz();}
        vec2f br() const { return zx();}
        vec3f bgr() const { return vec3f (m_b,m_g,m_r);}
        vec3f gbr() const { return vec3f (m_g,m_b,m_r);}
        vec3f grb() const { return vec3f (m_g,m_r,m_b);}
        vec3f brg() const { return vec3f (m_b,m_r,m_g);}
        vec3f rbg() const { return vec3f (m_r,m_b,m_g);}
        vec3f rgb() const { return vec3f (m_r,m_g,m_b);}
    };//end vec3f
};//end math namespace

#endif

