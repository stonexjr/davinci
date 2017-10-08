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

#include "GLGizmoScale.h"
#include "GLError.h"
#include "GLFont.h"
#include "GL/freeglut_std.h"
#include <assert.h>

using namespace davinci;


GLGizmoScale::GLGizmoScale(void)
	:GLGizmoMove()
{
	initAxisGeometry();	
	m_slabOffsetRatio = 0.25f;
	m_slabWidthRatio = 0.25f;
}

GLGizmoScale::~GLGizmoScale(void)
{
}

typedef enum{X=1,Y,Z,XY,XZ,YZ,XYZ} AxisId;
void GLGizmoScale::initAxisGeometry()
{
	//init three axis
	GLGizmoMove::initAxisGeometry();
	//init three scaling slab gizmo's and little triangle
	//slab in xy plane
	m_slabVertices[0].pos = vec3f(m_axisLength*m_slabOffsetRatio, 0, 0);
	m_slabVertices[0].axisId = XY;
	m_slabVertices[1].pos = vec3f(m_axisLength*(m_slabOffsetRatio+m_slabWidthRatio), 0, 0);
	m_slabVertices[1].axisId = XY;
	m_slabVertices[2].pos = vec3f(0, m_axisLength*(m_slabOffsetRatio+m_slabWidthRatio), 0);
	m_slabVertices[2].axisId = XY;
	m_slabVertices[3].pos = vec3f(0, m_axisLength*m_slabOffsetRatio, 0);
	m_slabVertices[3].axisId = XY;
	//slab in xz plane
	m_slabVertices[4].pos = vec3f(m_axisLength*m_slabOffsetRatio, 0, 0);
	m_slabVertices[4].axisId = XZ;
	m_slabVertices[5].pos = vec3f(m_axisLength*(m_slabOffsetRatio+m_slabWidthRatio), 0, 0);
	m_slabVertices[5].axisId = XZ;
	m_slabVertices[6].pos = vec3f(0, 0, m_axisLength*(m_slabOffsetRatio+m_slabWidthRatio));
	m_slabVertices[6].axisId = XZ;
	m_slabVertices[7].pos = vec3f(0, 0, m_axisLength*m_slabOffsetRatio);
	m_slabVertices[7].axisId = XZ;
	//slab in YZ plane
	m_slabVertices[8].pos = vec3f(0, 0, m_axisLength*(m_slabOffsetRatio+m_slabWidthRatio));
	m_slabVertices[8].axisId = YZ;
	m_slabVertices[9].pos = vec3f(0, 0, m_axisLength*m_slabOffsetRatio);
	m_slabVertices[9].axisId = YZ;
	m_slabVertices[10].pos = vec3f(0, m_axisLength*m_slabOffsetRatio, 0);
	m_slabVertices[10].axisId = YZ;
	m_slabVertices[11].pos = vec3f(0, m_axisLength*(m_slabOffsetRatio+m_slabWidthRatio), 0);
	m_slabVertices[11].axisId = YZ;
	//Degenerated triangular slab at xyz-plane
	m_slabVertices[12].pos = vec3f(m_axisLength*m_slabOffsetRatio, 0, 0);
	m_slabVertices[12].axisId = XYZ;
	m_slabVertices[13].pos = vec3f(0, m_axisLength*m_slabOffsetRatio, 0);
	m_slabVertices[13].axisId = XYZ;
	m_slabVertices[14].pos = vec3f(0, 0, m_axisLength*m_slabOffsetRatio);
	m_slabVertices[14].axisId = XYZ;
	m_slabVertices[15].pos = vec3f(0, 0, m_axisLength*m_slabOffsetRatio);
	m_slabVertices[15].axisId = XYZ;
}

