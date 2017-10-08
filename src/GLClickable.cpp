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

#include "GLClickable.h"
using namespace davinci;

GLClickable::GLClickable(void)
    :m_clickedID(-1),m_enableClickable(false)
{
}


GLClickable::~GLClickable(void)
{
}

void GLClickable::initClickableShaders()
{
	m_clickableShader.setShaderName("GLClickable::m_clickableShader");
    m_clickableShader.loadVertexShaderFile(m_vtxShaderFile);
    if (m_geomShaderFile != "")
    {
        m_clickableShader.loadGeomShaderFile(m_geomShaderFile);
    }
    m_clickableShader.loadFragShaderFile(m_fragShaderFile);
    m_clickableShader.CreateShaders();
}

void GLClickable::refresh()
{
    if (m_clickableShader.getProgramId())
    {
        m_clickableShader.refresh();
    }
}

int GLClickable::mouseClicked( int x, int y )
{
    GLint id=-1;
    GLint viewport[4];//(x,y),(w,h)
    glGetIntegerv(GL_VIEWPORT, viewport);

    m_fboClickable->bind();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    //glReadPixels(x, viewport[3] - y, 1,1, GL_RGBA, GL_UNSIGNED_BYTE, &rgba);//method 1.
    glReadPixels(x, viewport[3] - y, 1,1, GL_RED_INTEGER, GL_INT, &id);//method 2.

    //rgba.res[3]=0;
    m_clickedID = id-1;//id 0 is reserved for background.

    m_fboClickable->unbind();

    glReadBuffer(GL_BACK);
    //cout <<"viewport["<<viewport[0]<<","<<viewport[1]<<","<<viewport[2]<<","<<viewport[3]<<"]\n";
    cout <<"Clickable::mouseClicked():mouseposition["<<x<<","<<viewport[3]-y<<"]\n";
    cout << "Clickable::mouseClicked(): id="<<id<<", clicked cell id="<<m_clickedID<<"(base-0 index)."<<endl;

    return m_clickedID;
}

void GLClickable::resize( int w, int h )
{
    //Create FBO for drawID.
    //m_texID = GLTexture2DRef(new GLTexture2d(w, h, GLTextureAbstract::getNextAvailabeTexUnitId()));
	/*
    m_texID = GLTexture2DRef(new GLTexture2d(w, h, GLTextureAbstract::getNextAvailabeTexUnitId()
                                             //,GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE));//method 1.
                                            ,GL_R32I, GL_RED_INTEGER, GL_INT));//method 2.
	*/
    m_texID = GLTexture2DRef(new GLTexture2d(w, h //,GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE));//method 1.
                                            ,GL_R32I, GL_RED_INTEGER, GL_INT));//method 2.
    m_texID->setName("Clickable::m_texID");

    m_texDepth = GLTexture2DRef(new GLTexture2d(w,h,// GLTextureAbstract::getNextAvailabeTexUnitId(),
                                GL_DEPTH_COMPONENT,
                                GL_DEPTH_COMPONENT,GL_FLOAT));

    m_texDepth->setName("Clickable::m_texDepth");

    if(!m_fboClickable)
        m_fboClickable = GLFrameBufferObjectRef(new GLFrameBufferObject());

    if (m_fboClickable)
    {
        m_fboClickable->bind();
        m_fboClickable->attachColorBuffer(0, m_texID);
        m_fboClickable->attachDepthBuffer(m_texDepth);
        m_fboClickable->unbind();
    }
    m_fboClickable->checkFramebufferStatus();
    m_fboClickable->printFramebufferInfo();
}