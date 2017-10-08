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

#ifndef _GL_CUBIC_HERMITE_SPLINE_H_
#define _GL_CUBIC_HERMITE_SPLINE_H_
#pragma once
#include "GLSpline.h"

class GLCubicHermiteSpline :
    public GLSpline
{
public:
    GLCubicHermiteSpline(void);
    ~GLCubicHermiteSpline(void);

    virtual void createCurve();
    virtual void draw(GLfloat lineWidth=1.0 );
	void enableDrawCurve(bool val){ m_drawSplineCurve = val;}
    void enableDrawTangentKnot(bool v){ m_drawTangentKnot = v;}
    int  whichTangentKnotSelected( const vec3f& mousePos, vec3f& retPt );
    void setSelTangentKnotIdx(unsigned int val) { m_selTanKnotIdx = val; }
    int  getSelTangentKnotIdx(){ return m_selTanKnotIdx;}
    std::vector<vec3f>& getTangentKnotArrayRef() { return m_tangentKnotArray; }
    void setDone(bool val){ m_done = val;}
    bool getDone() { return m_done ;}
	void clear();
    //Save curve path to designated file.
    //iomode: 0 : text format
    //ios::binary: binary format
    void save(const string& file, GLuint iomode=0) ;
	void save( ofstream& ofs, GLuint iomode/*=0*/);
	//Load curve path from designated file.
    //iomode: 0 : text format
    //ios::binary: binary format
    void load(const string& file, GLuint iomode=0);
	void load( ifstream& ifs, GLuint iomode/*=0*/ );
protected:
    void drawTangentKnot();
    std::vector<vec3f> m_tangentVecArray;
    std::vector<vec3f> m_tangentKnotArray;
    int m_selTanKnotIdx;
    bool m_done;
    bool m_drawTangentKnot;
	bool m_drawSplineCurve;
};

#endif

