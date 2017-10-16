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

#include <GL/glew.h>
#include "GLBSpline.h"
#include <GLError.h>
#include <sstream>

GLBSpline::GLBSpline( int order/*=3*/ )
	:m_order1(order),m_drawNaturalKnotPts(true)
	,m_selKnotPts(-1),m_knotPtsColor1(1,0,0,1)
{

}
//Create num-1 equal-distance intervals between range [l,r].
//Returns num boundary tick of each interval.
vector<float> GLBSpline::linspace(float l, float r, int num)
{
	float interval = (r-l)/float(num-1);
	vector<float> pt(num,0);
	for (int i=0 ; i < num ;i++)
	{
		pt[i] = interval * i;
	}
	return pt;
}

void GLBSpline::createCurve()
{
	int dim = getDimension();//(int)m_controlPtArray.size();
	int k = m_order1;
	//n is the largest index of natural knots
	int n = dim + 1 - k;
	if(n < 1) {
		//dim < k
		std::stringstream ss;
		ss << __func__<<": dim="<<dim<<"k="<<k<<", try to decrease the order of BSpline!";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
		return;
	}

	vector<float> X = linspace(0,1,100);
	m_naturalKnots1  = linspace(0,1,n+1);
	//Add coincidental knots at both boundary of natual_knots.
	m_tau1.resize(m_naturalKnots1.size()+2*(k-1));
	int l = (int)m_tau1.size();
	for (int i = 0 ; i < k-1 ; i++)
	{
		m_tau1[i] = 0;
		m_tau1[l-1-i] = 1;
	}
	std::copy(m_naturalKnots1.begin(), m_naturalKnots1.end(), m_tau1.begin()+k-1);
	
#ifdef _DEBUG
	if (l-k != m_controlPtArray.size())
	{
		std::stringstream ss;
		ss << __func__":m_controlPtArray.size()=dim="
		   <<m_controlPtArray.size()<<" != (l-k)="<<l-k<<"!";
		std::string msg=ss.str();
		GLError::ErrorMessage(msg);
	}
#endif
	m_outputPtArray.resize(X.size());
	vector<float> N(l-1,0);
	for (int i = 0 ; i < X.size(); i++)
	{
		float x = X[i];
		compute_B_splines(m_tau1, k, x, N);

		vec3f pt(0,0,0);
		for (int j=0; j < dim ; j++)
		{
			pt+=N[j]*m_controlPtArray[j];//add the weighted Basis function Ns.
		}
		m_outputPtArray[i] = pt;
	}
	//create points corresponds to the natual_knots.
	m_naturalKnotPts1.resize(m_naturalKnots1.size());
	for (int i = 0 ; i < m_naturalKnots1.size(); i++)
	{
		float x = m_naturalKnots1[i];
		compute_B_splines(m_tau1, k, x, N);

		vec3f pt(0,0,0);
		for (int j=0; j < dim ; j++)
		{
			pt+=N[j]*m_controlPtArray[j];//add the weighted Basis function Ns.
		}
		m_naturalKnotPts1[i] = pt;
	}
}

void GLBSpline::compute_B_splines( const vector<float>& tau, int k,
								   float x, vector<float>& N )
{
	int l = (int)tau.size();
	int m = 0;
	std::fill(N.begin(), N.end(), 0.0f);
	//Determine i, s.t Tau(i)<=x<Tau(i+1).
	for (int i=0; i < l-1 ; i++)
	{
		if (tau[i] <= x && x < tau[i+1])
		{
			m = i; break;
		}
	}

	if (m==0)
	{
		//if x==tau[l], search backwards from right side, s.t. Tau(i)<x<=Tau(i+1)
		for (int i=l-2; i >= 0; i--)
		{
			if (tau[i] < x && x <= tau[i+1])
			{
				m = i; break;
			}
		}
	}

	N[m]=1;
	float alpha=0.0f, beta=0.0f;
	for (int j=1 ; j < k ; j++)
	{// for each degree j(column)
		int i = max(m-j, 0);
		for ( ; i < (l-1)-j ; i++)
		{
			if (tau[i+j] - tau[i] < 1.1e-6)
			{
				alpha = 0.0f;
			}else{
				alpha = (x - tau[i])/(tau[i+j] - tau[i]);
			}

			if (tau[i+j+1] - tau[i+1] < 1.1e-6)
			{
				beta = 0.0f;
			}else{
				beta = (tau[i+j+1] - x )/(tau[i+j+1] - tau[i+1]);
			}
			N[i] = alpha * N[i] + beta * N[i+1];
		}
	}

}

void GLBSpline::draw( GLfloat lineWidth/*=1.0*/ )
{
	GLSpline::draw(lineWidth);
	drawNaturalKnotPts();
}
void GLBSpline::drawNaturalKnotPts(){
	glColor3fv((float*)m_knotPtsColor1);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (int i = 0 ; i < m_naturalKnotPts1.size() ; i++)
	{
		glVertex3fv((float*)m_naturalKnotPts1[i]);
	}
	glEnd();
}
