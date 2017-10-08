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

#include "GLTexture3D.h"
#include "GLError.h"
#include <sstream>
#include <iostream>

namespace davinci{

GLTexture3d::GLTexture3d(
    int w/*=16*/, int h/*=16*/, int d/*=16*/,//GLenum texUnitId/*=0*/,
    GLint internalformat/*=GL_RGBA8*/, GLint format/*=GL_RGBA*/,
    GLint type/*=GL_UNSIGNED_BYTE*/,const GLvoid* pixelData/*=NULL*/)
    :GLTextureAbstract(w,h,d,GL_TEXTURE_3D,/*texUnitId,*/internalformat,format,type)
//    :m_bUseAlpha(true),m_texId(0),m_width(w),m_height(h),m_texUnitId(texUnitId)
//    ,m_internalformat(internalformat), m_format(format),m_type(type)
{
    //glActiveTexture(GL_TEXTURE0+m_texUnitId);
    /*
    GLint id=0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &id);
    std::cout<<"---GL_TEXTURE_3D----GLTexture"<<id-GL_TEXTURE0<<" activated!-----------\n";
    */
    glGenTextures(1, &m_texId);
    glBindTexture(GL_TEXTURE_3D,m_texId);
        //bind();
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        //What this does is sets the active texture to GL_MODULATE.
        //The GL_MODULATE attribute allows you to apply effects such as lighting
        //and coloring to your texture. If you do not want lighting and coloring
        //to effect your texture and you would like to display the texture unchanged
        //when coloring is applied replace GL_MODULATE with GL_DECAL.
       // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL
        glTexImage3D(GL_TEXTURE_3D, 0, m_internalformat, m_width, m_height, m_depth,
                     0, m_format, m_type, pixelData);
        //unbind();
    glBindTexture(GL_TEXTURE_3D, 0);
    //glDisable(GL_TEXTURE_3D);
    GLError::glCheckError("GLTexture3d(): failed to generate texture3d.");
}


GLTexture3d::~GLTexture3d(void)
{
}

void GLTexture3d::resize( int w, int h, int d )
{
    //deleteTexture();
    m_width = w;
    m_height = h;
    m_depth = d;

    //glActiveTexture(m_texChannelId);
    //glGenTextures(1, &m_texId);
    bindTexture();
    //Reallocate GPU texture memory
    glTexImage3D(GL_TEXTURE_3D, 0, m_internalformat,
                 m_width, m_height, m_depth,
                 0, m_format, m_type, NULL);
    unbindTexture();
    GLError::glCheckError(__func__);
}

void GLTexture3d::upload( int w, int h, int d, const GLvoid *pixelData )
{
    resize(w, h, d);
    bindTexture();
        //glTexImage3D(GL_TEXTURE_3D, 0, m_internalformat, w, h, d,
        //             0, m_format, m_type, pixelData);
        glTexSubImage3D(GL_TEXTURE_3D, 0, 0,0,0,  w, h, d,
                        m_format, m_type, pixelData);
    unbindTexture();
}

void GLTexture3d::setWrapMode( GLint mode )
{
    glBindTexture(m_target,m_texId);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, mode );
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, mode );
    glTexParameteri(m_target, GL_TEXTURE_WRAP_R, mode );
    glBindTexture(m_target,0);
}
}//end of namespace lily
