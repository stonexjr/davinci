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

//AUTHOR: Jinrong Xie (stonexjr at gmail.com)
//CREATED: 2013-10-25
//UPDATED: 2014-01-23

#ifndef _GL_TEXUTRE2D_H_
#define  _GL_TEXUTRE2D_H_

#ifdef ENABLE_QT
#include <QImage>
#endif
#include "GLTextureAbstract.h"

namespace davinci{
	class GLTexture2d : public GLTextureAbstract
	{
		public:
			//Create a 2D texture using predefined pixel data specified by the
			//parameter pixelData. By default it is NULL. if pixelData is NULL
			// the content of the texture is undefined.
			//param internalformat: For a table of legitimate internalformat,
			//please refer to https://www.opengl.org/sdk/docs/man4/xhtml/glTexImage2D.xml
			//Named formats: GL_RGB, GL_RGBA(default), GL_DEPTH_COMPONENT32, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_INTENSITY and GL_ALPHA.
			//You can specify sized representations of these internalformats,
			//e.g. GL_RGBA8(default),GL_RGBA32F_ARB,GL_RGB8,GL_RGBA16F_ARB.
			//This is a hint to the implementation, and indicates the desired color resolution of the texture.
			//For more information please refer to http://www.berkelium.com/OpenGL/GDC99/internalformat.html
			//param format:
			//specify how many color components. eg. GL_RGB,GL_RGBA(default),GL_DEPTH_COMPONENT.
			//Used for performing a Pixel Transfer operation.
			//param type:
			//the data type of each color components. eg. GL_UNSIGNED_BYTE(default) GL_FLOAT
			//Used for performing a Pixel Transfer operation.
			GLTexture2d(int w=16, int h=16, //GLenum texUnitId=0,
						GLint internalformat=GL_RGBA8, GLint format=GL_RGBA,
						GLint type=GL_UNSIGNED_BYTE, GLint minFilter=GL_LINEAR,
						GLint magFilter=GL_LINEAR, const GLvoid* pixelData=NULL);

			~GLTexture2d(void);
#ifdef ENABLE_QT
			//Create a 2D texture by simply load an external image file.
			GLTexture2d(QString& fileName, //GLenum texUnitId=0, 
						GLint internalformat=GL_RGBA8, GLint format=GL_RGBA,
						GLint type=GL_UNSIGNED_BYTE);

			void   LoadTexture( const QString & imgPath );
#endif
			//param mode:
			//What this does is sets the texture mode of active texture to either
			//GL_MODULATE or GL_DECAL.
			//The GL_MODULATE attribute allows you to apply effects such as lighting
			//and coloring to your texture. If you do not want lighting and coloring
			//to effect your texture and you would like to display the texture unchanged
			//when coloring is applied replace GL_MODULATE with GL_DECAL. 
			//void   bind(GLenum mode=GL_MODULATE);
			//void   unbind();
			//resize the texture to a specified resolution in pixel units.
			//The content of the texture is undefined. It's better to initialize
			// the texture buffer by copying content from existing PBO or from
			// an pixelData at Client side(CPU side).
			void   resize(int w, int h);
			//Upload pixelData at Client side to Server side(GPU side).
			//Users are responsible to make sure the w and h and internalformat
			//of pixelData match what they specified in the constructor method.
			void   upload(int w, int h, const GLvoid *pixelData);
			void   bindTexture(GLenum mode=GL_MODULATE);
			//Generate mipmap using specified quality hint
			//hint could be: GL_FASTEST, GL_NICEST, GL_DONT_CARE
			void   generateMipMap(GLint qualityHint=GL_FASTEST);
			//GL_CLAMP, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, or GL_REPEAT
			void   setWrapMode(GLint mode);
#ifdef ENABLE_QT
			QImage& getQImageRef(){ return m_image;}
		private:
			uint ConvertImageToTexture(const QImage &img);
			//resource sharing with cuda.
			QImage  m_image;
#endif
		   
	};
	typedef std::shared_ptr<GLTexture2d> GLTexture2DRef;
}
#endif