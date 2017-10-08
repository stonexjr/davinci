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
//CREATED: 2014-10-30
//UPDATED: 2015-05-23

#ifndef _GLUNIFORM_H_
#define _GLUNIFORM_H_
#include <string>
#include "vec2i.h"
#include "vec3i.h"
#include "vec2f.h"
#include "vec3d.h"
#include "mat4.h"
#include "GLUniformBlockBufferObject.h"
#include "GLShaderStorageBufferObject.h"
#include "GLAtomicCounter.h"

namespace davinci
{
    class GLTextureAbstract;
    class GLShader;
    class GLUniform
    {
        public:
            GLUniform();
            GLUniform(int loc, const char* name);
            virtual void upload()=0;
            virtual GLint getUniformLocation(GLShader &shader, std::string uniformName);
            std::string getName() const { return m_name;}
            void setLocation(int loc){ m_location = loc;}
            //bool isActive() const { return m_bActive; }
            //void setActive(bool v){ m_bActive = v; }
    protected:
            //bool m_bActive;
            int m_location;
            std::string m_name;
    };
    //float
    class GLUniform1f : public GLUniform
    {
        public:
            GLUniform1f(int loc, const char* name, float val);
            virtual void upload();
            void setValue(float v){ m_val = v; }
        private:
            float m_val;
    };

    class GLUniform2f : public GLUniform
    {
        public:
            GLUniform2f(int loc, const char* name, const vec2f& val);
            virtual void upload();
            void setValue(const vec2f &v){ m_val = v; }
        private:
            vec2f m_val;
    };

    class GLUniform3f : public GLUniform
    {
        public:
            GLUniform3f(int loc, const char* name, const vec3f& val);
            virtual void upload();
            void setValue(const vec3f &v){ m_val = v; }
        private:
            vec3f m_val;
    };

    class GLUniform4f : public GLUniform
    {
        public:
            GLUniform4f(int loc, const char* name, const vec4f& val);
            virtual void upload();
            void setValue(const vec4f &v){ m_val = v; }
        private:
            vec4f m_val;
    };
    //double
    class GLUniform1d : public GLUniform
    {
    public:
        GLUniform1d(int loc, const char* name, float val);
        virtual void upload();
            void setValue(double v){ m_val = v; }
    private:
        double m_val;
    };
    /*TODO: will be available once vec4d is ready.
    class GLUniform2d : public GLUniform
    {
    public:
        GLUniform2d(int loc, const char* name, const vec2f& val);
        virtual void upload();
    private:
        vec2d m_val;
    };

    class GLUniform4f : public GLUniform
    {
    public:
        GLUniform4f(int loc, const char* name, const vec4f& val);
        virtual void upload();
    private:
        vec4d m_val;
    };
    */
    class GLUniform3d : public GLUniform
    {
    public:
        GLUniform3d(int loc, const char* name, const vec3d& val);
        virtual void upload();
        void setValue(const vec3d &v){ m_val = v; }
    private:
        vec3d m_val;
    };
    //int
    class GLUniform1i : public GLUniform
    {
    public:
        GLUniform1i(int loc, const char* name, int val);
        virtual void upload();
        void setValue(int v){ m_val = v; }

    protected:
        int m_val;
    };
    class GLUniform2i : public GLUniform
    {
    public:
        GLUniform2i(int loc, const char* name,const vec2i& val);
        virtual void upload();
        void setValue(const vec2i &v){ m_val = v; }

    protected:
        vec2i m_val;
    };
    class GLUniform3i : public GLUniform
    {
    public:
        GLUniform3i(int loc, const char* name,const vec3i& val);
        virtual void upload();
        void setValue(const vec3i &v){ m_val = v; }

    protected:
        vec3i m_val;
    };
    //uint
    class GLUniform1u : public GLUniform
    {
    public:
        GLUniform1u(int loc, const char* name, unsigned int val);
        virtual void upload();
        void setValue(const unsigned int v){ m_val = v; }
    private:
        unsigned int m_val;
    };

    class GLUniformSampler : public GLUniform1i
    {
    public:
        GLUniformSampler(int loc, const char* name,GLTextureAbstract* val);
        virtual void upload();
        virtual void unbind();
        void setValue(GLTextureAbstract* v){ m_pTex = v; }

    protected:
        GLTextureAbstract* m_pTex;
    };
    class GLUniformImage : public GLUniformSampler
    {
    public:
        GLUniformImage(int loc, const char* name,GLTextureAbstract* val);
        virtual void upload();
        virtual void unbind();
    };

    class GLUniformMat4 : public GLUniform
    {
    public:
        //mat2/3/4 is row major while OpenGL use column major.
        //Therefore, by default, the isRowMajor argument is true.
        GLUniformMat4(int loc, const char* name, const mat4& val, bool isRowMajor=true);
        virtual void upload();
        void setValue(const mat4& v, bool isRowMajor = true){ m_val = v; m_isRowMajor = isRowMajor; }
    //private:
        mat4 m_val;
        bool m_isRowMajor;
    };
    class GLUniformMat3 : public GLUniform
    {
    public:
        //mat2/3/4 is row major while OpenGL use column major.
        //Therefore, by default, the isRowMajor argument is true.
        GLUniformMat3(int loc, const char* name, const mat3& val, bool isRowMajor=true);
        virtual void upload();
        void setValue(const mat3& v, bool isRowMajor = true){ m_val = v; m_isRowMajor = isRowMajor; }
    private:
        mat3 m_val;
        bool m_isRowMajor;
    };

    class GLUniformBlock : public GLUniform
    {
    public:
        GLUniformBlock(int loc, const char* name, GLUniformBlockBufferObjectRef ubbo);
        virtual void upload();
        virtual void unbind();
        virtual GLint getUniformLocation(GLShader &shader, std::string uniformName);
        void setValue(const GLUniformBlockBufferObjectRef v){ m_ubbo = v; }

    private:
        GLUniformBlockBufferObjectRef m_ubbo;
        GLint m_programId;
    };

    class GLUniformAtomicCounter: public GLUniform
    {
    public:
        GLUniformAtomicCounter(int loc, const char* name, GLAtomicCounterRef ubbo);
        virtual void upload();
        virtual void unbind();
        virtual GLint getUniformLocation(GLShader &shader, std::string uniformName);
        void setValue(const GLAtomicCounterRef v){ m_uac = v; }
    private:
        GLAtomicCounterRef m_uac;
        GLint m_programId;
    };

    class GLUniformSSBO: public GLUniform
    {
    public:
        GLUniformSSBO(int loc, const char* name, GLShaderStorageBufferObjectRef ssbo);
        virtual void upload();
        virtual void unbind();
        virtual GLint getUniformLocation(GLShader &shader, std::string uniformName);
        void setValue(const GLShaderStorageBufferObjectRef v){ m_ssbo = v; }
    private:
        GLShaderStorageBufferObjectRef m_ssbo;
        GLint m_programId;
    };

}

#endif

