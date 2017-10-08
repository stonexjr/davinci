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

//AUTHOR: Jinrong Xie (stonexjr at gmail.com)
//CREATED: 2014-03-03
//UPDATED: 2014-04-30
#ifndef _GL_UTILITIES_H_
#define  _GL_UTILITIES_H_

#ifdef ENABLE_QT
#include <QImage>
#endif // ENABLE_QT
#include <unordered_map>
#include "GLTexture2D.h"
#include "constant.h"
#include "vec2d.h"
#include "mat4.h"
#include "BBox2D.h"
namespace davinci{
	class BBox;
	class vec4i;

	class GLUtilities
	{
		public:
			GLUtilities(void);
			~GLUtilities(void);
		static bool checkGLExtension(std::string extension_string);
		//begin 2D setting, origin of the coordinate system is at the lower left corner
		//of the screen by default.x-axis is pointing to the right , y-axis is pointing upwards.
		static void glBegin2D(int scr_width, int scr_height);
		static void glEnd2D();
		//begin 2D setting, origin of the coordinate system is at the center
		//of the screen by default. x-axis is pointing to the right , y-axis is
		//pointing upwards.
		static void glBegin2DCoords(int scrWidth, int scrHeight);
		static void glEnd2DCoords();

		static void glBegin3D(int scr_width, int scr_height);
		static void glEnd3D();

		static void glDrawQuad2Di(davinci::BBox2D& bbox, bool fill);
		static void glDrawQuad2Di(int xstart, int ystart, int width, int height,bool fill);
		static void glDrawQuad2Df(davinci::BBox2D& bbox,bool fill );
		static void glDrawQuad2Df( float xstart, float ystart, float width, float height,bool  fill );
		static void glDrawQuad2Df( float xstart, float ystart, float width, float height, float angle );
		static void glDrawWedge2Df( float xstart, float ystart, float width, float height, float skew_ratio, bool bFill  );
		static void glDrawQuadBeveled2Df( float xstart, float ystart, float width, float height, float bevel_ratio, bool bFill );
		static void glDrawBackgroundGradient(float w, float h,const float start[4],const float end[4]);
		//b: bounding box you want to draw
		//option: 0: wireframe model
		//        1: filled model
		static void freeResource();
#ifdef ENABLE_QT
		//return a pointer to the sub-block of of current frame buffer.
		//(x,y) is the frame buffer coordinates where x-axis points to the right
		//and y-axis points upwards.
		//fileName: full path name of image file to which the frame buffer is saved.
		//whichBuffer: Indicate which buffer to grab. By default it is GL_BACK
		//If framebuffer object is used, whichBuffer should refer to one of the
		//color attachment, i.e. GL_COLORATTACHMENT_0/1/2....
		static void grabFrameBuffer(int x, int y, int width, int height, 
									const std::string& fileName,
									GLenum whichBuffer=GL_BACK,
									GLenum pixelFormat=GL_BGRA,
									GLenum pixelType=GL_UNSIGNED_BYTE);

		static QImage grabFrameBuffer( int x, int y, int width, int height, 
									GLenum whichBuffer=GL_BACK,
									GLenum pixelFormat=GL_BGRA,
									GLenum pixelType=GL_UNSIGNED_BYTE);
#endif
		static void glGetViewPort(vec4i& viewport);

		//Same as gluUnProject() to get the object coordinates that correspond to those window coordinates
		//srcXY: screen coordinates directly returned by local windows system, where origin is at the top left corner.
		//Return: output parameter, storing the transformed coordinates in object space.
		static vec2d PlaneUnproject(const davinci::vec2d& scrXY);
		static vec3f PlaneUnproject(const davinci::vec3f& scrXYZ, const davinci::mat4 &modelViewMatrix, const davinci::mat4 projMatrix, const int viewport[4]);
		static vec3f unProject(const vec3i& srcXYZ, const mat4&  modelViewMtx, const mat4&  projMtx);

		typedef void (*drawCallBack)(void* params);
		//Apply supersampling to the renderCallBack.
		//qualityLevel: int value ranges from 1(equivalent to no supersampling) 
		//to 8 (highest quality).
		static void glSupersampling(drawCallBack renderCallBack, void* params, int qualityLevel=1);

		static std::unordered_map<GLint, std::string>  m_openglInt2TypeName;
		static std::unordered_map< std::string, GLint> m_openglTypeName2Int;
	};

};
#endif
