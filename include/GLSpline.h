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

#ifndef _SPLINE_H_
#define  _SPLINE_H_
#pragma once

#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include <vector>
#include <vec3f.h>
#include <vec4f.h>
using namespace davinci;

class GLSpline
{
public:
    GLSpline(void);
    ~GLSpline(void);
    virtual void createCurve()=0;
    //Draw Spline
    //bDrawCS: toggle drawing Control Segments.
    virtual void draw(GLfloat lineWidth=1.0);
    void drawControlPt();
    void drawControlSegment();
    void addControlPt(const vec3f& pt);
    //set resolution of the Spline.
    //resolution is defined as the number of points interploted between
    //two consecutive keyframe.
    void setResolution(int val){ m_resolution = val;}
    int  getResolution() const{ return m_resolution;}
    bool getDrawCtlPt() const {return m_bDrawCtlPt;}
    void enableDrawCtlPt(bool val){m_bDrawCtlPt = val;}
    int  getSelCtlPtIndx() const {return m_selCtlPtIdx;}
    void setSelCtlPtIndx(int val) {m_selCtlPtIdx = val;}
	//The diameter of visualized control point.
    void setCtlPtSize(float v){m_ctlPtSize = v;};
    float getCtlPtSize(){return m_ctlPtSize;}

    //set color
    vec4f getLineColor() const { return m_lineColor; }
    void  setLineColor(const vec4f& val) { m_lineColor = val; }
    vec4f getCtlPtColor() const { return m_ctlPtColor; }
    void  setCtlPtColor(const vec4f& val) { m_ctlPtColor = val; }
    vec4f getCtlSegColor() const { return m_ctlSegColor; }
    void  setCtlSegColor(const vec4f& val) { m_ctlSegColor = val; }

    std::vector<vec3f>& getOutputPtArrayRef() {return m_outputPtArray;}
    //Return the reference to the control point(weights) array.
    std::vector<vec3f>& getControlPtArrayRef() {return m_controlPtArray;}
    //void SetOutputPtArray(std::vector<vec3f>  val) { m_outputPtArray = val;}
    void setControlPtArray(const std::vector<vec3f>& val) { m_controlPtArray = val;}
    //************************************
    // Get selected control point according to current mouse
    // clicked position. if mouse clicked on one of the control
    // points, the method returns true. retPt returns the reference
    // of selected control points, or else method returns false, retPt
    // shall not be used.
    //************************************
    int  whichCtrPtSelected(const vec3f& mousePos, vec3f& retPt);
    //Save curve path to files.
    //iomode: 0 : text format
    //ios::binary: binary format
    void save(const string& fileName, GLuint iomode=0) ;
    void load(const string& fileName, GLuint iomode=0);
protected:
    void save(ofstream& ofs, GLuint iomode=0);
    void load(ifstream& ifs, GLuint iomode=0);

protected:
    int  m_resolution;
    bool m_bDrawCtlPt;
    int  m_selCtlPtIdx;
    float m_ctlPtSize;
    std::vector<vec3f> m_controlPtArray;
    std::vector<vec3f> m_outputPtArray;
    vec4f m_lineColor;
    vec4f m_ctlPtColor;
    vec4f m_ctlSegColor;
};

#endif
