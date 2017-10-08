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

#ifndef  _GL_BSPLINE_2D_H_
#define  _GL_BSPLINE_2D_H_
#pragma once
#include "GLBSpline.h"
#include "GLVertexArray.h"
#include "GLIndexBufferObject.h"
#include "BBox.h"
using namespace davinci;

class GLBSpline2D :
	public GLBSpline
{
public:
	//k1: order of the BSpline basis function of the first variable.
	//k2: order of the BSpline basis function of the second variable.
	GLBSpline2D(int k1, int k2);
	~GLBSpline2D(void);
	virtual void draw(GLfloat lineWidth=1.0);
	virtual void createCurve(){ createMesh();};
	void createMesh();
	vector<vector<float> > getOutputMeshValueRef(){ return m_outputMeshValue;}//<T>
	void setControlPtArrayBivariate(const vector<vector<float> >& ctlPts){ m_controlPtArrayBivariate = ctlPts;}
	int getDimensionX(){ return (int)m_controlPtArrayBivariate[0].size();}
	int getDimensionY(){ return (int)m_controlPtArrayBivariate.size();}
	BBox getMeshBBox(){ return m_bbox;}

private:
	vector<vec3f> getOutputPtArrayRef();
	void createVBO();
private:
	int m_order2;
	vector<float> m_tau2;
	vector<float> m_naturalKnots2;
	vector<vec3f> m_naturalKnotPts2;

	vector<float> m_X1;
	vector<float> m_X2;
	vector<vector<float> > m_outputMeshValue;//<T>
	vector<vector<float> > m_controlPtArrayBivariate;//<T>
	//visualization
	GLVertexArrayObjectRef m_pVAO;
	GLIndexBufferObjectRef  m_pIbo;

	BBox				  m_bbox;

};
#endif

