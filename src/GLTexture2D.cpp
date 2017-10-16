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
#include "GLTexture2D.h"
#include "GLError.h"
#include <sstream>
#include <iostream>

#ifdef ENABLE_QT
#include <QGLWidget>
#include <QFile>
#endif

namespace davinci{

GLTexture2d::GLTexture2d(
    int w/*=16*/, int h/*=16*/,//GLenum texUnitId/*=0*/,
    GLint internalformat/*=GL_RGBA8*/, GLint format/*=GL_RGBA*/,
    GLint type/*=GL_UNSIGNED_BYTE*/, GLint minFilter/*=GL_LINEAR*/,
    GLint magFilter/*=GL_LINEAR*/,
    const GLvoid* pixelData/*=NULL*/)
    :GLTextureAbstract(w,h,1,GL_TEXTURE_2D,/*texUnitId,*/
    internalformat,format,type,minFilter, magFilter)
{
    //glActiveTexture(GL_TEXTURE0+m_texUnitId);
    /*
    GLint id=0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &id);
    std::cout<<"---GL_TEXTURE_2D----GLTexture"<<id-GL_TEXTURE0<<" activated!-----------\n";
    */
    glGenTextures(1, &m_texId);
    glBindTexture(GL_TEXTURE_2D,m_texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //What glTexEnvf() does is sets the active texture to GL_MODULATE. 
        //The GL_MODULATE attribute allows you to apply effects such as lighting
        //and coloring to your texture. If you do not want lighting and coloring
        //to effect your texture and you would like to display the texture unchanged
        //,then replace GL_MODULATE with GL_DECAL. 

        //However, glTexEnvf is deprecated since GL 3.3 Core profile.
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL

        glTexImage2D(GL_TEXTURE_2D, 0, m_internalformat, m_width, m_height,
                     0, m_format, m_type, pixelData);

        //http://www.opengl.org/wiki/Common_Mistakes#gluBuild2DMipmaps
        //at glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR).
        //The magnification filter can't specify the use of mipmaps; only the minification filter can do that.
        if (useMipmap())
        {//for mipmap tutorial, please refer to http://www.g-truc.net/post-0256.html
            glGenerateMipmap(getTarget()); //Allocate the mipmaps
        }

    glBindTexture(GL_TEXTURE_2D, 0);
    //Deprecated in GL 3.3 Core profile and higher.
    GLError::glCheckError(__func__);
}

GLTexture2d::~GLTexture2d(void)
{
}

void GLTexture2d::resize( int w, int h )
{
    m_width = w;
    m_height = h;
    bindTexture();
    //Reallocate GPU texture memory
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalformat, m_width, m_height,
                 0, m_format, m_type, NULL);
    unbindTexture();
    GLError::glCheckError(__func__);
}

#ifdef ENABLE_QT
GLTexture2d::GLTexture2d( QString& fileName,//GLenum texUnitId/*=0*/,
    GLint internalformat/*=GL_RGBA8*/, GLint format/*=GL_RGBA*/,
    GLint type/*=GL_UNSIGNED_BYTE*/)
    :GLTextureAbstract(16,16,1,GL_TEXTURE_2D,/*texUnitId,*/internalformat,format,type)
{
    //glActiveTexture(GL_TEXTURE0+m_texUnitId);
    glGenTextures(1, &m_texId);
    glBindTexture(GL_TEXTURE_2D,m_texId);
    //bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //What this does is sets the active texture to GL_MODULATE. 
    //The GL_MODULATE attribute allows you to apply effects such as lighting
    //and coloring to your texture. If you do not want lighting and coloring
    //to effect your texture and you would like to display the texture unchanged
    //,then replace GL_MODULATE with GL_DECAL. 
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL deprecated in GL core 3.3

    glBindTexture(GL_TEXTURE_2D, 0);
    GLError::glCheckError("GLTexture2d(): Failed on generating texture2d.");

    LoadTexture(fileName);
    glDisable(GL_TEXTURE_2D);
}

void GLTexture2d::LoadTexture( const QString & imgPath )
{
    QFile file(imgPath);
    if (!file.exists())
    {
        GLError::ErrorMessage("image file:"+imgPath+" doesn't exist");
    }
    file.close();
    QImage qimg;
    qimg.load(imgPath);
    //qimg. has Alpha?
    //GLTexture2DRef imgRef = GLTexture2DRef(new GLTexture2D());
    m_image = QGLWidget::convertToGLFormat(qimg);
    m_texId = ConvertImageToTexture(m_image);
    m_width = m_image.width();
    m_height = m_image.height();
    GLError::glCheckError(__func__);
}

GLuint GLTexture2d::ConvertImageToTexture( const QImage &img )
{
    bindTexture();
    m_format =m_bUseAlpha ? GL_RGBA : GL_RGB; 
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, img.width(), img.height(),
                 0,m_format, m_type, img.constBits());

    unbindTexture();
    return m_texId;
}
#endif

void GLTexture2d::upload( int w, int h, const GLvoid *pixelData )
{
    resize(w, h);
    bindTexture();
        //glTexImage2D(GL_TEXTURE_2D, 0, m_internalformat, w, h, 0, m_format,
        //             m_type, pixelData);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, w, h, m_format,m_type, pixelData);
    unbindTexture();
}

void GLTexture2d::generateMipMap(GLint qualityHint/*GL_FASTEST*/)
{
    bindTexture();
    glHint(GL_GENERATE_MIPMAP_HINT, qualityHint);
    GLError::glCheckError(std::string(__func__)+": invalid quality hint!");
    glGenerateMipmap(getTarget());
    GLError::glCheckError(std::string(__func__)+": generate mipmap failed!");
    unbindTexture();
}

void GLTexture2d::bindTexture()
{
    GLTextureAbstract::bindTexture();
    if (useMipmap())
    {
        glTexParameteri(getTarget(), GL_TEXTURE_MIN_FILTER, m_minFilter);
    }
}

void GLTexture2d::setWrapMode( GLint mode )
{
    glBindTexture(m_target,m_texId);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, mode );
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, mode );
    glBindTexture(m_target,0);
}

}//end of namespace lily
