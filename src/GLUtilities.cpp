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
//UPDATED: 2014-08-28

#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif
#include "GL/freeglut.h"
#include "vec4i.h"
#include "BBox.h"
#include "GLUtilities.h"
//#include "GLFont.h"
#include "GLError.h"
#include "GLContext.h"

using namespace davinci;

#define MIN(x, y) ((x) < (y)? (x) : (y))

GLUtilities::GLUtilities(void)
{
}

GLUtilities::~GLUtilities(void)
{
}

void GLUtilities::glBegin2D(int scr_width, int scr_height)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, scr_width, 0.0, scr_height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	return;
}

void GLUtilities::glEnd2D()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);

	return;
}

void GLUtilities::glBegin2DCoords(int scrWidth, int scrHeight)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION); // switch to projection matrix
	glPushMatrix();	//save current projection matrix
	glLoadIdentity(); //reset projection matrix
	gluOrtho2D(0, scrWidth, 0, scrHeight);
	//glOrtho(0.0, scrWidth, 0.0, scrHeight, -1.0, 1.0);
	//backup current model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity(); //reset modelview matrix
	glTranslatef((GLfloat)(scrWidth)*0.5f, (GLfloat)(scrHeight)*0.5f, 0.0);
}

void GLUtilities::glEnd2DCoords()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();//restore modelview matrix

	glMatrixMode(GL_PROJECTION); // switch to projection matrix
	glPopMatrix();//restore projection matrix

	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
}

//Deprecated in core 3.3 and higher
void GLUtilities::glBegin3D( int scrWidth, int scrHeight )
{
	float aspect = float(scrWidth)/float(scrHeight);
	float m_zNear = 0.01f, m_zFar = 100.0f;
	float m_fov = 40.0f;
	float m_near_height = tanf(0.5f*m_fov*DEGREE_TO_RADIAN)*m_zNear;
	//set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION); // switch to projection matrix
	glPushMatrix();	//save current projection matrix
	glLoadIdentity(); //reset projection matrix
	//gluPerspective(m_fov, aspect, m_zNear, m_zFar);//<==>
	glFrustum(-m_near_height*aspect, m_near_height*aspect,\
			  -m_near_height, m_near_height, m_zNear, m_zFar);
	//backup current model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity(); //reset modelview matrix
	glTranslatef(0,0, -2.0f);
	glMatrixMode(GL_MODELVIEW);
}

void GLUtilities::glEnd3D()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();//restore modelview matrix

	glMatrixMode(GL_PROJECTION); // switch to projection matrix
	glPopMatrix();//restore projection matrix

	glMatrixMode(GL_MODELVIEW);
}

void GLUtilities::glDrawQuad2Di(davinci::BBox2D& bbox, bool fill )
{
	davinci::vec2f extend = bbox.getDimension();
	glDrawQuad2Di(bbox.pMin.x(), bbox.pMin.y(), extend.x(), extend.y(), fill);
}

