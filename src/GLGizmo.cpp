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
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif
#include "GLFont.h"
#include "GL/freeglut.h"
#include "GLGizmo.h"
#include "GLError.h"
#include "BBox.h"
#include "BBox2D.h"
using namespace davinci;

void GLGizmo::glDrawAxis( float length, float lineWidth/*=1.0f*/,
			const char* xLabel,const char* yLabel,const char* zLabel )
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();//modelview

    glPushAttrib(GL_ENABLE_BIT|GL_DEPTH_BITS|GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS);// to avoid visual artifacts with grid lines
    glLineWidth(lineWidth);

    glBegin(GL_LINES);
    glColor3f(1.0f,0.0f,0.0f);//x
    glVertex3f(-length,0.0f,0.0f);
    glVertex3f( length,0.0f,0.0f);
    
    glColor3f(0.0f,1.0f,0.0f);//y
    glVertex3f(0.0f,-length,0.0f);
    glVertex3f(0.0f, length,0.0f);
    
    glColor3f(0.0f,0.0f,1.0f);//z
    glVertex3f(0.0f,0.0f,-length);
    glVertex3f(0.0f,0.0f, length);
    glEnd();

    glLineWidth(lineWidth);
    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex3f(length, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, length, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, length);
    glEnd();
    glPointSize(1);

    float pos[3] = {length, 0.0f, 0.0f};
    float color[4] = {1,1,1,1};
    void* font = GLUT_BITMAP_8_BY_13;
    //draw X,Y,Z label
    GLFont::drawString3D(xLabel, pos, color, font);
    pos[0] = 0; pos[1] = length;
    GLFont::drawString3D(yLabel, pos, color, font);
    pos[1] = 0; pos[2] = length;
    GLFont::drawString3D(zLabel, pos, color, font);
    GLError::glCheckError(__func__);

    glDepthFunc(GL_LEQUAL);
    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void GLGizmo::glDrawGrid( float size/*=10.0f*/, float step/*=1.0f*/ )
{
    // disable lighting
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glPushAttrib(GL_LIGHTING_BIT| GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
    glEnable(GL_LINE_SMOOTH);
    // 20x20 grid
    glBegin(GL_LINES);

    glColor3f(0.5f, 0.5f, 0.5f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size, 0,  i);   // lines parallel to X-axis
        glVertex3f( size, 0,  i);
        glVertex3f(-size, 0, -i);   // lines parallel to X-axis
        glVertex3f( size, 0, -i);

        glVertex3f( i, 0, -size);   // lines parallel to Z-axis
        glVertex3f( i, 0,  size);
        glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(-i, 0,  size);
    }

    // x-axis
    glColor3f(1, 0, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f( size, 0, 0);

    // z-axis
    glColor3f(0,0,1);
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0,  size);

    glEnd();

    // enable lighting back
    glPopAttrib();

    glPopMatrix();
}

void GLGizmo::glDrawBoundingBox( const davinci::BBox& b, const GLenum fillmode)
{
    vec3f bottomLeft = b.pMin;
    vec3f topRight = b.pMax;

    glPushAttrib(GL_POLYGON_BIT|GL_ENABLE_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, fillmode);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    //-Y
    glColor3f(0.0f, 1.0f , 0.0f);
    glVertex3f(bottomLeft.x(), bottomLeft.y(), bottomLeft.z());//000
    glVertex3f(bottomLeft.x(), topRight.y(),   bottomLeft.z());//010

    glVertex3f(topRight.x(), topRight.y(),	 bottomLeft.z());//110
    glVertex3f(topRight.x(), bottomLeft.y(), bottomLeft.z());//100
    //+Y
    glVertex3f(bottomLeft.x(), bottomLeft.y(), topRight.z());//001
    glVertex3f(bottomLeft.x(), topRight.y(),   topRight.z());//011

    glVertex3f(topRight.x(), topRight.y(),	 topRight.z());//111
    glVertex3f(topRight.x(), bottomLeft.y(), topRight.z());//101


    //+Z
    glColor3f(0.0f, 0.0f , 1.0f);	
    glVertex3f(topRight.x(), bottomLeft.y(), topRight.z());//101
    glVertex3f(topRight.x(), bottomLeft.y(), bottomLeft.z());//100
    glVertex3f(topRight.x(), topRight.y(), bottomLeft.z());//110
    glVertex3f(topRight.x(), topRight.y(), topRight.z());//111
    //-Z
    glVertex3f(bottomLeft.x(), bottomLeft.y(), bottomLeft.z());//000
    glVertex3f(bottomLeft.x(), bottomLeft.y(), topRight.z());//001
    glVertex3f(bottomLeft.x(), topRight.y(), topRight.z());//011
    glVertex3f(bottomLeft.x(), topRight.y(), bottomLeft.z());//010

    //+X
    glColor3f(1.0f, 0.0f , 0.0f);
    glVertex3f(bottomLeft.x(), topRight.y(), topRight.z());//011
    glVertex3f(topRight.x(), topRight.y(), topRight.z());//111
    glVertex3f(topRight.x(), topRight.y(), bottomLeft.z());//110
    glVertex3f(bottomLeft.x(), topRight.y(), bottomLeft.z());//010
    //-X
    glVertex3f(bottomLeft.x(), bottomLeft.y(), bottomLeft.z());//000
    glVertex3f(topRight.x(), bottomLeft.y(), bottomLeft.z());//100
    glVertex3f(topRight.x(), bottomLeft.y(), topRight.z());//101
    glVertex3f(bottomLeft.x(), bottomLeft.y(), topRight.z());//001

    glEnd();

    glPopAttrib();
}

