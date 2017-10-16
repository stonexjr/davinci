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
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <GL/freeglut.h>

#include "GLFont.h"

namespace freetype_mod {
    void font_data::init(const char * fname, unsigned int h) {
        this->h=h;

        FT_Library library;

        if (FT_Init_FreeType( &library )) 
            throw std::runtime_error("FT_Init_FreeType failed");

        FT_Face face;

        //This is where we load in the font information from the file.
        //Of all the places where the code might die, this is the most likely,
        //as FT_New_Face will die if the font file does not exist or is somehow broken.
        if (FT_New_Face( library, fname, 0, &face )) 
            throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

        //For some twisted reason, Freetype measures font size
        //in terms of 1/64ths of pixels.  Thus, to make a font
        //h pixels high, we need to request a size of h*64.
        //(h << 6 is just a prettier way of writting h*64)
        FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);

        for(int i=0;i<128;i++) chars[i]=new char_data(i,face);

        //We don't need the face information now that the display
        //lists have been created, so we free the assosiated resources.
        FT_Done_Face(face);

        FT_Done_FreeType(library);

    }

    void font_data::clean() {
        for(int i=0;i<128;i++) delete chars[i];
    }

    ///So while glRasterPos won't let us set the raster position using
    ///window coordinates, these hacky functions will let us move the current raster
    ///position a given delta x or y.
    inline void move_raster_x(int x) {
        glBitmap(0,0,0,0,x,0,NULL); }
    inline void move_raster_y(int y) {
        glBitmap(0,0,0,0,0,y,NULL); }


    ///Much like Nehe's glPrint function, but modified to work
    ///with freetype fonts.
    ///For this hack, I've taken out the newline processing, though it's easy to
    ///see how you could use the move_raster() functions to put newline processing back
    ///in.
    void print(const font_data &ft_font, const char *fmt, ...)  {

        //	float h=ft_font.h/.63f;						//We make the height about 1.5* that of


        char		text[256];								// Holds Our String
        va_list		ap;										// Pointer To List Of Arguments

        if (fmt == NULL)									// If There's No Text
            *text=0;											// Do Nothing

        else {
            va_start(ap, fmt);									// Parses The String For Variables
            vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
            va_end(ap);											// Results Are Stored In Text
        }

        glPushAttrib(GL_CURRENT_BIT | GL_PIXEL_MODE_BIT | GL_ENABLE_BIT);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

        //we'll be nice people and save the old pixel unpack alignment-
        //while setting the unpack alignment to one couldn't possibly
        //hurt anyone else's pixel drawing, it might slow it down.
        GLint old_unpack;
        glGetIntegerv(GL_UNPACK_ALIGNMENT,&old_unpack); 
        glPixelStorei(GL_UNPACK_ALIGNMENT ,1);

        float color[4];
        glGetFloatv(GL_CURRENT_COLOR,color);

        glPixelTransferf(GL_RED_SCALE,color[0]);
        glPixelTransferf(GL_GREEN_SCALE,color[1]);
        glPixelTransferf(GL_BLUE_SCALE,color[2]);
        glPixelTransferf(GL_ALPHA_SCALE,color[3]);

        for(int i=0;text[i];i++) {
            const char_data &cdata=*ft_font.chars[text[i]];

            move_raster_x(cdata.left);
            move_raster_y(cdata.move_up);

            glDrawPixels(cdata.w,cdata.h,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,cdata.data);

            move_raster_y(-cdata.move_up);
            move_raster_x(cdata.advance- cdata.left);

        }

        glPixelStorei(GL_UNPACK_ALIGNMENT ,old_unpack);
        glPopAttrib();
    }
}

namespace davinci{

    GLFont::GLFont(void)
    {
    }

    GLFont::~GLFont(void)
    {
    }