void GLUtilities::glDrawQuad2Di( int xstart, int ystart, int width, int height, bool fill )
{
	if (fill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(xstart, ystart);
	glTexCoord2i(1, 0);
	glVertex2i(xstart+width, ystart);
	glTexCoord2i(1, 1);
	glVertex2i(xstart+width, ystart+height);
	glTexCoord2i(0, 1);
	glVertex2i(xstart, ystart+height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void GLUtilities::glDrawQuad2Df(davinci::BBox2D& bbox, bool fill )
{
	davinci::vec2f extend = bbox.getDimension();
	glDrawQuad2Df(bbox.pMin.x(), bbox.pMin.y(), extend.x(), extend.y(), fill);
}

void GLUtilities::glDrawQuad2Df( float xstart, float ystart, float width, float height,bool fill )
{
	if (fill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(xstart, ystart);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(xstart+width, ystart);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(xstart+width, ystart+height);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(xstart, ystart+height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void GLUtilities::glDrawQuadBeveled2Df( float xstart, float ystart, float width, float height, float bevel_ratio, bool bFill )
{
	float bevel_size = MIN(width,height)*bevel_ratio;
	if (bFill){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_TEXTURE_2D);

		glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(xstart, ystart);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(xstart+width, ystart);
		/*
		glTexCoord2f(1.0f-tex_bevel, 0.0f);
		glVertex2f(xstart+width-bevel_size, ystart);

		glTexCoord2f(1.0f, 0.0f+tex_bevel);
		glVertex2f(xstart+width, ystart+bevel_size);
		*/
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(xstart+width, ystart+height);

		/*
		glTexCoord2f(0.0f+tex_bevel, 1.0f);
		glVertex2f(xstart+bevel_size, ystart+height);

		glTexCoord2f(0.0f, 1.0f-tex_bevel);
		glVertex2f(xstart, ystart+height-bevel_size);
		*/
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(xstart, ystart+height);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}

	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);
	glVertex2f(xstart, ystart);
	glVertex2f(xstart+width-bevel_size, ystart);
	glVertex2f(xstart+width, ystart+bevel_size);
	glVertex2f(xstart+width, ystart+height);
	glVertex2f(xstart+bevel_size, ystart+height);
	glVertex2f(xstart, ystart+height-bevel_size);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void GLUtilities::glDrawWedge2Df( float xstart, float ystart, float width, float height, float skew_ratio, bool bFill )
{
	float skip = MIN(width, height) * skew_ratio;
	GLenum  fill_option = bFill ? GL_FILL : GL_LINE;
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, fill_option);
	glBegin(GL_QUADS);
	if (width > height){
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(xstart, ystart);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(xstart+width, ystart);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(xstart+width, ystart+height);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(xstart+skip, ystart+height);
	}else{
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(xstart, ystart);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(xstart+width, ystart+skip);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(xstart+width, ystart+height);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(xstart, ystart+height);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void GLUtilities::glDrawQuad2Df( float xstart, float ystart, float width, float height, float angle )
{
	float x_skip = height / tanf(angle);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(xstart+x_skip, ystart);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(xstart+width, ystart);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(xstart+width-x_skip, ystart+height);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(xstart, ystart+height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void GLUtilities::glDrawBackgroundGradient(float scrWidth, float scrHeight,
									const float start[4],const float end[4] )
{
	int i;
	int sections = 32; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	GLfloat twoPi   = 2.0f * PI;
	GLfloat centerX = scrWidth*0.5;
	GLfloat centerY = scrHeight*0.5;
	GLfloat radius  = sqrtf(centerX*centerX+centerY*centerY);//0.8f; //radius

	GLUtilities::glBegin2DCoords(scrWidth, scrHeight);
	//Save current enable bit and color, since the black color used later
	//will cause texture mapping in fixed pipe line looks black.
	
	glPushAttrib(GL_ENABLE_BIT|GL_CURRENT_BIT);

	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_TRIANGLE_FAN);
	//draw a big enough circle approximated by triangle fans to cover the
	//entire background.
	glColor4f(start[0],start[1],start[2],start[3]);
	glVertex2f(0, 0);// origin
	glColor4f(end[0],end[1],end[2],end[3]);
	for(i = 0; i <= sections; i++) { // make $section number of circles
		glVertex2f(radius * cos(i * twoPi / sections),
			radius * sin(i * twoPi / sections));
	}
	glEnd();

	glPopAttrib();
	GLUtilities::glEnd2DCoords();
}

#ifdef ENABLE_QT
void GLUtilities::grabFrameBuffer( int x, int y, int width, int height, 
				const std::string& fileName, GLenum whichBuffer/*=GL_BACK*/,
				GLenum pixelFormat/*=GL_BGRA*/, GLenum pixelType/*=GL_UNSIGNED_BYTE*/)
{
	/*
	GLint viewport[4]={0};
	glGetIntegerv(GL_VIEWPORT, viewport);
	width = min(width, viewport[2]);
	height= min(height, viewport[3]);
	*/
	//lily::vec4i subImgBBox(0,0, viewport[2]-1, viewport[3]-1);
	davinci::vec4i subImgBBox(0,0, width-1, height-1);
	int iW = std::abs(subImgBBox.z()-subImgBBox.x()+1);
	int iH = std::abs(subImgBBox.w()-subImgBBox.y()+1); 

	vector<GLuint> img( iW * iH);
	glReadBuffer(whichBuffer);
	GLError::glCheckError(string(__func__)+"glReadBuffer failed.");

	glReadPixels(subImgBBox.x(), subImgBBox.y(), iW, iH,
				 pixelFormat , pixelType, img.data());

	GLError::glCheckError(string(__func__)+": glReadPixels() failed.");

	vector<GLuint> imgData(iW * iH);
	//flip imgData upside down, since y axis in opengGL goes downwards while image file has 
	//y axis goes upwards.
	for (int y=0; y < iH ; y++)
	{
		memcpy(&(imgData[y*iW]), &(img[(iH-1-y)*iW]), sizeof(GLuint)*iW);
	}
	QImage qImage(reinterpret_cast<uchar*>(imgData.data()), iW, iH ,QImage::Format_ARGB32);
	qImage.save(fileName.data(),"png");
}

QImage GLUtilities::grabFrameBuffer( int x, int y, int width, int height, 
				GLenum whichBuffer/*=GL_BACK*/, GLenum pixelFormat/*=GL_BGRA*/,
				GLenum pixelType/*=GL_UNSIGNED_BYTE*/)
{
	davinci::vec4i subImgBBox(0,0, width-1, height-1);
	int iW = std::abs(subImgBBox.z()-subImgBBox.x()+1);
	int iH = std::abs(subImgBBox.w()-subImgBBox.y()+1); 

	vector<GLuint> img( iW * iH);
	glReadBuffer(whichBuffer);
	GLError::glCheckError(string(__func__)+"glReadBuffer failed.");

	glReadPixels(subImgBBox.x(), subImgBBox.y(), iW, iH,
				 pixelFormat , pixelType, img.data());

	GLError::glCheckError(string(__func__)+": glReadPixels() failed.");

	vector<GLuint> imgData(iW * iH);
	//flip imgData upside down, since y axis in opengGL goes downwards while image file has 
	//y axis goes upwards.
	for (int y=0; y < iH ; y++)
	{
		memcpy(&(imgData[y*iW]), &(img[(iH-1-y)*iW]), sizeof(GLuint)*iW);
	}
	QImage qImage(reinterpret_cast<uchar*>(imgData.data()), iW, iH ,QImage::Format_ARGB32);
	return qImage.copy(0,0,iW,iH);
}
#endif

//http://en.wikibooks.org/wiki/OpenGL_Programming/Supersampling
//
void GLUtilities::glSupersampling(drawCallBack renderCallBack, void* param, int qualityLevel/*=1*/)
{
	//Prepare accumulate buffer for oversampling.
	vec4i viewport;
	GLUtilities::glGetViewPort(viewport);
	float w = (float)viewport[2];
	float h = (float)viewport[3];
	GLContext::glMatrixMode(GLContext::GL_ProjectionMatrix);
	GLContext::glPushMatrix();
	mat4 oldPjM = GLContext::g_PjM;
	int nLoop = 1;
	int kernelWidth = qualityLevel;
	//Prepare accumulate buffer for oversampling.
	switch(qualityLevel) 
	{
		case 1: nLoop = 1; break;
		case 2: nLoop = 4; break;
		case 3: nLoop = 9; break;
		case 4: nLoop = 16; break;
		case 5: nLoop = 25; break;
		case 6: nLoop = 36; break;
		case 7: nLoop = 49; break;
		case 8: nLoop = 64; break;
		default:nLoop = 1;
	}
	float weight = 1.0f/float(nLoop);
	for(int i = 0; i < nLoop; i++) {
		davinci::vec3f shift = davinci::vec3f((i % kernelWidth) * 0.5 / w, (i / kernelWidth) * 0.5 / h, 0);
		mat4 aa;
		aa.identity();
		aa.translate(shift);
		davinci::GLContext::g_PjM = aa * oldPjM;
		//=================================================================================
		renderCallBack(param);
		//*********************************************************************************
		glAccum(i ? GL_ACCUM : GL_LOAD, weight);
	}

	glAccum(GL_RETURN, 1);
	GLContext::glPopMatrix();
	GLError::glCheckError(string(__func__)+" End supersmmpling().");
}

void GLUtilities::glGetViewPort( vec4i& viewport )
{ 
	glGetIntegerv(GL_VIEWPORT, &viewport[0]);
}

vec2d GLUtilities::PlaneUnproject(const davinci::vec2d&  scrXY)
{
	GLdouble glMVM[16], glPjM[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, glMVM);
	glGetDoublev(GL_PROJECTION_MATRIX, glPjM);
	glGetIntegerv(GL_VIEWPORT, &viewport[0]);

	GLdouble objXYZ[3]={0};
	gluUnProject(scrXY.x(), viewport[3] - 1 - scrXY.y(), 0,
		glMVM, glPjM, viewport, &objXYZ[0], &objXYZ[1], &objXYZ[2]);

	return davinci::vec2d(objXYZ[0], objXYZ[1]);
}
//https://www.opengl.org/sdk/docs/man2/xhtml/gluUnProject.xml
vec3f GLUtilities::PlaneUnproject(const davinci::vec3f&  scrXYZ, const davinci::mat4 &modelViewMatrix, const davinci::mat4 projMatrix, const int viewport[4])
{
	vec2f coordXY;
	vec2f viewStart(viewport[0], viewport[1]);
	vec2f viewDims(viewport[2], viewport[3]);
	vec2f scrXY(scrXYZ.x(), viewDims.y() - 1.0 - scrXYZ.y());
	coordXY = 2.0 * (scrXY - viewStart) / viewDims - 1.0;
	double Z = 2.0 * scrXYZ.z() - 1.0;
	vec4f objXYZW = (projMatrix*modelViewMatrix).inverse() * vec4f(coordXY, Z, 1.0);
	return objXYZW.xyz();
}

vec3f GLUtilities::unProject(const vec3i& srcXYZ, const mat4&  modelViewMtx, const mat4&  projMtx)
{
	davinci::vec4f viewport;
	glGetFloatv(GL_VIEWPORT, &viewport[0]);

	davinci::vec4f srcPos(srcXYZ.x(),viewport[3]-float(srcXYZ.y()), srcXYZ.z(), 1.0f );
	srcPos[0] = (srcPos[0] - viewport[0])/viewport[2];//normalize x to [0,1]
	srcPos[1] = (srcPos[1] - viewport[1])/viewport[3];//normalize y to [0,1]
	srcPos[0] = srcPos[0]*2.0f - 1.0f;//convert to NDC
	srcPos[1] = srcPos[1]*2.0f - 1.0f;//convert to NDC
	srcPos[2] = srcPos[2]*2.0f - 1.0f;//convert to NDC

	davinci::mat4 modelViewProjectionInverse = projMtx * modelViewMtx;

	modelViewProjectionInverse.inverse();
	davinci::vec4f objPos = modelViewProjectionInverse * srcPos;
	return objPos.xyz();
}

bool davinci::GLUtilities::checkGLExtension(std::string extension_string)
{
	return glewIsSupported(
		extension_string.c_str()
		);
}

std::unordered_map<GLint, std::string> 
GLUtilities::m_openglInt2TypeName({
	{ GL_UNSIGNED_BYTE, "GL_UNSIGNED_BYTE" },
	{ GL_SHORT, "GL_SHORT" }, { GL_DOUBLE, "GL_DOUBLE" }, { GL_UNSIGNED_INT, "GL_UNSIGNED_INT" },
	{ GL_FLOAT, "GL_FLOAT" }, { GL_FLOAT_VEC2, "GL_FLOAT_VEC2" }, { GL_FLOAT_VEC3, "GL_FLOAT_VEC3" }, { GL_FLOAT_VEC4, "GL_FLOAT_VEC4" },
	{ GL_INT, "GL_INT" }, { GL_INT_VEC2, "GL_INT_VEC2" }, { GL_INT_VEC3, "GL_INT_VEC3" }, { GL_INT_VEC4, "GL_INT_VEC4" },
	{ GL_BOOL, "GL_BOOL" }, { GL_BOOL_VEC2, "GL_BOOL_VEC2" }, { GL_BOOL_VEC3, "GL_BOOL_VEC3" }, { GL_BOOL_VEC4, "GL_BOOL_VEC4" },
	{ GL_FLOAT_MAT2, "GL_FLOAT_MAT2" }, { GL_FLOAT_MAT3, "GL_FLOAT_MAT3" }, { GL_FLOAT_MAT4, "GL_FLOAT_MAT4" },
	{ GL_SAMPLER_1D, "GL_SAMPLER_1D" }, { GL_SAMPLER_2D, "GL_SAMPLER_2D" }, { GL_SAMPLER_3D, "GL_SAMPLER_3D" }, { GL_SAMPLER_CUBE, "GL_SAMPLER_CUBE" },
	{ GL_SAMPLER_1D_SHADOW, "GL_SAMPLER_1D_SHADOW" }, { GL_SAMPLER_2D_SHADOW, "GL_SAMPLER_2D_SHADOW" },
	{ GL_UNSIGNED_INT_ATOMIC_COUNTER, "GL_UNSIGNED_INT_ATOMIC_COUNTER" },
	//Geometry Shader Specific
	//input type
	{ GL_POINTS, "GL_POINTS" }, { GL_LINES, "GL_LINES" }, { GL_LINES_ADJACENCY, "GL_LINES_ADJACENCY" },
	{ GL_TRIANGLES, "GL_TRIANGLES" }, { GL_TRIANGLES_ADJACENCY, "GL_TRIANGLES_ADJACENCY" },
	//output type
	{ GL_LINE_STRIP, "GL_LINE_STRIP" }, { GL_TRIANGLE_STRIP, "GL_TRIANGLE_STRIP" },
});

std::unordered_map<std::string, GLint>
GLUtilities::m_openglTypeName2Int({
	{ "GL_UNSIGNED_BYTE", GL_UNSIGNED_BYTE },
	{ "GL_UNSIGNED_SHORT", GL_UNSIGNED_SHORT},
	{ "GL_SHORT",  GL_SHORT},
	{ "GL_UNSIGNED_INT", GL_UNSIGNED_INT },
	{ "GL_INT", GL_INT },
	{ "GL_FLOAT",  GL_FLOAT },
	{ "GL_DOUBLE", GL_DOUBLE},
	});
