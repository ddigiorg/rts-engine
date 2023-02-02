#ifndef DEBUG_SCREEN_H
#define DEBUG_SCREEN_H

// local includes
#include "types.h"

// third party includes
// #include "SDL_ttf.h"
#include <ft2build.h>
#include FT_FREETYPE_H

// STL includes
#include <iostream>
#include <algorithm>

// definitions
#define FONT_FILEPATH "D:/_projects/rts-engine/resources/fonts/MONOFONT.TTF"

// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

// =============================================================================
// Chunk Class
// =============================================================================
class DebugScreen {
    public:
        DebugScreen() {};
        void init();
        void render();
    
    private:
        FT_GlyphSlot gs;
        unsigned int w;
        unsigned int h;
        GLuint textureId = 0;

        struct charInfo {
            float ax; // advance.x
            float ay; // advance.y
            float bw; // bitmap.width;
            float bh; // bitmap.rows;
            float bl; // bitmap_left;
            float bt; // bitmap_top;
            float tx; // x offset of glyph in texture coordinates
        } c[128];
};

// =============================================================================
// Initialize
// =============================================================================
void DebugScreen::init() {

    FT_Error err;

    // initialize the freetype library
    FT_Library ft;
    err = FT_Init_FreeType(&ft);
    if (err) {
        std::cout << "ERROR: Could not init freetype library" << std::endl;
        exit(1);
    }

    // load the font
    FT_Face face;
    err = FT_New_Face(ft, FONT_FILEPATH, 0, &face);
    if (err) {
        std::cout << "ERROR: Could not open font" << std::endl;
        exit(1);
    }
    gs = face->glyph;

    // set the font size
    err = FT_Set_Pixel_Sizes(face, 0, 32);

    // load the font characters for the font atlas
    for (int i = 32; i < 128; i++) {
        err = FT_Load_Char(face, i, FT_LOAD_RENDER);
        if(err) {
            std::cout << "WARNING: Loading character " << char(i) << "failed" << std::endl;
            continue;
        }

        w += gs->bitmap.width;
        h = std::max(h, gs->bitmap.rows);
    }

    // setup the texture atlas
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        w,
        h,
        0,
        GL_ALPHA,
        GL_UNSIGNED_BYTE,
        0);

    // load glyph images into the texture atlas
    int x = 0;
    for(int i = 32; i < 128; i++) {
        err = FT_Load_Char(face, i, FT_LOAD_RENDER);
        if(err) {
            continue;
        }

        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            x,
            0,
            gs->bitmap.width,
            gs->bitmap.rows,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            gs->bitmap.buffer);

        x += gs->bitmap.width;

        c[i].ax = gs->advance.x >> 6;
        c[i].ay = gs->advance.y >> 6;
        c[i].bw = gs->bitmap.width;
        c[i].bh = gs->bitmap.rows;
        c[i].bl = gs->bitmap_left;
        c[i].bt = gs->bitmap_top;
        c[i].tx = (float)x / w;
    }
}

// =============================================================================
// Render
// =============================================================================
void DebugScreen::render() {

}

#endif // DEBUG_SCREEN_H