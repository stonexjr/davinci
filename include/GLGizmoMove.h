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

#ifndef _GLGIZMOMOVE_H_
#define  _GLGIZMOMOVE_H_
#include "GLVertexBufferObject.h"
#include "GLVertexArray.h"
#include "GLGizmoTransform.h"

namespace davinci
{
class GLGizmoMove :
	public GLGizmoTransform
{
public:
	enum{WORLD, LOCAL};
	GLGizmoMove(void);
	~GLGizmoMove(void);
	void  setLineWidth(float val){ m_lineWidth = val;}
	float getLineWidth()const{ return m_lineWidth;}
	void  setAxisLength(float val);
	void  setSelectedAxisId( int id ) {m_selectedAxisId = id;}
	int   getSelectedAxisId() const { return m_selectedAxisId;}
	vec3f getSelectedAxis(int relative=WORLD) const ;

	virtual void init();
	virtual void draw();
	virtual void drawID();

	virtual void initBufferObject();
protected:
	void drawAxisLabel();
	//virtual void initColorShader(const string& vertShaderFile,
	//						     const string& fragShaderFile);
	void initAxisGeometry();
	struct axisInfo 
	{
		davinci::vec3f pos;
		int			axisId;
	};
	float m_axisLength;
	int   m_selectedAxisId;//-1:no selection; 0: X-axis; 1:Y-axis; 2:Z-axis.
	axisInfo m_axisInfo[6];//data structure to define the three axis.
	/*
	GLVertexBufferObjectRef m_vbo;
	GLVertexArrayObjectRef  m_vaoID;
	GLVertexArrayObjectRef  m_vaoColor;
	GLShader m_colorShader;
	*/
	float  m_lineWidth;
};

typedef std::shared_ptr<GLGizmoMove> GLGizmoMoveRef;

};
#endif