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

#ifndef _GL_GIZMO_SCALE_H_
#define  _GL_GIZMO_SCALE_H_
#include "GLVertexBufferObject.h"
#include "GLVertexArray.h"
#include "GLGizmoMove.h"

namespace davinci
{
class GLGizmoScale:
	public GLGizmoMove
{
public:
	GLGizmoScale(void);
	~GLGizmoScale(void);
	void  setAxisLength( float val );
	vec3f getSelectedAxis(int relative=WORLD) const ;

	virtual void draw();
	virtual void drawID();
	//virtual void initShader();

	virtual void initBufferObject();
private:
	void initAxisGeometry();
	GLVertexBufferObjectRef m_vboSlab;//interaction gizmo for scaling in xy,yz,xz planes.
	GLVertexArrayObjectRef  m_vaoIdSlab;//
	GLVertexArrayObjectRef  m_vaoColorSlab;//
	float    m_slabOffsetRatio, m_slabWidthRatio;
	axisInfo m_slabVertices[16];//[4*3+3];
};

typedef std::shared_ptr<GLGizmoScale> GLGizmoScaleRef;

};
#endif
