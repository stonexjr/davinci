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

#ifndef _VEC3D_H_
#define _VEC3D_H_
#pragma once
#include <cmath>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "constant.h"
using namespace std;

namespace davinci{
    class vec3i;
    class vec3f;
    class vec3d
    {
    private:
        union{
            struct{
                double m_x, m_y, m_z;
            };
            struct{
                double m_r, m_g, m_b;
            };
            struct{
                double m_v[3];
            };
        };
    public:
        vec3d(void) :m_x(0.0f),m_y(0.0f),m_z(0.0f) {};
        vec3d(double x, double y, double z):m_x(x),m_y(y),m_z(z){};
        explicit vec3d(const double vs[3]):m_x(vs[0]),m_y(vs[1]),m_z(vs[2]){};
        explicit vec3d(double s):m_x(s),m_y(s),m_z(s) {};
        explicit vec3d(const vec3i& v);
        explicit vec3d(const vec3f& v);

        double x() const {return m_x;}
        double y() const {return m_y;}
        double z() const {return m_z;}
        double r() const {return m_r;}
        double g() const {return m_g;}
        double b() const {return m_b;}
        double v(int i) const { return m_v[i];}
        void  setX(double x) {m_x = x;}
        void  setY(double y) {m_y = y;}
        void  setZ(double z) {m_z = z;}
        void  setR(double r) {m_r = r;}
        void  setG(double g) {m_g = g;}
        void  setB(double b) {m_b = b;}
        double lengthSquared() const {
            return (m_x*m_x+m_y*m_y+m_z*m_z);
        }
        double length() const {
            return sqrt(lengthSquared());
        }
        double max_norm(const vec3d& v){
            double dx, dy, dz;
            dx = abs(m_x - v.x());
            dy = abs(m_y - v.y());
            dz = abs(m_z - v.z());
            return max(dx,max(dy,dz));
        }
        void  normalize(){
            double inv = length();
            if (inv==0.0f){
                return;
            }
            inv = 1.0f/inv;
            m_x *= inv;
            m_y *= inv;
            m_z *= inv;
        }
        double dot(const vec3d &v) const { return x()*v.x()+y()*v.y()+z()*v.z();};
        vec3d cross(const vec3d &v) const{
            return vec3d(
                (y()*v.z() - z()*v.y()),
                (z()*v.x() - x()*v.z()),
                (x()*v.y() - y()*v.x())
                );
        };
        double max_extend1f() const{
            double dx, dy, dz, val;
            dx = abs(x());
            dy = abs(y());
            dz = abs(z());
            val = max(dx, max(dy, dz));
            return val;
        }
        friend  double max_extend2f(const vec3d &v1,const vec3d &v2){
            double dx, dy, dz, v;
            dx = abs(v1.m_x - v2.m_x);
            dy = abs(v1.m_y - v2.m_y);
            dz = abs(v1.m_z - v2.m_z);
            v = max(dx, max(dy, dz));
            return v;
        }
        double min_extend1f() const{
            double dx, dy, dz, val;
            dx = abs(x());
            dy = abs(y());
            dz = abs(z());
            val = min(dx, min(dy, dz));
            return val;
        }
        friend  double min_extend2f(const vec3d &v1,const vec3d &v2){
            double dx, dy, dz, v;
            dx = abs(v1.m_x - v2.m_x);
            dy = abs(v1.m_y - v2.m_y);
            dz = abs(v1.m_z - v2.m_z);
            v = min(dx, min(dy, dz));
            return v;
        }
        operator double*() { return m_v;};
        double  operator [](int i) const { return m_v[i];};
        double& operator [](int i) { return m_v[i];};
        vec3d& operator = (const vec3i &v);
        vec3d operator + (const vec3d &v) const{
            return vec3d(x()+v.x(), y()+v.y(), z()+v.z());
        }
        vec3d operator + (const double s) const{
            return vec3d(x()+s, y()+s, z()+s);
        }
        vec3d operator - (const vec3d &v) const{
            return vec3d(x()-v.x(), y()-v.y(), z()-v.z());
        }
        vec3d operator - (const double s) const{
            return vec3d(x()-s, y()-s, z()-s);
        }
        vec3d operator - () const {
            return vec3d(-m_x,-m_y,-m_z);
        }
        vec3d operator * (const vec3d &v) const{
            return vec3d(x()*v.x(), y()*v.y(), z()*v.z());
        }
        vec3d operator * (const double  s) const{
            return vec3d(x()*s, y()*s, z()*s);
        }
        vec3d operator / (const vec3d &v) const{
            return vec3d(x()/v.x(), y()/v.y(), z()/v.z());
        }
        vec3d operator / (const double  s) const{
            return vec3d(x()/s, y()/s, z()/s);
        }
        bool  operator == (const vec3d &v)const{
            return (x()==v.x() && y()==v.y() && z()==v.z());
        }
        //Component-wise comparison, if equal return 1.0, or else 0.0
        //tor: tolerance
        vec3d equal(const vec3d& v, double tor=0.0f)const{
            vec3d diff = *this - v;
            return vec3d(abs(diff.x())<tor, abs(diff.y())<tor, abs(diff.z())<tor);
        }
        bool  operator != (const vec3d &v)const{
            return !(*this == v);
        }
        vec3d& operator += (const vec3d &v){
            m_x += v.x();
            m_y += v.y();
            m_z += v.z();
            return *this;
        }
        vec3d& operator += (double s){
            m_x += s;
            m_y += s;
            m_z += s;
            return *this;
        }
        vec3d& operator -= (const vec3d &v){
            m_x -= v.x();
            m_y -= v.y();
            m_z -= v.z();
            return *this;
        }
        vec3d& operator -= (double s){
            m_x -= s;
            m_y -= s;
            m_z -= s;
            return *this;
        }
        vec3d& operator *= (const vec3d &v){
            m_x *= v.x();
            m_y *= v.y();
            m_z *= v.z();
            return *this;
        }
        vec3d& operator *= (const double  s){
            m_x *= s;
            m_y *= s;
            m_z *= s;
            return *this;
        }
        vec3d& operator /= (const vec3d &v){
            m_x /= v.x();
            m_y /= v.y();
            m_z /= v.z();
            return *this;
        }
        vec3d& operator /= (const double  s){
            m_x /= s;
            m_y /= s;
            m_z /= s;
            return *this;
        }
        //lexicographical order.
        bool operator < (const vec3d& v) const{
                 if (m_x < v.m_x) return true;
            else if (m_x > v.m_x) return false;
            else if (m_y < v.m_y) return true;
            else if (m_y > v.m_y) return false;
            else return (m_z < v.m_z);
        }
        bool operator > (const vec3d& v) const{
                 if (m_x > v.m_x) return true;
            else if (m_x < v.m_x) return false;
            else if (m_y > v.m_y) return true;
            else if (m_y < v.m_y) return false;
            else return (m_z > v.m_z);
        }
        friend vec3d operator * (const double s, const vec3d &v){
            return v*s;
        }
		//for standard I/O and File input
        friend ostream& operator << (ostream& os, const vec3d& v)
        {
            os<<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<")\n";
            return os;
        };
		friend istream& operator >> (istream& is, vec3d& v)
		{
			is >> v[0] >> v[1] >> v[2];
			return is;
		};
		//for File Output
		friend ofstream& operator << (ofstream& os, const vec3d& v)
        {
            os<<" "<<v.x()<<" "<<v.y()<<" "<<v.z()<<"\n";
            return os;
        };
        friend string& operator << (string& str, const vec3d& v)
        {
            stringstream ss;
            ss <<"("<<v.x()<<", "<<v.y()<<", "<<v.z()<<")";
			str += ss.str();
            return str;
        };
    };//end vec3f
};//end math namespace

#endif

