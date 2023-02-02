#ifndef CHUNK_ATLAS_H
#define CHUNK_ATLAS_H

// local includes

/// third party includes
#include <GL/glew.h>
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// STL includes
#include <iostream>
#include <string>

// definitions
#define CHUNK_ATLAS_TILE_PIXELS_U 16
#define CHUNK_ATLAS_TILE_PIXELS_V 16
#define CHUNK_ATLAS_FILEPATH "D:/_projects/rts-engine/resources/images/terrain16.png"

// =============================================================================
// ChunkAtlas Class
// =============================================================================
class ChunkAtlas {
    public:
        ChunkAtlas() {};
        void init();
        GLuint getTextureId() { return textureId; };
        int getNumTilesU() { return numTilesU; };
        int getNumTilesV() { return numTilesV; };
        float getStepU() { return stepU; };
        float getStepV() { return stepV; };

    private:
        int numChannels;
        int numPixelsU;
        int numPixelsV;
        int numTilesU;
        int numTilesV;
        float stepU;
        float stepV;
        GLuint textureId = 0;
};

// =============================================================================
// Initialize
// =============================================================================
void ChunkAtlas::init() {

    GLubyte* pixels = stbi_load(
        CHUNK_ATLAS_FILEPATH,
        &numPixelsU,
        &numPixelsV,
        &numChannels,
        0);

    numTilesU = numPixelsU / CHUNK_ATLAS_TILE_PIXELS_U;
    numTilesV = numPixelsV / CHUNK_ATLAS_TILE_PIXELS_V;
    stepU = 1.0f / float(numTilesU);
    stepV = 1.0f / float(numTilesV);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA32F,
        numPixelsU,
        numPixelsV,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(pixels);
}

#endif // CHUNK_ATLAS_H