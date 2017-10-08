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

#ifndef _VEC4D_H_
#define _VEC4D_H_
#pragma once
#include <cmath>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "vec2d.h"
using namespace std;

namespace davinci{
    class vec4i;
    class vec3d;
    class vec4d
    {
    private:
        union{
            struct{
                double m_x, m_y, m_z, m_w;
            };
            struct{
                double m_r, m_g, m_b, m_a;
            };
            struct{
                double m_v[4];
            };
        };
    public:
        vec4d(void):m_x(0),m_y(0),m_z(0), m_w(0){};
        vec4d(double x, double y, double z, double w):m_x(x),m_y(y),m_z(z), m_w(w){};
        vec4d(const vec3d& v, double w);
        explicit vec4d(double s):m_x(s),m_y(s),m_z(s), m_w(s){};
        explicit vec4d(const vec4i& v);
        explicit vec4d(const double vs[4]):m_x(vs[0]),m_y(vs[1]),m_z(vs[2]), m_w(vs[3]){};

        double x() const {return m_x;}
        double y() const {return m_y;}
        double z() const {return m_z;}
        double w() const {return m_w;}
        double r() const {return m_r;}
        double g() const {return m_g;}
        double b() const {return m_b;}
        double a() const {return m_a;}
        double v(int i) const { return m_v[i];}
        void  setX(double x) {m_x = x;}
        void  setY(double y) {m_y = y;}
        void  setZ(double z) {m_z = z;}
        void  setW(double w) {m_w = w;}
        void  setR(double r) {m_r = r;}
        void  setG(double g) {m_g = g;}
        void  setB(double b) {m_b = b;}
        void  setA(double a) {m_a = a;}

        double lengthSquared() const {
            return (m_x*m_x+m_y*m_y+m_z*m_z+m_w*m_w);
        }
        double length() const {
            return sqrt(lengthSquared());
        }
        double max_norm(const vec4d& v){
            double dx, dy, dz, dw;
            dx = fabs(m_x - v.x());
            dy = fabs(m_y - v.y());
            dz = fabs(m_z - v.z());
            dw = fabs(m_w - v.w());
            return max(dx,max(dy,max(dz,dw)));
        }
        void  normalize(){
            double inv = length();
            if (inv==0.0){
                return;
            }
            inv = 1.0/inv;
            m_x *= inv;
            m_y *= inv;
            m_z *= inv;
            m_w *= inv;
        }
        double dot(const vec4d &v) const { return x()*v.x()+y()*v.y()+z()*v.z()+w()*v.w();};
        double max_extend1d() const{
            double dx, dy, dz, dw, val;
            dx = fabs(x());
            dy = fabs(y());
            dz = fabs(z());
            dw = fabs(w());
            val = max(dx, max(dy, max(dz, dw)));
            return val;
        }
        friend  double max_extend2d(const vec4d &v1,const vec4d &v2){
            double dx, dy, dz, dw, v;
            dx = fabs(v1.m_x - v2.m_x);
            dy = fabs(v1.m_y - v2.m_y);
            dz = fabs(v1.m_z - v2.m_z);
            dw = fabs(v1.m_w - v2.m_w);
            v = max(dx, max(dy, max(dz,dw)));
            return v;
        }
        operator double*() { return m_v;};
        double  operator [](int i) const{return m_v[i];};
        double& operator [](int i){return m_v[i];};

