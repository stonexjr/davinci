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

#include <GL/glew.h>
#include "GLTextureCubeMap.h"
#include "GLError.h"
#include <sstream>
#include <iostream>

namespace davinci{

GLTextureCubeMap::GLTextureCubeMap(
    int w/*=16*/, int h/*=16*/,//GLenum texUnitId/*=0*/,
    GLint internalformat/*=GL_RGBA8*/, GLint format/*=GL_RGBA*/,
    GLint type/*=GL_UNSIGNED_BYTE*/)
    :GLTextureAbstract(w,h,1,GL_TEXTURE_CUBE_MAP,internalformat,format,type)
{
    //glActiveTexture(GL_TEXTURE0+m_texUnitId);
    /*
    GLint id=0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &id);
    std::cout<<"---GL_TEXTURE_2D----GLTexture"<<id-GL_TEXTURE0<<" activated!-----------\n";
    */
    float aniso = 2.0;
    glGenTextures(1, &m_texId);
    glBindTexture(m_target,m_texId);
        glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
        //glTexImage2D(GL_TEXTURE_2D, 0, m_internalformat, m_width, m_height,
        //            0, m_format, m_type, pixelData);
    glBindTexture(m_target, 0);
    //Deprecated in GL 3.3 Core profile and higher.
    GLError::glCheckError(__func__);
}

GLTextureCubeMap::~GLTextureCubeMap(void)
{
}

void GLTextureCubeMap::resize( int w, int h )
{
    m_width = w;
    m_height = h;
    bindTexture();
    //Reallocate GPU texture memory
    /*
    glTexImage2D(m_target, 0, m_internalformat, m_width, m_height,
                 0, m_format, m_type, NULL);
    */
    unbindTexture();
    GLError::glCheckError(__func__);
}

void GLTextureCubeMap::upload(int faceId, int w, int h, const GLvoid *pixelData )
{
    bindTexture();
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+faceId, 0, 
                     m_internalformat, w, h, 0, m_format, m_type,
                     pixelData);
        GLError::glCheckError(std::string(__func__) + "glTexImage2D() failed!");
    unbindTexture();
}

}//end of namespace lily
