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

#include "GLGizmoTransform.h"
#include "GLError.h"
#include "GLFont.h"
#include "GL/freeglut_std.h"

using namespace davinci;

static std::string clickable_vert =
"#version 330 core\n \
in int  id;\n \
in vec3 in_Position;\n \
uniform mat4 modelViewMat;\n \
uniform mat4 projMat;\n \
flat out int IDValue;\n \
void main(void)\n \
{\n \
	gl_Position = projMat * modelViewMat * vec4(in_Position,1);\n \
	IDValue = id;\n \
}\n";

static std::string clickable_frag =
"#version 330 core\n \
flat in int IDValue;\n \
out int out_Color;\n \
void main(void)\n \
{\n \
	out_Color = IDValue;\n \
}\n";

static std::string color_vert =
"#version 330 core\n\
in int  id;\n\
in vec3 in_Position;\n\
uniform mat4 modelViewMat;\n\
uniform mat4 projMat;\n\
uniform int selectedAxisId;\n\
out vec4 v2fColor;\n\
\n\
vec4 getColor(int idVal)\n\
{\n\
	vec4 color;\n\
	vec4 hightlight=vec4(1,1,0,0.5);\n\
	if (idVal==1)\n\
	{//X\n\
		color = vec4(1,0,0,1);\n\
		if (selectedAxisId==idVal-1)\n\
		{\n\
			color = hightlight;\n\
		}\n\
	}else if (idVal==2)\n\
	{//Y\n\
		color = vec4(0,0.5,0,1);\n\
		if (selectedAxisId==idVal-1)\n\
		{\n\
			color = hightlight;\n\
		}\n\
	}else if(idVal==3){//Z\n\
		color = vec4(0,0,1,1);\n\
		if (selectedAxisId==idVal-1)\n\
		{\n\
			color = hightlight;\n\
		}\n\
	}else if(idVal==4){//XY\n\
		color = vec4(1,0,0,0.5);\n\
		if (selectedAxisId==idVal-1)\n\
		{\n\
			color = hightlight;\n\
		}\n\
	}else if(idVal==5){//XZ\n\
		color = vec4(0,1,0,0.5);\n\
		if (selectedAxisId==idVal-1)\n\
		{\n\
			color = hightlight;\n\
		}\n\
	}else if(idVal==6){//YZ\n\
		color = vec4(0,0,1,0.5);\n\
		if (selectedAxisId==idVal-1)\n\
		{\n\
			color = hightlight;\n\
		}\n\
	}else if(idVal==7){//YZ\n\
		color = vec4(0.5,0.5,0.5,0.5);\n\
		if (selectedAxisId==idVal-1)\n\
		{\n\
			color = hightlight;\n\
		}\n\
	}\n\
	return color;\n\
}\n\
void main(void)\n\
{\n\
	gl_Position = projMat * modelViewMat * vec4(in_Position,1);\n\
	v2fColor = getColor(id);\n\
}\n";
static std::string color_frag =
"#version 330 core \n\
in vec4 v2fColor;\n\
out vec4 out_Color;\n\
\n\
void main(void)\n\
{\n\
	out_Color = v2fColor;\n\
}";

GLGizmoTransform::GLGizmoTransform(void)
	:m_selectedAxisId(-1)
{
	labels[0]="X";
	labels[1]="Y";
	labels[2]="Z";
}

GLGizmoTransform::~GLGizmoTransform(void)
{
}

void GLGizmoTransform::initClickableShaders()
{
	m_clickableShader.setShaderName(string(__func__));
	m_clickableShader.setVertexShaderStr(clickable_vert);
	m_clickableShader.setFragShaderStr(clickable_frag);
	m_clickableShader.CreateShaders();
}

void GLGizmoTransform::initColorShader()
{
	m_colorShader.setShaderName("GLGizmoTransform::m_colorShader");
	m_colorShader.setVertexShaderStr(color_vert);
	m_colorShader.setFragShaderStr(color_frag);
	m_colorShader.CreateShaders();
}
void davinci::GLGizmoTransform::initShader()
{
	initClickableShaders();
	initColorShader();
}

int GLGizmoTransform::mouseClicked( int x, int y )
{
	union{
		struct{
			unsigned char res[4];
		};
		struct{
			int ival;
		};
	}rgba;
	GLint viewport[4];//(x,y),(w,h)
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLClickable::m_fboClickable->bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(x, viewport[3] - y, 1,1, GL_RED_INTEGER, GL_INT, &rgba);
	m_clickedID = rgba.ival-1;//id 0 is reserved for background.
	GLClickable::m_fboClickable->unbind();
	glReadBuffer(GL_BACK);
	//cout <<"viewport["<<viewport[0]<<","<<viewport[1]<<","<<viewport[2]<<","<<viewport[3]<<"]\n";
	cout <<"Clickable::mouseClicked():mouseposition["<<x<<","<<viewport[3]-y<<"]\n";
	cout << "Clickable::mouseClicked(): clicked Id="<<m_clickedID<<endl;
	cout << "Clickable::mouseClicked(): clicked rgba.ival="<<rgba.ival<<endl;
	return m_clickedID;
}