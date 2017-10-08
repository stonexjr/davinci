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

#ifndef _GLCOLORMAP_H_
#define _GLCOLORMAP_H_
#include <vector>
#include <cstring>
#include "vec2i.h"
#include "vec4i.h"
#include "GLTexture1D.h"
#include "GLShader.h"
#include "GLFont.h"

namespace davinci{

class GLColorMap
{
public:
    //The color map is draw in the 2D screen space, where
    //the origin is at the center of the screen, the extend to the screen
    //is [-half_screen_width,+half_screen_width]x[-half_screen_height, +half_screen_height]
    GLColorMap(int x, int y, int width, int height);
    ~GLColorMap(void);
    void setTitle(const std::string& title){ m_title = title;}
    std::string getTitle() const { return m_title;}
    void setCurrentValue(float v);
    float getCurrentValue() const { return m_curValue;}
    void setFontRef(const TTFontRef& ttFontRef){ m_ttf_font = ttFontRef;  computeTitleBBox();}
    void setFontPath(const std::string& fontPath){m_font_path = fontPath; m_ttf_font=TTFontRef((TTFont*)NULL);}
    void setFontSize(const int val){m_font_size = val; m_ttf_font = TTFontRef((TTFont*)NULL);}
    void updateFont();

    TTFontRef getFontRef(){ return m_ttf_font;}
    davinci::vec4i getTitleBBox() const { return m_titleDimension;}
    davinci::vec4i getValueBBox()const {return m_curValueDimension;}

    void computeTitleBBox();

    void setMinMaxValue(float minV, float maxV){m_range=vec2f(minV,maxV);}
    void setMinMaxValue(const vec2f range){m_range=range;}

    //param rgba: a array of RGBAs,little endian, with each component in bytes.
    void setColors(const std::vector<unsigned char>& rgba, size_t resolution)
    {
        m_rgba = rgba; m_res = resolution;
    }
    void setTexture1D(const davinci::GLTexture1DRef& tex){ m_tex1d = tex;}
    void setBBox(const davinci::vec4i& dim){ m_dimension = dim; }
    davinci::vec4i getBBox() const { return m_dimension;}

    void update();
    void draw();
    void resize(int window_width, int window_height);
protected:
    void loadShader();
    void drawCurValue(int window_width, int window_height);
    void drawTitle(int window_width, int window_height);
    void drawTicks();

   
private:
    size_t m_res;
    std::vector<unsigned char> m_rgba;
    vec2f m_range;//min and max.
    float m_curValue;
    std::string m_curValueStr;
    davinci::GLTexture1DRef m_tex1d;
    davinci::vec4i m_dimension;//dimension of the color map square(x,y),(w,h).
    davinci::GLShaderRef m_shaderRef;
    std::string m_vertShaderFile;
    std::string m_fragShaderFile;
    std::string m_title;
    davinci::vec4i m_titleDimension;
    davinci::vec4i m_curValueDimension;//dimension of the color map square(x,y),(w,h).
    bool m_above;
    TTFontRef m_ttf_font;
    int m_font_size;
    std::string m_font_path;
};

typedef std::shared_ptr<GLColorMap> GLColorMapRef;

}
#endif
