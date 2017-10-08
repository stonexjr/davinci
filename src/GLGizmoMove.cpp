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

#include "GLGizmoMove.h"
#include "GLError.h"
#include "GLFont.h"
#include "GL/freeglut_std.h"
#include <assert.h>

using namespace davinci;


GLGizmoMove::GLGizmoMove(void)
	:m_axisLength(1.0f),m_selectedAxisId(-1),m_lineWidth(1.0f)
{
	initAxisGeometry();	
}

GLGizmoMove::~GLGizmoMove(void)
{
}

void GLGizmoMove::initAxisGeometry()
{
	vec3f origin(0.0f);
	//X-axis
	m_axisInfo[0].pos    = origin;
	m_axisInfo[0].axisId = 1;
	m_axisInfo[1].pos    = vec3f(m_axisLength,0,0);
	m_axisInfo[1].axisId = 1;
	//Y-axis
	m_axisInfo[2].pos    = origin;
	m_axisInfo[2].axisId = 2;
	m_axisInfo[3].pos    = vec3f(0,m_axisLength,0);
	m_axisInfo[3].axisId = 2;
	//Z-axis
	m_axisInfo[4].pos    = origin;
	m_axisInfo[4].axisId = 3;
	m_axisInfo[5].pos    = vec3f(0, 0, m_axisLength);
	m_axisInfo[5].axisId = 3;
}

void GLGizmoMove::drawAxisLabel()
{
	// draw arrows(actually big square dots)
	/*
	*/
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glLoadTransposeMatrixf(GLClickable::m_proj.get());
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glLoadTransposeMatrixf(GLClickable::m_MVM.get());
	GLError::glCheckError(string(__func__)+"0");

	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	glVertex3f(m_axisLength, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, m_axisLength, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, m_axisLength);
	glEnd();
	glPointSize(1);
	//draw label
	float pos[3] = {m_axisLength, 0.0f, 0.0f};
	float color[4] = {1,1,1,1};
	void* font = GLUT_BITMAP_8_BY_13;
	//draw X,Y,Z label
	GLFont::drawString3D(labels[0].c_str() , pos, color, font);
	pos[0] = 0; pos[1] = m_axisLength;
	GLFont::drawString3D(labels[1].c_str(), pos, color, font);
	pos[1] = 0; pos[2] = m_axisLength;
	GLFont::drawString3D(labels[2].c_str(), pos, color, font);
	GLError::glCheckError(__func__);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	GLError::glCheckError(string(__func__)+"1");
}
void GLGizmoMove::draw()
{
	glPushAttrib(GL_ENABLE_BIT|GL_POLYGON_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(m_lineWidth);
	if(m_colorShader.getProgramId()==0)
	{
		GLError::ErrorMessage(string(__func__)+": color shader not initialized!");
	}
	m_colorShader.UseShaders();
	m_colorShader.SetMatrixUniform("modelViewMat", GLClickable::m_MVM);
	m_colorShader.SetMatrixUniform("projMat", GLClickable::m_proj);
	m_colorShader.SetIntUniform("selectedAxisId", m_selectedAxisId);

	m_vaoColor->draw();

	m_colorShader.ReleaseShader();
	
	drawAxisLabel();

	glPopAttrib();

	drawID();
}

void GLGizmoMove::drawID()
{
	GLClickable::m_fboClickable->bind();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_ENABLE_BIT|GL_POLYGON_BIT);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(m_lineWidth*20);
	if(GLClickable::m_clickableShader.getProgramId()==0)
	{
		GLError::ErrorMessage(string(__func__)+": clickable shader not initialized!\n");
	}
	GLClickable::m_clickableShader.UseShaders();
	m_clickableShader.UseShaders();
	m_clickableShader.SetMatrixUniform("modelViewMat", GLClickable::m_MVM);
	m_clickableShader.SetMatrixUniform("projMat", GLClickable::m_proj);

	m_vaoID->draw();

	GLClickable::m_clickableShader.ReleaseShader();

	glLineWidth(m_lineWidth);
	glPopAttrib();
	GLClickable::m_fboClickable->unbind();
}

void GLGizmoMove::initBufferObject()
{
	if (!m_vbo)
	{
		m_vbo = GLVertexBufferObjectRef(new GLVertexBufferObject());
	}
	m_vbo->upload(sizeof(m_axisInfo), 6, m_axisInfo);

	if (!m_vaoID) {
		m_vaoID = GLVertexArrayObjectRef(new GLVertexArrayObject(GL_LINES));
	}
	if (!m_vaoColor) {
		m_vaoColor = GLVertexArrayObjectRef(new GLVertexArrayObject(GL_LINES));
	}

	int progId = GLClickable::m_clickableShader.getProgramId();
	m_vaoID->addAttribute(progId, "in_Position", GL_FLOAT, 3, sizeof(axisInfo),
						offsetof(axisInfo, pos),    false, m_vbo);
	m_vaoID->addAttribute(progId, "id", GL_INT, 1, sizeof(axisInfo),
						offsetof(axisInfo, axisId), false, m_vbo);
	m_vaoID->enable();

	progId = m_colorShader.getProgramId();
	m_vaoColor->addAttribute(progId, "in_Position", GL_FLOAT, 3, sizeof(axisInfo),
						offsetof(axisInfo, pos),    false, m_vbo);
	m_vaoColor->addAttribute(progId, "id", GL_INT, 1, sizeof(axisInfo),
						offsetof(axisInfo, axisId), false, m_vbo);
	m_vaoColor->enable();

}

void GLGizmoMove::setAxisLength( float val )
{
	if (!m_vbo)
	{
		m_vbo = GLVertexBufferObjectRef(new GLVertexBufferObject());
	}
	initAxisGeometry();
	m_vbo->upload(sizeof(m_axisInfo), 6, m_axisInfo);
}

davinci::vec3f davinci::GLGizmoMove::getSelectedAxis(int relative/*=WORLD*/) const
{
	static const vec3f dirs[3] = {
		vec3f(1,0,0), vec3f(0,1,0), vec3f(0,0,1),
	};
	assert(m_selectedAxisId<3);
	mat4 transformMtx;
	if (relative==LOCAL)
	{
		transformMtx = GLClickable::m_MVM;
	}
	vec3f o_dir = dirs[m_selectedAxisId];
	vec3f w_dir = transformMtx * o_dir;
	w_dir.normalize();
	return w_dir;
}

void davinci::GLGizmoMove::init()
{
	initShader();
	initBufferObject();
}
