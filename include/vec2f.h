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

#ifndef _VEC2F_H_
#define _VEC2F_H_
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

    class vec2f
    {
    private:
        union{
            struct{
                float m_x, m_y;
            };
            struct{
                float m_r, m_g;
            };
            struct{
                float m_v[2];
            };
        };
    public:
        vec2f(void):m_x(0.0f),m_y(0.0f) { };
        vec2f( float x, float y) :m_x(x),m_y(y) { };
        explicit vec2f(float s):m_x(s),m_y(s) { };
        explicit vec2f(const vec2i& v);
        explicit vec2f(const float d[2]){ m_v[0]=d[0]; m_v[1]=d[1];};

        float x() const {return m_x;}
        float y() const {return m_y;}
        float r() const {return m_r;}
        float g() const {return m_g;}
        float v(int i) const { return m_v[i];}
        void  setX(float x) {m_x = x;}
        void  setY(float y) {m_y = y;}
        void  setR(float r) {m_r = r;}
        void  setG(float g) {m_g = g;}

        float lengthSquared() const {
            return (m_x*m_x+m_y*m_y);
        }
        float length() const {
            return sqrtf(lengthSquared());
        }
        float max_norm(const vec2f& v){
            float dx, dy;
            dx = fabsf(m_x - v.x());
            dy = fabsf(m_y - v.y());
            return std::max(dx,dy);
        }
        void  normalize(){
            float inv = length();
            if (inv==0.0f){
                return;
            }
            inv = 1.0f/inv;
            m_x *= inv;
            m_y *= inv;
        }
        float dot(const vec2f &v) const { return x()*v.x()+y()*v.y();};
        
        float max_extend1f() const{
            float dx, dy, val;
            dx = fabsf(x());
            dy = fabsf(y());
            val = std::max(dx, dy);
            return val;
        }
        friend  float max_extend2f(const vec2f &v1,const vec2f &v2){
            float dx, dy, v;
            dx = fabsf(v1.m_x - v2.m_x);
            dy = fabsf(v1.m_y - v2.m_y);
            v = std::max(dx, dy);
            return v;
        }
        float min_extend1f() const{
            float dx, dy, val;
            dx = fabsf(x());
            dy = fabsf(y());
            val = std::min(dx,dy);
            return val;
        }
        friend  float min_extend2f(const vec2f &v1,const vec2f &v2){
            float dx, dy, v;
            dx = fabsf(v1.m_x - v2.m_x);
            dy = fabsf(v1.m_y - v2.m_y);
             v = std::min(dx, dy);
            return v;
        }
        operator float*() { return m_v;};

        float  operator [](int i) const{
            return m_v[i];
        };

        float& operator [](int i){
            return m_v[i];
        };

        vec2f& operator = (const vec2i &v);
        
        vec2f operator + (const vec2f &v) const{
            return vec2f(x()+v.x(), y()+v.y());
        }
        vec2f operator + (const float s) const{
            return vec2f(x()+s, y()+s);
        }
        vec2f operator - (const vec2f &v) const{
            return vec2f(x()-v.x(), y()-v.y());
        }
        vec2f operator - (const float s) const{
            return vec2f(x()-s, y()-s);
        }
        
        vec2f operator - () const{
            return vec2f(-m_x, -m_y);
        }
        vec2f operator * (const vec2f &v) const{
            return vec2f(x()*v.x(), y()*v.y());
        }
        vec2f operator * (const float  s) const{
            return vec2f(x()*s, y()*s);
        }
        vec2f operator / (const vec2f &v) const{
            return vec2f(x()/v.x(), y()/v.y());
        }
        vec2f operator / (const float  s) const{
            return vec2f(x()/s, y()/s);
        }
        bool  operator == (const vec2f &v)const{
            return (x()==v.x() && y()==v.y());
        }
        bool  operator != (const vec2f &v)const{
            return !(*this == v);
        }
        vec2f& operator += (const vec2f &v){
            m_x += v.x();
            m_y += v.y();
            return *this;
        }
        vec2f& operator += (float s){
            m_x += s;
            m_y += s;
            return *this;
        }
        vec2f& operator -= (const vec2f &v){
            m_x -= v.x();
            m_y -= v.y();
            return *this;
        }
        vec2f& operator -= (float s){
            m_x -= s;
            m_y -= s;
            return *this;
        }
        vec2f& operator *= (const vec2f &v){
            m_x *= v.x();
            m_y *= v.y();
            return *this;
        }
        vec2f& operator *= (const float  s){
            m_x *= s;
            m_y *= s;
            return *this;
        }
        vec2f& operator /= (const vec2f &v){
            m_x /= v.x();
            m_y /= v.y();
            return *this;
        }
        vec2f& operator /= (const float  s){
            m_x /= s;
            m_y /= s;
            return *this;
        }
        //lexicographical order.
        bool operator < (const vec2f& v) const{
                 if (m_x < v.m_x) return true;
            else if (m_x > v.m_x) return false;
            else  return (m_y < v.m_y);
        }
        bool operator > (const vec2f& v) const{
                 if (m_x > v.m_x) return true;
            else if (m_x < v.m_x) return false;
            else  return (m_y > v.m_y);
        }
        friend vec2f operator * (const float s, const vec2f &v){
            return v*s;
        }
        //for standard I/O and File input
        friend ostream& operator << (ostream& os, const vec2f& v)
        {
            os<<"("<<v.x()<<", "<<v.y()<<")\n";
            return os;
        };
        friend istream& operator >> (istream& is, vec2f& v)
        {
            is >> v[0] >> v[1];
            return is;
        };
        //for File Output
        friend ofstream& operator << (ofstream& os, const vec2f& v)
        {
            os<<" "<<v.x()<<" "<<v.y()<<"\n";
            return os;
        };
        friend string& operator << (string& str, const vec2f& v)
        {
            stringstream ss;
            ss <<"("<<v.x()<<", "<<v.y()<<")";
            str += ss.str();
            return str;
        };
    };//end vec2f
};//end math namespace

#endif