        vec4d& operator = (const vec4i &v);
        vec4d& operator = (const vec4f &v);
        vec4d operator + (const vec4d &v) const{
            return vec4d(x()+v.x(), y()+v.y(), z()+v.z(), w()+v.w());
        }
        vec4d operator + (const double s) const{
            return vec4d(x()+s, y()+s, z()+s, w()+s);
        }
        vec4d operator - (const vec4d &v) const{
            return vec4d(x()-v.x(), y()-v.y(), z()-v.z(), w()-v.w());
        }
        vec4d operator - (const double s) const{
            return vec4d(x()-s, y()-s, z()-s, w()-s);
        }
        vec4d operator - () const {
            return vec4d(-m_x, -m_y, -m_z, -m_w);
        }
        vec4d operator * (const vec4d &v) const{
            return vec4d(x()*v.x(), y()*v.y(), z()*v.z(), w()*v.w());
        }
        vec4d operator * (const double  s) const{
            return vec4d(x()*s, y()*s, z()*s, w()*s);
        }
        vec4d operator / (const vec4d &v) const{
            return vec4d(x()/v.x(), y()/v.y(), z()/v.z(), w()/v.w());
        }
        vec4d operator / (const double  s) const{
            return vec4d(x()/s, y()/s, z()/s, w()/s);
        }
        bool  operator == (const vec4d &v)const{
            return (x()==v.x() && y()==v.y() && z()==v.z() && w()==v.w());
        }
        bool  operator != (const vec4d &v)const{
            return !(*this == v);
        }
        vec4d& operator += (const vec4d &v){
            m_x += v.x();
            m_y += v.y();
            m_z += v.z();
            m_w += v.w();
            return *this;
        }
        vec4d& operator += (double s){
            m_x += s;
            m_y += s;
            m_z += s;
            m_w += s;
            return *this;
        }
        vec4d& operator -= (const vec4d &v){
            m_x -= v.x();
            m_y -= v.y();
            m_z -= v.z();
            m_w -= v.w();
            return *this;
        }
        vec4d& operator -= (double s){
            m_x -= s;
            m_y -= s;
            m_z -= s;
            m_w -= s;
            return *this;
        }
        vec4d& operator *= (const vec4d &v){
            m_x *= v.x();
            m_y *= v.y();
            m_z *= v.z();
            m_w *= v.w();
            return *this;
        }
        vec4d& operator *= (const double  s){
            m_x *= s;
            m_y *= s;
            m_z *= s;
            m_w *= s;
            return *this;
        }
        vec4d& operator /= (const vec4d &v){
            m_x /= v.x();
            m_y /= v.y();
            m_z /= v.z();
            m_w /= v.w();
            return *this;
        }
        vec4d& operator /= (const double  s){
            m_x /= s;
            m_y /= s;
            m_z /= s;
            m_w /= s;
            return *this;
        }
        //lexicographical order.
        bool operator < (const vec4d& v) const{
                 if (m_x < v.m_x) return true;
            else if (m_x > v.m_x) return false;
            else if (m_y < v.m_y) return true;
            else if (m_y > v.m_y) return false;
            else if (m_z < v.m_z) return true;
            else if (m_z > v.m_z) return false;
            else return (m_w < v.m_w);
        }
        bool operator > (const vec4d& v) const{
                 if (m_x > v.m_x) return true;
            else if (m_x < v.m_x) return false;
            else if (m_y > v.m_y) return true;
            else if (m_y < v.m_y) return false;
            else if (m_z > v.m_z) return true;
            else if (m_z < v.m_z) return false;
            else return (m_w > v.m_w);
        }
        friend vec4d operator * (const double s, const vec4d &v){
            return v*s;
        }
        //for standard I/O and File input
        friend ostream& operator << (ostream& os, const vec4d& v)
        {
            os<<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w()<<")\n";
            return os;
        };
        friend istream& operator >> (istream& is, vec4d& v)
        {
            is >> v[0] >> v[1] >> v[2] >> v[3];
            return is;
        };
        //for File Output
        friend ofstream& operator << (ofstream& os, const vec4d& v)
        {
            os<<" "<<v.x()<<" "<<v.y()<<" "<<v.z()<<" "<<v.w()<<"\n";
            return os;
        };
        friend string& operator << (string& str, const vec4d& v)
        {
            stringstream ss;
            ss <<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<", "<<v.w()<<")";
            str += ss.str();
            return str;
        };
        //swizzling operation
        vec2d xy() const { return vec2d(m_x, m_y);}
        vec2d yx() const { return vec2d(m_y, m_x);}
        vec2d yz() const { return vec2d(m_y, m_z);}
        vec2d zy() const { return vec2d(m_z, m_y);}
        vec2d xz() const { return vec2d(m_x, m_z);}
        vec2d zx() const { return vec2d(m_z, m_x);}
        vec2d rg() const { return xy();}
        vec2d gr() const { return yx();}
        vec2d gb() const { return yz();}
        vec2d bg() const { return zy();}
        vec2d rb() const { return xz();}
        vec2d br() const { return zx();}
        vec3d zyx() const { return vec3d (m_z,m_y,m_x);}
        vec3d yzx() const { return vec3d (m_y,m_z,m_x);}
        vec3d yxz() const { return vec3d (m_y,m_x,m_z);}
        vec3d zxy() const { return vec3d (m_z,m_x,m_y);}
        vec3d xzy() const { return vec3d (m_x,m_z,m_y);}
        vec3d xyz() const { return vec3d (m_x,m_y,m_z);}
        vec4d wzyx() const { return vec4d (m_w,m_z,m_y,m_x);}
        vec4d zwyx() const { return vec4d (m_z,m_w,m_y,m_x);}
        vec4d zywx() const { return vec4d (m_z,m_y,m_w,m_x);}
        vec4d zyxw() const { return vec4d (m_z,m_y,m_x,m_w);}
        vec4d wyzx() const { return vec4d (m_w,m_y,m_z,m_x);}
        vec4d ywzx() const { return vec4d (m_y,m_w,m_z,m_x);}
        vec4d yzwx() const { return vec4d (m_y,m_z,m_w,m_x);}
        vec4d yzxw() const { return vec4d (m_y,m_z,m_x,m_w);}
        vec4d wyxz() const { return vec4d (m_w,m_y,m_x,m_z);}
        vec4d ywxz() const { return vec4d (m_y,m_w,m_x,m_z);}
        vec4d yxwz() const { return vec4d (m_y,m_x,m_w,m_z);}
        vec4d yxzw() const { return vec4d (m_y,m_x,m_z,m_w);}
        vec4d wzxy() const { return vec4d (m_w,m_z,m_x,m_y);}
        vec4d zwxy() const { return vec4d (m_z,m_w,m_x,m_y);}
        vec4d zxwy() const { return vec4d (m_z,m_x,m_w,m_y);}
        vec4d zxyw() const { return vec4d (m_z,m_x,m_y,m_w);}
        vec4d wxzy() const { return vec4d (m_w,m_x,m_z,m_y);}
        vec4d xwzy() const { return vec4d (m_x,m_w,m_z,m_y);}
        vec4d xzwy() const { return vec4d (m_x,m_z,m_w,m_y);}
        vec4d xzyw() const { return vec4d (m_x,m_z,m_y,m_w);}
        vec4d wxyz() const { return vec4d (m_w,m_x,m_y,m_z);}
        vec4d xwyz() const { return vec4d (m_x,m_w,m_y,m_z);}
        vec4d xywz() const { return vec4d (m_x,m_y,m_w,m_z);}
        vec4d xyzw() const { return vec4d (m_x,m_y,m_z,m_w);}
        vec3d bgr() const { return vec3d (m_b,m_g,m_r);}
        vec3d gbr() const { return vec3d (m_g,m_b,m_r);}
        vec3d grb() const { return vec3d (m_g,m_r,m_b);}
        vec3d brg() const { return vec3d (m_b,m_r,m_g);}
        vec3d rbg() const { return vec3d (m_r,m_b,m_g);}
        vec3d rgb() const { return vec3d (m_r,m_g,m_b);}
        vec4d abgr() const { return vec4d (m_a,m_b,m_g,m_r);}
        vec4d bagr() const { return vec4d (m_b,m_a,m_g,m_r);}
        vec4d bgar() const { return vec4d (m_b,m_g,m_a,m_r);}
        vec4d bgra() const { return vec4d (m_b,m_g,m_r,m_a);}
        vec4d agbr() const { return vec4d (m_a,m_g,m_b,m_r);}
        vec4d gabr() const { return vec4d (m_g,m_a,m_b,m_r);}
        vec4d gbar() const { return vec4d (m_g,m_b,m_a,m_r);}
        vec4d gbra() const { return vec4d (m_g,m_b,m_r,m_a);}
        vec4d agrb() const { return vec4d (m_a,m_g,m_r,m_b);}
        vec4d garb() const { return vec4d (m_g,m_a,m_r,m_b);}
        vec4d grab() const { return vec4d (m_g,m_r,m_a,m_b);}
        vec4d grba() const { return vec4d (m_g,m_r,m_b,m_a);}
        vec4d abrg() const { return vec4d (m_a,m_b,m_r,m_g);}
        vec4d barg() const { return vec4d (m_b,m_a,m_r,m_g);}
        vec4d brag() const { return vec4d (m_b,m_r,m_a,m_g);}
        vec4d brga() const { return vec4d (m_b,m_r,m_g,m_a);}
        vec4d arbg() const { return vec4d (m_a,m_r,m_b,m_g);}
        vec4d rabg() const { return vec4d (m_r,m_a,m_b,m_g);}
        vec4d rbag() const { return vec4d (m_r,m_b,m_a,m_g);}
        vec4d rbga() const { return vec4d (m_r,m_b,m_g,m_a);}
        vec4d argb() const { return vec4d (m_a,m_r,m_g,m_b);}
        vec4d ragb() const { return vec4d (m_r,m_a,m_g,m_b);}
        vec4d rgab() const { return vec4d (m_r,m_g,m_a,m_b);}
        vec4d rgba() const { return vec4d (m_r,m_g,m_b,m_a);}
    };//end vec4d
};//end math namespace

#endif

