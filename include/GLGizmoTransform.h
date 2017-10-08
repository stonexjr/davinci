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

#ifndef _GL_GIZMO_TRANSFROM_H_
#define  _GL_GIZMO_TRANSFROM_H_
#include "GLClickable.h"
#include "GLVertexBufferObject.h"
#include "GLVertexArray.h"

namespace davinci
{
class GLGizmoTransform :
	public GLClickable
{
public:
	GLGizmoTransform(void);
	~GLGizmoTransform(void);
	virtual void initClickableShaders();
	virtual void init()=0;
	virtual void draw() = 0;
	virtual void drawID() = 0;
	virtual void initShader();
	virtual void initBufferObject()=0;
	void initColorShader();
	void setSelectedAxisId( int id ) {m_selectedAxisId = id;}
	int  getSelectedAxisId() const { return m_selectedAxisId;}

	virtual int mouseClicked( int x, int y );

protected:
	virtual void initAxisGeometry(){};

protected:

	std::string labels[3];
	int   m_selectedAxisId;//-1:no selection; 0: X-axis; 1:Y-axis; 2:Z-axis.
	GLVertexBufferObjectRef m_vbo;
	GLVertexArrayObjectRef  m_vaoID;
	GLVertexArrayObjectRef  m_vaoColor;
	GLShader m_colorShader;
};

typedef std::shared_ptr<GLGizmoTransform> GLGizmoTransformRef;

};
#endif
