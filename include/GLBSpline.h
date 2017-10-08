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

#ifndef _GL_BSPLINE_H_
#define _GL_BSPLINE_H_
#pragma once
#include <GLSpline.h>
#include <vec3f.h>
#include <vec4f.h>
#include <vector>
using namespace davinci;
using namespace std;

class GLBSpline :
	public GLSpline
{
public:
	GLBSpline(int order=3);
	virtual void createCurve();
	virtual void draw(GLfloat lineWidth=1.0);
	//Return the reference to the knot array
	vector<float>& getKnotArray(){ return m_tau1;}
	void setKnotArray(const vector<float>& knots){ m_tau1 = knots;}
	void setOrder(int k){ m_order1 = k;}
	int  getOrder(){ return m_order1;}
	int  getDimension(){ return (int)m_controlPtArray.size();}
	void enableDrawKnotPts(bool val){ m_drawNaturalKnotPts = val;}
	int  getSelKnotPts(){ return m_selKnotPts;}
	void setSelKnotPts(int val){ m_selKnotPts = val;}
	vector<vec3f>& getNaturalKnotPtsRef() { return m_naturalKnotPts1;}
protected:
	//Given a row vector of knots 'tau' and spline order k and x,
	//returns a row vector of spline basis function N[1...l-k][k] evaluated
	//at x,where l=tau.size().
	//k: the order of the basis functions.
	void compute_B_splines(const vector<float>& tau, int k, float x, vector<float>& N);
	void drawNaturalKnotPts();
	//Create num-1 equal-distance intervals between range [l,r].
	//Returns num boundary tick of each interval.
	vector<float> linspace(float l, float r, int num);
protected:
	vector<float> m_tau1;
	vector<float> m_naturalKnots1;
	vector<vec3f> m_naturalKnotPts1;//Points that correspond to the natural knots.
	vec4f m_knotPtsColor1;
	int m_order1;
	int m_selKnotPts;
	bool m_drawNaturalKnotPts;
};

#endif

