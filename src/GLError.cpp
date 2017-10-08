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

#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif
#include <string>
#include <iostream>
#include <sstream>
#include "GLError.h"

#ifdef ENABLE_QT
#include <QMessageBox>
#include <QDebug>
#endif

namespace davinci{
#define MAX_ERROR_LENGTH 256

GLError::GLError(void)
{
}

GLError::~GLError(void)
{
}

void GLError::glCheckError(const char* call)
{
    /*
	char enums[][50] =
	{
		"invalid enumeration", // GL_INVALID_ENUM
		"invalid value",       // GL_INVALID_VALUE
		"invalid operation",   // GL_INVALID_OPERATION
		"stack overflow",      // GL_STACK_OVERFLOW
		"stack underflow",     // GL_STACK_UNDERFLOW
		"out of memory",        // GL_OUT_OF_MEMORY
        "invalid framebuffer operation"// GL_INVALID_FRAMEBUFFER_OPERATION
	};

	GLenum errcode = glGetError();
	if (errcode == GL_NO_ERROR)
		return;

	errcode -= GL_INVALID_ENUM;
	QString message;
	message.sprintf("OpenGL %s in '%s'",enums[errcode], call);
    ErrorMessage(message);
    */

	GLenum errcode = glGetError();
	if (errcode == GL_NO_ERROR)
		return;

    std::stringstream message;
    message << "OpenGL "<< gluErrorString(errcode)<<" in '" << call <<"'";
    ErrorMessage(message.str());

}

void GLError::glCheckError( const std::string& call )
{
    glCheckError((const char*)call.data());
}

void GLError::purgePreviousGLError()
{
    glGetError();
}

void GLError::ErrorMessage(const std::string &msg )
{
    std::cerr << msg << std::endl;
#ifdef ENABLE_QT
	QMessageBox::warning(NULL,"Error",QString(msg.data()),QMessageBox::Ok);
#endif // ENABLE_QT

#ifdef DEBUG
    std::vector<int> v;
    v[10]=0;
#else
	exit(1);
#endif
}

#ifdef ENABLE_QT
void GLError::ErrorMessage(const QString &msg )
{
    std::cerr << qPrintable(msg) << std::endl;
    QMessageBox::warning(NULL,"Error",msg,QMessageBox::Ok);
#ifdef DEBUG
    std::vector<int> v;
    v[10]=0;//force it to crash.
#else
    exit(1);
#endif
}
void GLError::WarningMessage( const QString &msg,QWidget* parent )
{
	qDebug()<<msg;
	QMessageBox::warning(parent,"Warning",msg,
		QMessageBox::Ok, QMessageBox::NoButton);
}
#endif

}//end of namespace