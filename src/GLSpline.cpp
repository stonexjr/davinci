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
//#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <GL/freeglut.h>
#include "GLSpline.h"
#include "GLError.h"

GLSpline::GLSpline(void)
        :m_resolution(10),m_bDrawCtlPt(false)
        ,m_selCtlPtIdx(-1), m_ctlPtSize(0.05f)
        ,m_lineColor(1,1,1,1),m_ctlPtColor(0,1,1,1)
        ,m_ctlSegColor(1,0,1,1)
{
}

GLSpline::~GLSpline(void)
{
}

void GLSpline::addControlPt(const vec3f& val)
{
    m_controlPtArray.push_back(val);
}

void GLSpline::draw(GLfloat lineWidth/*=1.0*/)
{
    if (m_outputPtArray.size()<2)
        return;
    glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3fv((float*)m_lineColor);
    glLineWidth(lineWidth);
    glBegin(GL_LINE_STRIP);
    for (int i=0 ; i < m_outputPtArray.size() ; i++)
    {
        glVertex3fv((float*)(m_outputPtArray[i]));
    }
    glEnd();
    glPopAttrib();

    if (m_bDrawCtlPt)
    {
        drawControlSegment();
    }
}

void GLSpline::drawControlSegment()
{
    drawControlPt();
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_DEPTH_TEST);
    glColor3fv((float*)m_ctlSegColor);
    std::vector<vec3f>::iterator it = m_controlPtArray.begin();
    glBegin(GL_LINE_STRIP);
    while (it != m_controlPtArray.end())
    {
        vec3f v2 = *it++;
        glVertex3fv((float*)v2);
    }
    glEnd();
    glPopAttrib();
}

void GLSpline::drawControlPt()
{
    glPushAttrib(GL_CURRENT_BIT|GL_LIGHTING_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3fv((float*)m_ctlPtColor);
    for(int i=0 ; i < m_controlPtArray.size() ; i++)
    {
        vec3f& v = m_controlPtArray[i];
        glPushMatrix();
        glTranslatef(v.x(), v.y(), v.z());
        glutSolidCube(m_ctlPtSize);
        glPopMatrix();
    }
    glPopAttrib();
}

int GLSpline::whichCtrPtSelected( const vec3f& mousePos, vec3f& retPt )
{
    for(int i=0; i < m_controlPtArray.size() ; ++i)
    {
        vec3f& v = m_controlPtArray[i];
        if (   mousePos.x() < v.x()+5 && mousePos.x() > v.x()-5
                        && mousePos.y() < v.y()+5 && mousePos.y() > v.y()-5)
        {
            retPt = v;
            return i;
        }
    }
    return -1;
}

void GLSpline::save( const string& fileName, GLuint iomode/*=0*/) 
{
    ofstream ofs(fileName, ios::out | (ios_base::openmode) iomode);
    save(ofs, iomode);
    ofs.close();
}

void GLSpline::save( ofstream& ofs, GLuint iomode/*=0*/)
{
    //first element is the size of output array
    //followed by the data in the output array.
    if (!ofs)
    {
        string msg="GLSpline::save(): ofs does not exists.";
        GLError::ErrorMessage(msg);
    }
    if(iomode == ios::binary)
    {
        GLuint size = (GLuint)m_controlPtArray.size();
        ofs.write(reinterpret_cast<char*>(&size), sizeof(GLuint));
        ofs.write(reinterpret_cast<char*>((float*)(m_controlPtArray.data())),
                  size*sizeof(vec3f));
        size = m_outputPtArray.size();
        ofs.write(reinterpret_cast<char*>(&size), sizeof(GLuint));
        ofs.write(reinterpret_cast<char*>((float*)(m_outputPtArray.data()))
                  , size*sizeof(m_outputPtArray[0]));
        printf("sizeof(m_outputPtArray[0]=%lu, sizeof(vec3f)=%lu\n",sizeof(m_outputPtArray[0]),sizeof(vec3f));


    }else{
        //save in text format.
        //1. Save control points
        ofs << m_controlPtArray.size() <<"\n";
        for (int i = 0 ; i <  m_controlPtArray.size() ; i++)
        {
            ofs << m_controlPtArray[i];
        }
        //2. Save points on curve.
        ofs << m_outputPtArray.size() << "\n";
        for (int i = 0 ; i <  m_outputPtArray.size() ; i++)
        {
            ofs << m_outputPtArray[i];
        }
    }
}

void GLSpline::load( const string& fileName, GLuint iomode/*=0*/)
{
    ifstream ifs(fileName, ios::in | (ios_base::openmode) iomode);
    load(ifs, iomode);
    ifs.close();
}

void GLSpline::load( ifstream& ifs, GLuint iomode/*=0*/ )
{
    if (!ifs)
    {
        string msg=string(__func__)+" ifstream NULL, the file does not exists.";
        GLError::ErrorMessage(msg);
    }
    m_controlPtArray.clear();
    m_outputPtArray.clear();

    size_t size=0;
    if(iomode == ios::binary){
        ifs.read(reinterpret_cast<char*>(&size), sizeof(GLuint));
        m_controlPtArray.resize(size);
        ifs.read(reinterpret_cast<char*>(m_controlPtArray.data()), sizeof(vec3f)*size);

        ifs.read(reinterpret_cast<char*>(&size), sizeof(GLuint));
        m_outputPtArray.resize(size);
        ifs.read(reinterpret_cast<char*>(m_outputPtArray.data()), sizeof(vec3f)*size);

    }else{
        //1. Load control points.
        ifs >> size;
        m_controlPtArray.resize(size);
        for (int i = 0 ; i < size; i++)
        {
            ifs >> m_controlPtArray[i];
        }
        //2. Load points on curve.
        ifs >> size;
        m_outputPtArray.resize(size);
        for (int i = 0 ; i < size; i++)
        {
            ifs >> m_outputPtArray[i];
        }
    }
}