void davinci::GLGizmo::glDrawBoundingBox(const davinci::BBox2D& b, const GLenum fillmode/*=GL_LINE*/)
{
	glDrawBoundingBox(davinci::BBox(davinci::vec3f(b.pMin, 0.0f), davinci::vec3f(b.pMax, 0.0f)) );
}

void GLGizmo::glDrawBoundingBox2(const davinci::BBox& b, const unsigned int fillmode, const int type)
{
    vec3f bottomLeft = b.pMin;
    vec3f topRight = b.pMax;
    //glDisable(GL_LIGHTING);
    glPushAttrib(GL_POLYGON_BIT|GL_ENABLE_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, fillmode);
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    if (type==0){
        glColor3f(1.0f,1.0f,1.0f);
        vec3f direction(topRight-bottomLeft);
        vec3f offset(direction*0.3f);
        vec3f s,e;
        glBegin(GL_LINES);
            s = bottomLeft;
            e = s + vec3f(1,0,0)*offset;
            glVertex3fv((float*)s);
            glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,1,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,0,1)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        ////////////////
        glBegin(GL_LINES);
            s = bottomLeft + vec3f(1,0,0)*direction;
            e = s + vec3f(-1,0,0)*offset;
            glVertex3fv((float*)s);
            glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
            e = s + vec3f(0,1,0)*offset;
            glVertex3fv((float*)s);
            glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
            e = s + vec3f(0,0,1)*offset;
            glVertex3fv((float*)s);
            glVertex3fv((float*)e);
        glEnd();
        //////////////////
        glBegin(GL_LINES);
        s = bottomLeft + vec3f(0,1,0)*direction;
        e = s + vec3f(1,0,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,-1,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,0,1)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        //////////////////
        glBegin(GL_LINES);
        s = bottomLeft + vec3f(0,0,1)*direction;
        e = s + vec3f(1,0,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,1,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,0,-1)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        //////////////////
        //////////////////
        glBegin(GL_LINES);
        s = topRight;
        e = s + vec3f(-1,0,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,-1,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,0,-1)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        //////////////////
        glBegin(GL_LINES);
        s = topRight + vec3f(-1,0,0)*direction;;
        e = s + vec3f(1,0,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,-1,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,0,-1)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        //////////////////
        glBegin(GL_LINES);
        s = topRight + vec3f(0,-1,0)*direction;;
        e = s + vec3f(-1,0,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,1,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,0,-1)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        //////////////////
        glBegin(GL_LINES);
        s = topRight + vec3f(0,0,-1)*direction;;
        e = s + vec3f(-1,0,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,-1,0)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
        glBegin(GL_LINES);
        e = s + vec3f(0,0,1)*offset;
        glVertex3fv((float*)s);
        glVertex3fv((float*)e);
        glEnd();
    }else if(type==1){
        glBegin(GL_QUADS);//-z
        glVertex3f(bottomLeft.x(), bottomLeft.y(), bottomLeft.z());//000
        glVertex3f(bottomLeft.x(), topRight.y(), bottomLeft.z());//010
        glVertex3f(topRight.x(), topRight.y(), bottomLeft.z());//110
        glVertex3f(topRight.x(), bottomLeft.y(), bottomLeft.z());//100
        glEnd();

        glBegin(GL_QUADS);//+z
        glVertex3f(topRight.x(), topRight.y(), topRight.z());//111
        glVertex3f(bottomLeft.x(), topRight.y(), topRight.z());//011
        glVertex3f(bottomLeft.x(), bottomLeft.y(), topRight.z());//001
        glVertex3f(topRight.x(), bottomLeft.y(), topRight.z());//101
        glEnd();

        glBegin(GL_QUADS);//+x
        glVertex3f(topRight.x(), bottomLeft.y(), topRight.z());//101
        glVertex3f(topRight.x(), bottomLeft.y(), bottomLeft.z());//100
        glVertex3f(topRight.x(), topRight.y(), bottomLeft.z());//110
        glVertex3f(topRight.x(), topRight.y(), topRight.z());//111
        glEnd();

        glBegin(GL_QUADS);//-x
        glVertex3f(bottomLeft.x(), bottomLeft.y(), bottomLeft.z());//000
        glVertex3f(bottomLeft.x(), bottomLeft.y(), topRight.z());//001
        glVertex3f(bottomLeft.x(), topRight.y(), topRight.z());//011
        glVertex3f(bottomLeft.x(), topRight.y(), bottomLeft.z());//010
        glEnd();

        glBegin(GL_QUADS);//+y
        glVertex3f(bottomLeft.x(), topRight.y(), topRight.z());//011
        glVertex3f(topRight.x(), topRight.y(), topRight.z());//111
        glVertex3f(topRight.x(), topRight.y(), bottomLeft.z());//110
        glVertex3f(bottomLeft.x(), topRight.y(), bottomLeft.z());//010
        glEnd();

        glBegin(GL_QUADS);//-y
        glVertex3f(bottomLeft.x(), bottomLeft.y(), bottomLeft.z());//000
        glVertex3f(topRight.x(), bottomLeft.y(), bottomLeft.z());//100
        glVertex3f(topRight.x(), bottomLeft.y(), topRight.z());//101
        glVertex3f(bottomLeft.x(), bottomLeft.y(), topRight.z());//001
        glEnd();
    }

    glPopAttrib();
}
