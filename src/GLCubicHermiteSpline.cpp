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

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <stdlib.h>
#include <fstream>
#include <GL/freeglut.h>
#include "GLCubicHermiteSpline.h"
#include "GLError.h"

GLCubicHermiteSpline::GLCubicHermiteSpline(void)
    :m_done(false),m_drawTangentKnot(true)
	,m_drawSplineCurve(true)
{
}

GLCubicHermiteSpline::~GLCubicHermiteSpline(void)
{
}

void GLCubicHermiteSpline::createCurve()
{
    if (m_controlPtArray.size() < 2)
    {
        return;
    }
    float delta_t = 1.0f/float(m_resolution+1);//(1.0f/m_controlPtArray.size())*0.2f;
    //cout<<1.0f/delta_t<<endl;
    if (!m_done)
    {
        m_outputPtArray.clear();
        m_tangentVecArray.clear();
        m_tangentKnotArray.clear();
        //Initialize default tangents.
        size_t nCtlPt = m_controlPtArray.size();
        vec3f tangent;
        tangent = (m_controlPtArray[1] - m_controlPtArray[0])*0.5f;
        m_tangentVecArray.push_back(tangent);
        m_tangentKnotArray.push_back(m_controlPtArray[0]+tangent);
        for (size_t i = 1 ; i < nCtlPt - 1; i++)
        {
            tangent = (m_controlPtArray[i+1] - m_controlPtArray[i-1])*0.5f;
            m_tangentVecArray.push_back(tangent);
            m_tangentKnotArray.push_back(m_controlPtArray[i] + tangent);
        }
        tangent = (m_controlPtArray[nCtlPt-1] - m_controlPtArray[nCtlPt-2])*0.5f;
        m_tangentVecArray.push_back(tangent);
        m_tangentKnotArray.push_back(m_controlPtArray[nCtlPt-1] + tangent);
        //Interpolate one interval at a time.
        vec3f curPt;
        float h00, h10, h01, h11; 
        for (size_t i = 0 ; i < nCtlPt - 1; i++)
        {
            //for each interval [i,i+1]
            for (float t = 0 ; t <= 1.0f ; t += delta_t)
            {
                h00 = (1.0f+2.0f*t)*(1.0f - t)*(1.0f - t); 
                h10 = t*(1.0f - t)*(1.0f - t);
                h01 = t*t*(3.0f - 2.0f*t);
                h11 = t*t*(t - 1.0f);
                curPt = h00*m_controlPtArray[i]+h10*m_tangentVecArray[i]
                +h01*m_controlPtArray[i+1]+h11*m_tangentVecArray[i+1];
                m_outputPtArray.push_back(curPt);
            }
        }
    }else
    {
        m_outputPtArray.clear();
        m_tangentVecArray.clear();
        //Initialize default tangents.
        size_t nCtlPt = m_controlPtArray.size();
        vec3f tangent;
        for (size_t i = 0 ; i < nCtlPt ; i++)
        {
            tangent = (m_tangentKnotArray[i] - m_controlPtArray[i]);
            m_tangentVecArray.push_back(tangent);
        }
        //Interpolate one interval at a time.
        vec3f curPt;
        float h00, h10, h01, h11; 
        for (size_t i = 0 ; i < nCtlPt - 1; i++)
        {
            //for each interval [i,i+1]
            for (float t = 0 ; t <= 1.0f ; t += delta_t)
            {
                h00 = (1.0f+2.0f*t)*(1.0f - t)*(1.0f - t); 
                h10 = t*(1.0f - t)*(1.0f - t);
                h01 = t*t*(3.0f - 2.0f*t);
                h11 = t*t*(t - 1.0f);
                curPt = h00*m_controlPtArray[i]+h10*m_tangentVecArray[i]
                       +h01*m_controlPtArray[i+1]+h11*m_tangentVecArray[i+1];

                m_outputPtArray.push_back(curPt);
            }
        }
    }
    //cout<<"Total # of interpolated points="<<m_outputPtArray.size()<<endl;
}

void GLCubicHermiteSpline::drawTangentKnot()
{
    glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);

    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 0.0f);//green
    for(int i=0 ; i < m_tangentKnotArray.size() ; i++)
    {
        vec3f& v = m_tangentKnotArray[i];
        glPushMatrix();
        glTranslatef(v.x(), v.y(), v.z());
        glutSolidCube(0.05f);
        glPopMatrix();
    }

    glBegin(GL_LINES);
    for (int i=0 
         ;i < m_controlPtArray.size() && i < m_tangentKnotArray.size()
         ; i ++)
    {
        glVertex3fv((float*)(m_controlPtArray[i]));
        glVertex3fv((float*)(m_tangentKnotArray[i]));
    }
    glEnd();

    glPopAttrib();
}

