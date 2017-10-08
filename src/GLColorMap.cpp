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

#include "GLColorMap.h"
#include "GLUtilities.h"
#include "GLError.h"
#include <iomanip>
#include <limits>

namespace davinci{

GLColorMap::GLColorMap( int x, int y, int width, int height )
    :m_range(0,0),m_curValue(0),m_ttf_font((TTFont*)NULL),m_above(true)
    ,m_font_size(15),m_curValueStr("")
{
    m_dimension = vec4i(x, y, width, height);
    m_vertShaderFile="Undefined vertex shader of color map";
    m_fragShaderFile="Undefined fragment shader of color map";
}

GLColorMap::~GLColorMap(void)
{
}

void GLColorMap::loadShader()
{
    m_shaderRef = GLShaderRef(new GLShader());
    m_shaderRef->loadVertexShaderFile(m_vertShaderFile);
    m_shaderRef->loadFragShaderFile(m_fragShaderFile);
    m_shaderRef->CreateShaders();
}

void GLColorMap::drawTicks()
{

}

void GLColorMap::updateFont()
{
    ifstream ifs(m_font_path);
    if (!ifs)
    { 
        stringstream ss;
        ss << "Cannot find font:"<<m_font_path<<"\n";
        string msg=ss.str();
        GLError::ErrorMessage(msg);
    }
    if (!m_ttf_font)
    {
        m_ttf_font = TTFontRef(new TTFont());
    }

    m_ttf_font->init(m_font_path.data(), m_font_size);

    computeTitleBBox();

}

void GLColorMap::computeTitleBBox()
{
    vec2i titleSize = GLFont::computeStringDimension(m_title, *m_ttf_font);

    //align to the center of the colormap's bbox in horizontal direction
    //and above the colormap.
    int x_start = m_dimension.x() + (m_dimension.z()>>1) - (titleSize.x()>>1);
    int y_start = m_dimension.y();
    int spacing = 5;
    if (m_above)
    {
        y_start += (m_dimension.w() + spacing);
    }else{
        y_start += (-titleSize.y() - spacing);
    }
    m_titleDimension[0] = x_start;
    m_titleDimension[1] = y_start;
    m_titleDimension[2] = titleSize.x();
    m_titleDimension[3] = titleSize.y();
}

void GLColorMap::resize( int window_width, int window_height )
{
    /*
    m_dimension[0] = (window_width>>1)-m_dimension[2]-50;
    m_dimension[1] = 
    m_dimension = vec4i(x, y, width, height);
    computeTitleBBox();
    */
}

void GLColorMap::drawTitle(int window_width, int window_height)
{
    if (!m_ttf_font)
    {
       updateFont(); 
    }
   
    static float text_color[4]={1,1,1,1};
    GLUtilities::glBegin2DCoords(window_width, window_height);

        GLFont::drawString2DTTF(m_title, m_titleDimension[0], m_titleDimension[1]
                                , text_color, *m_ttf_font);

    GLUtilities::glEnd2DCoords();
    //cout <<"Colormap bbox"<<m_dimension;
    //cout <<"Title bbox"<<m_titleDimension; 
}

void GLColorMap::setCurrentValue( float v )
{
    m_curValue = v;
    stringstream ss;
    //get string representation
    ss << std::setprecision(6) << m_curValue;
    m_curValueStr = ss.str();
    //compute string bbox.
    vec2i textSize = GLFont::computeStringDimension(m_curValueStr, *m_ttf_font);
    int x_start = m_dimension.x() + (m_dimension.z()>>1) - (textSize.x()>>1);
    int y_start = m_dimension.y();
    int spacing = 5;
    if (false)
    {//above color map
        y_start += (m_dimension.w() + spacing);
    }else{//below color map
        y_start += (-textSize.y() - spacing);
    }
    m_curValueDimension[0] = x_start;
    m_curValueDimension[1] = y_start;
    m_curValueDimension[2] = textSize.x();
    m_curValueDimension[3] = textSize.y();
}

void GLColorMap::drawCurValue(int window_width, int window_height)
{
    if( m_range[0] <= m_curValue  && m_curValue <= m_range[1] &&
        abs(m_range[1]-m_range[0]) > std::numeric_limits<float>::epsilon())
    {
        float ratio = (m_curValue - m_range[0])/(m_range[1] - m_range[0]);//>=0
        float colorMapWidth = m_dimension.z();
        float colorMapHeight = m_dimension.w();
        vec2f padXY(2.0f, 2.0f);
        float width = 5.0f;
        float posX = m_dimension.x()+colorMapWidth*ratio;
        vec4f bbox(posX - padXY.x(), m_dimension.y()-padXY.y()
            , width, m_dimension.w()+padXY.y()*2.0f);

        glPushAttrib(GL_ENABLE_BIT|GL_POLYGON_BIT);
        GLUtilities::glBegin2DCoords(window_width, window_height);
        //Draw bounding box.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f(bbox.x(), bbox.y());//(x,y)

        glVertex2f(bbox.x()+bbox.z(), bbox.y());//(x+w,y)

        glVertex2f(bbox.x()+bbox.z(), bbox.y()+bbox.w());//(x+w,y+h);

        glVertex2f(bbox.x(), bbox.y()+bbox.w());//(x,y+h);
        glEnd();

        //value text
        if (!m_curValueStr.empty())
        {
            if (!m_ttf_font)
            {
                updateFont(); 
            }

            static float text_color[4]={1,1,1,1};

            GLFont::drawString2DTTF(m_curValueStr, m_curValueDimension[0], m_curValueDimension[1]
            , text_color, *m_ttf_font);
        }

        GLUtilities::glEnd2DCoords();
        glPopAttrib();
    }
}

void GLColorMap::draw()
{
    if (!m_shaderRef)
    {
       //loadShader();
    }
    if(!m_tex1d)
    {
        GLError::ErrorMessage(std::string("GLColorMap::draw(): color map texture is NULL!"));
    }
    //draw a quad with colormap tex pasted on it.
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);//(x,y),(w,h)

