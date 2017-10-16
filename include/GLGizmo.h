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

#ifndef _GL_GIZMO_H_
#define  _GL_GIZMO_H_
#pragma once

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

namespace davinci{
class BBox;
class BBox2D;

    class GLGizmo
    {
        public:
        GLGizmo(void);
        ~GLGizmo(void);

        static void glDrawAxis(float length=2, float lineWidth=1.0f,
				const char* xLable="X",const char* yLabel="Y",const char* zLabel="Z");

        static void glDrawGrid(float size=10.0f, float step=1.0f);
        //b: bounding box you want to draw
        //option: 0: wireframe model
        //        1: filled model
        static void glDrawBoundingBox(const davinci::BBox& b, const GLenum fillmode=GL_LINE); 
        static void glDrawBoundingBox(const davinci::BBox2D& b, const GLenum fillmode=GL_LINE); 
        static void glDrawBoundingBox2(const davinci::BBox& b, const unsigned int fillmode,const int option=0);

    };
};
#endif