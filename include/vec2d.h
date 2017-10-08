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

#ifndef _VEC2D_H_
#define _VEC2D_H_
#pragma once
#include <cmath>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

namespace davinci{
    class vec2i;
    class vec2f;

    class vec2d
    {
    private:
        union{
            struct{
                double m_x, m_y;
            };
            struct{
                double m_r, m_g;
            };
            struct{
                double m_v[2];
            };
        };
    public:
        vec2d(void):m_x(0.0),m_y(0.0) { };
        vec2d(double x, double y) :m_x(x),m_y(y) { };
        explicit vec2d(double s):m_x(s),m_y(s) { };
        explicit vec2d(const vec2i& v);
        explicit vec2d(const double d[2]){ m_v[0]=d[0]; m_v[1]=d[1];};
        explicit vec2d(const float d[2]){ m_v[0]=d[0]; m_v[1]=d[1];};

        double x() const {return m_x;}
        double y() const {return m_y;}
        double r() const {return m_r;}
        double g() const {return m_g;}
        double v(int i) const { return m_v[i];}
        void  setX(double x) {m_x = x;}
        void  setY(double y) {m_y = y;}
        void  setR(double r) {m_r = r;}
        void  setG(double g) {m_g = g;}

        double lengthSquared() const {
            return (m_x*m_x+m_y*m_y);
        }
        double length() const {
            return sqrt(lengthSquared());
        }
        double max_norm(const vec2d& v){
            double dx, dy;
            dx = fabs(m_x - v.x());
            dy = fabs(m_y - v.y());
            return max(dx,dy);
        }
        void  normalize(){
            double inv = length();
            if (inv==0.0){
                return;
            }
            inv = 1.0/inv;
            m_x *= inv;
            m_y *= inv;
        }
        double dot(const vec2d &v) const { return x()*v.x()+y()*v.y();};
        
        double max_extend1f() const{
            double dx, dy, val;
            dx = fabs(x());
            dy = fabs(y());
            val = max(dx, dy);
            return val;
        }
        //return maximum extend between two doubles.
        friend  double max_extend2d(const vec2d &v1,const vec2d &v2){
            double dx, dy, v;
            dx = fabs(v1.m_x - v2.m_x);
            dy = fabs(v1.m_y - v2.m_y);
            v = max(dx, dy);
            return v;
        }
        //return the maximum absolute value of current vec2d.
        double min_extend1f() const{
            double dx, dy, val;
            dx = fabs(x());
            dy = fabs(y());
            val = min(dx,dy);
            return val;
        }
        friend  double min_extend2f(const vec2d &v1,const vec2d &v2){
            double dx, dy, v;
            dx = fabs(v1.m_x - v2.m_x);
            dy = fabs(v1.m_y - v2.m_y);
            v = min(dx, dy);
            return v;
        }
        operator double*() { return m_v;};

        double  operator [](int i) const{
            return m_v[i];
        };

        double& operator [](int i){
            return m_v[i];
        };

        vec2d& operator = (const vec2i &v);
        vec2d& operator = (const vec2f &v);
        
        vec2d operator + (const vec2d &v) const{
            return vec2d(x()+v.x(), y()+v.y());
        }
        vec2d operator + (const double s) const{
            return vec2d(x()+s, y()+s);
        }
        vec2d operator - (const vec2d &v) const{
            return vec2d(x()-v.x(), y()-v.y());
        }
        vec2d operator - (const double s) const{
            return vec2d(x()-s, y()-s);
        }
        
        vec2d operator - () const{
            return vec2d(-m_x, -m_y);
        }
        vec2d operator * (const vec2d &v) const{
            return vec2d(x()*v.x(), y()*v.y());
        }
        vec2d operator * (const double  s) const{
            return vec2d(x()*s, y()*s);
        }
        vec2d operator / (const vec2d &v) const{
            return vec2d(x()/v.x(), y()/v.y());
        }
        vec2d operator / (const double  s) const{
            return vec2d(x()/s, y()/s);
        }
        bool  operator == (const vec2d &v)const{
            return (x()==v.x() && y()==v.y());
        }
        bool  operator != (const vec2d &v)const{
            return !(*this == v);
        }
        vec2d& operator += (const vec2d &v){
            m_x += v.x();
            m_y += v.y();
            return *this;
        }
        vec2d& operator += (double s){
            m_x += s;
            m_y += s;
            return *this;
        }
        vec2d& operator -= (const vec2d &v){
            m_x -= v.x();
            m_y -= v.y();
            return *this;
        }
        vec2d& operator -= (double s){
            m_x -= s;
            m_y -= s;
            return *this;
        }
        vec2d& operator *= (const vec2d &v){
            m_x *= v.x();
            m_y *= v.y();
            return *this;
        }
        vec2d& operator *= (const double  s){
            m_x *= s;
            m_y *= s;
            return *this;
        }
        vec2d& operator /= (const vec2d &v){
            m_x /= v.x();
            m_y /= v.y();
            return *this;
        }
        vec2d& operator /= (const double  s){
            m_x /= s;
            m_y /= s;
            return *this;
        }
        //lexicographical order.
        bool operator < (const vec2d& v) const{
                 if (m_x < v.m_x) return true;
            else if (m_x > v.m_x) return false;
            else  return (m_y < v.m_y);
        }
        bool operator > (const vec2d& v) const{
                 if (m_x > v.m_x) return true;
            else if (m_x < v.m_x) return false;
            else  return (m_y > v.m_y);
        }
        friend vec2d operator * (const double s, const vec2d &v){
            return v*s;
        }
        //for standard I/O and File input
        friend ostream& operator << (ostream& os, const vec2d& v)
        {
            os<<"("<<v.x()<<", "<<v.y()<<")\n";
            return os;
        };
        friend istream& operator >> (istream& is, vec2d& v)
        {
            is >> v[0] >> v[1];
            return is;
        };
        //for File Output
        friend ofstream& operator << (ofstream& os, const vec2d& v)
        {
            os<<" "<<v.x()<<" "<<v.y()<<"\n";
            return os;
        };
        friend string& operator << (string& str, const vec2d& v)
        {
            stringstream ss;
            ss <<"("<<v.x()<<", "<<v.y()<<")";
            str += ss.str();
            return str;
        };
    };//end vec2f
};//end math namespace

#endif

