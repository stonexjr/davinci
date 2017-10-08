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

#ifndef _GL_ERROR_H_
#define  _GL_ERROR_H_
#pragma once
#include <string>
#include <DError.h>

#ifdef ENABLE_QT
#include <QString>
#include <QWidget>
#endif

namespace davinci{
#define PRINT_GL_CAPABILITY(name, val){\
        glGetIntegerv((name), &(val));\
        printf(#name"=%d\n", (val));\
    }
class GLError
{
public:
    GLError(void);
    ~GLError(void);
    static	void glCheckError(const std::string& call);
    static	void glCheckError(const char* call);
    //Clean current GL error status.
    static  void purgePreviousGLError();
    static  void ErrorMessage(const std::string &msg);
#ifdef ENABLE_QT
    static  void ErrorMessage(const QString &msg);
    static  void WarningMessage( const QString &msg, QWidget* parent=NULL );
#endif
};

}
#endif