int GLCubicHermiteSpline::whichTangentKnotSelected( const vec3f& mousePos,
                                                        vec3f& retPt )
{
    for(int i=0; i < m_tangentKnotArray.size() ; ++i)
    {
        vec3f& v = m_tangentKnotArray[i];
        if (mousePos.x() < v.x()+5.0f && mousePos.x() > v.x()-5.0f
            &&mousePos.y() < v.y()+5.0f && mousePos.y() > v.y()-5.0f)
        {
            retPt = v;
            return i;
        }
    }
    return -1;
}

void GLCubicHermiteSpline::draw(GLfloat lineWidth/*=1.0*/)
{
	if (m_drawSplineCurve)
	{
		GLSpline::draw(lineWidth);
	}
    if (m_drawTangentKnot)
    {
        drawTangentKnot();
    }
}

void GLCubicHermiteSpline::save( const string& file, GLuint iomode/*=ios::binary*/)
{
    ofstream ofs(file, ios::out| (ios_base::openmode) iomode);
    save(ofs, iomode);
    ofs.close();
}

void GLCubicHermiteSpline::save( ofstream& ofs, GLuint iomode/*=0*/)
{
    //first element is the size of output array
    //followed by the data in the output array.
    if (!ofs)
    {
        string msg="GLCubicHermiteSpline::save(): ofs does not exists.";
        GLError::ErrorMessage(msg);
    }

    GLSpline::save(ofs, iomode);

    if(iomode == ios::binary)
    {
        GLuint size = (GLuint) m_tangentKnotArray.size();
        ofs.write(reinterpret_cast<char*>(&size), sizeof(GLuint));
        ofs.write(reinterpret_cast<char*>((float*)(m_tangentKnotArray.data()))
                  , size* sizeof(vec3f));

        size = (GLuint) m_tangentVecArray.size();
        ofs.write(reinterpret_cast<char*>(&size), sizeof(GLuint));
        ofs.write(reinterpret_cast<char*>((float*)(m_tangentVecArray.data()))
                  , size* sizeof(vec3f));

    }else
    {	//1. Save tangent knot array
        ofs << m_tangentKnotArray.size() << "\n";
        for (int i = 0 ; i <  m_tangentKnotArray.size() ; i++)
        {
            ofs << m_tangentKnotArray[i];
        }
        //2. Save tangent vector array
        ofs << m_tangentVecArray.size() << "\n";
        for (int i = 0 ; i <  m_tangentVecArray.size() ; i++)
        {
            ofs << m_tangentVecArray[i];
        }
    }
}

void GLCubicHermiteSpline::load( const string& file , GLuint iomode/*=ios::binary*/)
{
    //points on curve and control points array are loaded in super class.
    ifstream ifs(file, ios::in | (ios_base::openmode) iomode);
    load(ifs, iomode);
    ifs.close();
}

void GLCubicHermiteSpline::load( ifstream& ifs, GLuint iomode/*=0*/ )
{
    if (!ifs)
    {
        string msg="GLCubicHermiteSpline::load)(): ifstream NULL, the file does not exists.";
        GLError::ErrorMessage(msg);
    }
    
    GLSpline::load(ifs, (ios_base::openmode)iomode);

    m_tangentKnotArray.clear();
    m_tangentVecArray.clear();
    GLuint size=0;
    if (iomode == ios::binary)
    {
        ifs.read(reinterpret_cast<char*>(&size), sizeof(GLuint));
        m_tangentKnotArray.resize(size);
        ifs.read(reinterpret_cast<char*>(m_tangentKnotArray.data())
                 ,sizeof(vec3f)*size);

        ifs.read(reinterpret_cast<char*>(&size), sizeof(GLuint));
        m_tangentVecArray.resize(size);
        ifs.read(reinterpret_cast<char*>(m_tangentVecArray.data())
                ,sizeof(vec3f)*size);
    } 
    else
    {
        //1. Load control points.
        ifs >> size;
        m_tangentKnotArray.resize(size);
        for (GLuint i = 0 ; i < size; i++)
        {
            ifs >> m_tangentKnotArray[i];
        }
        //2. Load points on curve.
        ifs >> size;
        m_tangentVecArray.resize(size);
        for (GLuint i = 0 ; i < size; i++)
        {
            ifs >> m_tangentVecArray[i];
        }
    }
}

void GLCubicHermiteSpline::clear()
{
    m_controlPtArray.clear();
    m_outputPtArray.clear();
    m_tangentKnotArray.clear();
    m_tangentVecArray.clear();
}