    glPushAttrib(GL_ENABLE_BIT|GL_POLYGON_BIT);
    if (m_shaderRef)
    {
        m_shaderRef->UseShaders();
    }

    m_tex1d->bindTexture(GL_DECAL);

    GLenum texUnit = m_tex1d->getTextureUnitId()+GL_TEXTURE0;
    GLUtilities::glBegin2DCoords(viewport[2], viewport[3]);

    //Forgive me for using fix-pipeline API. By doing so,
    //I have to pay the price of having at most 4 texture units. :(
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
    glBegin(GL_QUADS);
        glMultiTexCoord1f(texUnit,0.0);
        glVertex2f(m_dimension.x(), m_dimension.y());//(x,y)
        
        glMultiTexCoord1f(texUnit,1.0);
        glVertex2f(m_dimension.x()+m_dimension.z(), m_dimension.y());//(x+w,y)

        glMultiTexCoord1f(texUnit,1.0);
        glVertex2f(m_dimension.x()+m_dimension.z(), m_dimension.y()+m_dimension.w());//(x+w,y+h);

        glMultiTexCoord1f(texUnit,0.0);
        glVertex2f(m_dimension.x(), m_dimension.y()+m_dimension.w());//(x,y+h);
    glEnd();
    m_tex1d->unbindTexture();

    //Draw bounding box.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f(m_dimension.x(), m_dimension.y());//(x,y)
        
        glVertex2f(m_dimension.x()+m_dimension.z(), m_dimension.y());//(x+w,y)

        glVertex2f(m_dimension.x()+m_dimension.z(), m_dimension.y()+m_dimension.w());//(x+w,y+h);

        glVertex2f(m_dimension.x(), m_dimension.y()+m_dimension.w());//(x,y+h);
    glEnd();
    GLUtilities::glEnd2DCoords();
    
    if (m_shaderRef)
    {
        m_shaderRef->ReleaseShader();
    }
    //draw ticks
    //draw title.
    
    drawTitle(viewport[2], viewport[3]);
    drawCurValue(viewport[2], viewport[3]);
    //cout << "viewport[2]"<<viewport[2]<<"viewport[3]"<<viewport[3]<<endl;
    GLError::glCheckError("end of colormap draw().");

    glPopAttrib();
}

}