void GLGizmoScale::draw()
{
	//GLGizmoMove::draw();
	glPushAttrib(GL_ENABLE_BIT|GL_POLYGON_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	//glLineWidth(m_lineWidth);
	if(m_colorShader.getProgramId()==0)
	{
		GLError::ErrorMessage(string(__func__)+": color shader not initialized!");
	}
	m_colorShader.UseShaders();
	m_colorShader.SetMatrixUniform("modelViewMat", GLClickable::m_MVM);
	m_colorShader.SetMatrixUniform("projMat",      GLClickable::m_proj);
	m_colorShader.SetIntUniform("selectedAxisId", m_selectedAxisId);

	m_vaoColor->draw();
	m_vaoColorSlab->draw();

	m_colorShader.ReleaseShader();

	drawAxisLabel();
	glPopAttrib();

	drawID();
}

void GLGizmoScale::drawID()
{
	GLGizmoMove::drawID();

	GLClickable::m_fboClickable->bind();
	glPushAttrib(GL_ENABLE_BIT|GL_POLYGON_BIT);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(m_lineWidth*20.0f);

	GLClickable::m_clickableShader.UseShaders();
	m_clickableShader.UseShaders();

	m_vaoIdSlab->draw();

	GLClickable::m_clickableShader.ReleaseShader();

	glLineWidth(1.0f);
	glPopAttrib();
	GLClickable::m_fboClickable->unbind();
}

void GLGizmoScale::initBufferObject()
{
	GLGizmoMove::initBufferObject();

	if (!m_vboSlab)
	{
		m_vboSlab = GLVertexBufferObjectRef(new GLVertexBufferObject());
	}
	m_vboSlab->upload(sizeof(m_slabVertices),
					  sizeof(m_slabVertices)/sizeof(m_slabVertices[0]),
					  m_slabVertices);

	if (!m_vaoIdSlab) {
		m_vaoIdSlab =
			GLVertexArrayObjectRef(new GLVertexArrayObject(GL_QUADS));
	}
	if (!m_vaoColorSlab) {
		m_vaoColorSlab =
			GLVertexArrayObjectRef(new GLVertexArrayObject(GL_QUADS));
	}

	int progId = GLClickable::m_clickableShader.getProgramId();
	m_vaoIdSlab->addAttribute(progId, "in_Position", GL_FLOAT, 3, sizeof(axisInfo),
						offsetof(axisInfo, pos),    false, m_vboSlab);
	m_vaoIdSlab->addAttribute(progId, "id", GL_INT, 1, sizeof(axisInfo),
						offsetof(axisInfo, axisId), false, m_vboSlab);
	m_vaoIdSlab->enable();

	progId = m_colorShader.getProgramId();
	m_vaoColorSlab->addAttribute(progId, "in_Position", GL_FLOAT, 3, sizeof(axisInfo),
						offsetof(axisInfo, pos),    false, m_vboSlab);
	m_vaoColorSlab->addAttribute(progId, "id", GL_INT, 1, sizeof(axisInfo),
						offsetof(axisInfo, axisId), false, m_vboSlab);
	m_vaoColorSlab->enable();

}

void GLGizmoScale::setAxisLength( float val )
{
	GLGizmoMove::setAxisLength(val);
	if (!m_vboSlab)
	{
		m_vboSlab = GLVertexBufferObjectRef(new GLVertexBufferObject());
	}
	initAxisGeometry();
	m_vboSlab->upload(sizeof(m_slabVertices),
					  sizeof(m_slabVertices)/sizeof(m_slabVertices[0]),
					  m_slabVertices);
}

davinci::vec3f davinci::GLGizmoScale::getSelectedAxis(int relative/*=WORLD*/) const
{
	static const vec3f dirs[7] = {
		vec3f(1,0,0), vec3f(0,1,0), vec3f(0,0,1),
		vec3f(1,1,0), //XY
		vec3f(1,0,1), //XZ
		vec3f(0,1,1), //YZ
		vec3f(1,1,1)//XYZ
	};
	assert(m_selectedAxisId<7);
	mat4 transformMtx;
	if (relative==LOCAL)
	{
		transformMtx = GLClickable::m_MVM;
	}
	vec3f o_dir;
	o_dir = dirs[m_selectedAxisId];
	vec3f w_dir = transformMtx * o_dir;
	w_dir.normalize();
	return w_dir;
}