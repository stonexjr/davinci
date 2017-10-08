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

#ifndef _CLICKABLE_H_
#define  _CLICKABLE_H_
#include <string>
#include "mat4.h"
#include "GLShader.h"
#include "GLTexture2D.h"
#include "GLFrameBufferObject.h"

namespace davinci{
class GLClickable
{
public:
    GLClickable(void);
    ~GLClickable(void);
    virtual void drawID()=0;
    virtual void resize(int w, int h);
    //return the element id under current mouse position in screen space.
    virtual int mouseClicked(int x, int y); 
    void setClickableVtxShader(const std::string& vtxFile){ m_vtxShaderFile = vtxFile ;}
    void setClickableGeomShader(const std::string& geomFile){ m_geomShaderFile = geomFile ;}
    void setClickableFragShader(const std::string& fragFile){ m_fragShaderFile = fragFile ;}
    void initClickableShaders();
    void setModelViewProjectionMtx( const davinci::mat4& modelviewMatrix,const davinci::mat4& projMtx ){m_MVM=modelviewMatrix; m_proj=projMtx;}
    void setModelViewMtx(const davinci::mat4& mvm){m_MVM = mvm;}
    void setProjMtx( const davinci::mat4& projMtx ){m_proj = projMtx;}
    void refresh();
	void enableClickable(bool val){m_enableClickable=val;}
	bool isEnabled()const{return m_enableClickable;}
    davinci::GLFrameBufferObjectRef getFBOIDRef(){ return m_fboClickable;}
    davinci::GLTexture2DRef getIdTexture(){ return m_texID;}
    davinci::GLTexture2DRef getDepthTexture(){ return m_texDepth;}
protected:
	bool m_enableClickable;
    std::string m_vtxShaderFile;
    std::string m_geomShaderFile;
    std::string m_fragShaderFile;
    davinci::GLShader m_clickableShader;
    davinci::mat4 m_MVM;
    davinci::mat4 m_proj;
    //FBO
    davinci::GLFrameBufferObjectRef m_fboClickable;
    davinci::GLTexture2DRef         m_texID;
    davinci::GLTexture2DRef         m_texDepth;
    int m_clickedID;
};
}//end of namespace
#endif