    void GLFont::drawString2D( const char *str, int x, int y, float color[4], void *font )
    {
        glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);//lighting and color mask
        glDisable(GL_LIGHTING); //need to disable lighting for proper text color
        glDisable(GL_TEXTURE_2D);
        glColor4fv(color); //set text color
        glRasterPos2i(x, y); //place text position
        //loop all characters in the string
        while (*str)
        {
            glutBitmapCharacter(font, *str);
            ++str;
        }
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glPopAttrib();
    }
    //draw a string in 2D space using true type font.
    void GLFont::drawString2DTTF( const char *str, int x, int y, float color[4], freetype_mod::font_data& font)
    {
        glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);//lighting and color mask
        glDisable(GL_LIGHTING); //need to disable lighting for proper text color
        glDisable(GL_TEXTURE_2D);
        glColor4fv(color); //set text color
        glRasterPos2i(x, y); //place text position
        //flagship paint-text function
        freetype_mod::print(font, str);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glPopAttrib();
    }

    void GLFont::drawString2DTTF( const std::string &str, int x, int y, float color[4], freetype_mod::font_data& font)
    {
        drawString2DTTF(str.data(), x,y,color, font);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // draw a string in 3D space
    ///////////////////////////////////////////////////////////////////////////////
    void drawString3D(const char *str, float pos[3], float color[4], void *font)
    {
        glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
        glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
        glDisable(GL_TEXTURE_2D);
        glColor4fv(color);          // set text color
        glRasterPos3fv(pos);        // place text position

        // loop all characters in the string
        while(*str)
        {
            glutBitmapCharacter(font, *str);
            ++str;
        }
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glPopAttrib();
    }

    void GLFont::drawString3D( const char *str, float pos[3], float color[4], void *font )
    {
        glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);//lighting and color mask
        glDisable(GL_LIGHTING); //need to disable lighting for proper text color

        glColor4fv(color); //set text color
        glRasterPos3fv(pos); //place text position

        //loop all characters in the string
        while (*str)
        {
            glutBitmapCharacter(font, *str);
            ++str;
        }

        glEnable(GL_LIGHTING);
        glPopAttrib();
    }

#ifdef ENABLE_QT
    void GLFont::drawString2DTTF(const QString &qstr, int x, int y, float color[4], freetype_mod::font_data& font)
    {
        QByteArray ba = qstr.toLatin1();
        char *str = ba.data();
        drawString2DTTF(str, x, y, color, font);
    }

    void GLFont::drawString2D( const QString &qstr, int x, int y, float color[4], void *font )
    {
        QByteArray ba = qstr.toLatin1();
        char *str = ba.data();
        drawString2D(str, x, y, color, font);
        /*
        glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);//lighting and color mask
        glDisable(GL_LIGHTING); //need to disable lighting for proper text color
        glDisable(GL_TEXTURE_2D);
        glColor4fv(color); //set text color
        glRasterPos2i(x, y); //place text position

        //loop all characters in the string
        while (*str)
        {
        glutBitmapCharacter(font, *str);
        ++str;
        }
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glPopAttrib();
        */
    }


    void GLFont::drawString3D( const QString &qstr, float pos[3], float color[4], void *font )
    {
        QByteArray ba = qstr.toLatin1();
        char *str = ba.data();
        glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);//lighting and color mask
        glDisable(GL_LIGHTING); //need to disable lighting for proper text color

        glColor4fv(color); //set text color
        glRasterPos3fv(pos); //place text position

        //loop all characters in the string
        while (*str)
        {
            glutBitmapCharacter(font, *str);
            ++str;
        }

        glEnable(GL_LIGHTING);
        glPopAttrib();
    }
#endif

    vec2i GLFont::computeStringDimension( const string& str, const freetype_mod::font_data& ttf_font )
    {
        vec2i lineSize(0, -1);
        for(int i=0; str[i]; i++) {
            if (str[i] > 128)
            {
                cerr << "current text contains none ASCII character!\n";
                exit(1);
            }
            const freetype_mod::char_data &cdata = *(ttf_font.chars[str[i]]);
            lineSize[0] += cdata.w;
            lineSize[1] = max(lineSize[1], cdata.h);
        }
        return lineSize;
    }


}