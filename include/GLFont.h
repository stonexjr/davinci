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

#ifndef _GLFONT_H_
#define _GLFONT_H_

#ifndef ENABLE_TEXT_RENDERING

#error "You need to enable text rendering in cmake to use GLFont"

#else
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
//FreeType Headers
#include <ft2build.h>
#include "vec2i.h"

#include <freetype.h>
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>

namespace freetype_mod {

    //Inside of this namespace, give ourselves the ability
    //to write just "vector" instead of "std::vector"
    using std::vector;

    //Ditto for string.
    using std::string;

    ///This is the data structure that I'm using to store everything I need
    ///to render a character glyph in opengl.  
    struct char_data {
        int w,h;
        int advance;
        int left;
        int move_up;
        unsigned char * data;

        char_data(char ch, FT_Face face) {

            // Load The Glyph For Our Character.
            if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
                throw std::runtime_error("FT_Load_Glyph failed");

            // Move The Face's Glyph Into A Glyph Object.
            FT_Glyph glyph;
            if(FT_Get_Glyph( face->glyph, &glyph ))
                throw std::runtime_error("FT_Get_Glyph failed");

            // Convert The Glyph To A Bitmap.
            FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
            FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

            // This Reference Will Make Accessing The Bitmap Easier.
            FT_Bitmap& bitmap=bitmap_glyph->bitmap;

            advance=face->glyph->advance.x >> 6;
            left=bitmap_glyph->left;
            w=bitmap.width;
            h=bitmap.rows;
            move_up=bitmap_glyph->top-bitmap.rows;


            data = new unsigned char[2*w*h];
            for(int x=0;x<w;x++) for(int y=0;y<h;y++) {
                const int my=h-1-y;
                data[2*(x+w*my)]=255;
                data[2*(x+w*my)+1]=bitmap.buffer[x+w*y];
            }
        }

        ~char_data() { delete [] data; }
    };

    //This holds all of the information related to any
    //freetype font that we want to create.  
    struct font_data {
        char_data * chars[128];

        float h;

        //The init function will create a font of
        //of the height h from the file fname.
        void init(const char * fname, unsigned int h);

        //Free all the resources associated with the font.
        void clean();
    };

    //The flagship function of the library - this thing will print
    //out text at the current raster position using the font ft_font.
    //The current modelview matrix will also be applied to the text. 
    void print(const font_data &ft_font, const char *fmt, ...) ;

}

namespace davinci{

typedef freetype_mod::font_data TTFont;
typedef std::shared_ptr<freetype_mod::font_data> TTFontRef;

class GLFont
{
public:
    GLFont(void);
    ~GLFont(void);
    //////////////////////////////////////////////////////////////////////////
    // write 2d text using GLUT
    // The projection matrix must be set to orthogonal before call this function.
    ///////////////////////////////////////////////////////////////////////////////
    static void drawString2D(const char *str, int x, int y, float color[4], void *font);
    ///////////////////////////////////////////////////////////////////////////////
    // write 2d text using True Type Font(TTF)
    // The projection matrix must be set to orthogonal before call this function.
    ///////////////////////////////////////////////////////////////////////////////
    static void drawString2DTTF( const char *str, int x, int y, float color[4], freetype_mod::font_data& font);
    static void drawString2DTTF( const std::string &str, int x, int y, float color[4], freetype_mod::font_data& font);
    ///////////////////////////////////////////////////////////////////////////////
    // draw a string in 3D space
    ///////////////////////////////////////////////////////////////////////////////
    static void drawString3D(const char *str, float pos[3], float color[4], void *font);
#ifdef ENABLE_QT
    ///////////////////////////////////////////////////////////////////////////////
    // write 2d text using GLUT
    // The projection matrix must be set to orthogonal before call this function.
    ///////////////////////////////////////////////////////////////////////////////
    static void drawString2D(const QString &str, int x, int y, float color[4], void *font);
    ///////////////////////////////////////////////////////////////////////////////
    // write 2d text using True Type Font(TTF)
    // The projection matrix must be set to orthogonal before call this function.
    ///////////////////////////////////////////////////////////////////////////////
    static void drawString2DTTF( const QString &str, int x, int y, float color[4], freetype_mod::font_data& font);
    ///////////////////////////////////////////////////////////////////////////////
    // draw a string in 3D space
    ///////////////////////////////////////////////////////////////////////////////
    static void drawString3D(const QString &str, float pos[3], float color[4], void *font);
#endif //ENABLE_QT
    //compute the width and height in pixel units of a given string based on the specified TTF font.
    static vec2i computeStringDimension(const string& str, const freetype_mod::font_data& ttf_font);
};

}//end of namespace lily

#endif

#endif