#ifndef DEBUG_SCREEN_H
#define DEBUG_SCREEN_H

#include "SDL_ttf.h"

#include "types.h"

#include <iostream>

// =============================================================================
// Chunk Class
// =============================================================================
class DebugScreen {
    public:
        DebugScreen();
        void render();
};

// =============================================================================
// Construct Debug Screen
// =============================================================================
DebugScreen::DebugScreen() {

    // setup sdl ttf
    int err = TTF_Init();
    if(err == -1) {
        std::cout << "ERROR: SDL TTF could not initialize." << std::endl
                  << TTF_GetError() << std::endl;
        exit(1);
    }

    // load the font
    TTF_Font* font = TTF_OpenFont("D:/_projects/rts-engine/resources/fonts/MONOFONT.TTF", 32);
    if (font == nullptr) {
        std::cout << "ERROR: SDL TTF could not load font." << std::endl;
        exit(1);
    }

    // setup the pixels from our text
    SDL_Surface* surfaceText = TTF_RenderText_Solid(font, "test test test", {255, 255, 255});

    // setup the texture
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceText->w, surfaceText->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surfaceText->pixels);

    // setup vertex stuff
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-1, -1);
    glTexCoord2f(0, 1); glVertex2f(-1, 1);
    glTexCoord2f(1, 1); glVertex2f(1, 1);
    glTexCoord2f(1, 0); glVertex2f(1, -1);
    glEnd();

    // free the surface
    SDL_FreeSurface(surfaceText);

    // glDeleteTextures(1, &textureId);
    // TTF_CloseFont(font);
}

// =============================================================================
// Render
// =============================================================================
void DebugScreen::render() {

}

#endif // DEBUG_SCREEN_H