#ifndef CHUNK_H
#define CHUNK_H

// local includes
#include "types.h"
#include "shader.h"
#include "camera.h"
#include "chunk_atlas.h"

/// third party includes
#include <GL/glew.h>
#include <SDL.h>
#include "FastNoiseLite.h"

// STL includes
#include <iostream>
#include <cstdint>
#include <string>

// definitions
#define TILE_VERTEX_SIZE 4 // xyuv
#define TILE_PIXELS_X 16
#define TILE_PIXELS_Y 16
#define TILE_VERTICIES 6
#define TILE_BUFFER_SIZE (TILE_VERTICIES * TILE_VERTEX_SIZE)

#define CHUNK_TILES_X 32
#define CHUNK_TILES_Y 32
#define CHUNK_PIXELS_X (CHUNK_TILES_X * TILE_PIXELS_X)
#define CHUNK_PIXELS_Y (CHUNK_TILES_Y * TILE_PIXELS_Y)
#define CHUNK_PIXELS_HALF_X (CHUNK_PIXELS_X / 2)
#define CHUNK_PIXELS_HALF_Y (CHUNK_PIXELS_Y / 2)
#define CHUNK_BUFFER_SIZE (TILE_BUFFER_SIZE * CHUNK_TILES_X * CHUNK_TILES_X)
#define CHUNK_VERT_SHADER_FILEPATH "D:/_projects/rts-engine/resources/shaders/chunk_vert.glsl"
#define CHUNK_FRAG_SHADER_FILEPATH "D:/_projects/rts-engine/resources/shaders/chunk_frag.glsl"

// type definitions
typedef std::uint8_t TileArray2D [CHUNK_TILES_Y][CHUNK_TILES_Y];

// =============================================================================
// Chunk Class
// =============================================================================
class Chunk {
    public:
        Chunk();
        ~Chunk();
        void loadData(TileArray2D& tiles);
        void updatePosition(int x, int y);
        void updateTiles();
        void bufferData();
        void render(Camera& camera);
        vec2i_t getPosition() { return pos; };

        bool active = false;

    private:
        static bool isStaticInitialized;
        static Shader shader;
        static ChunkAtlas atlas;

        vec2i_t pos;
        GLuint vaoId = 0;
        GLuint vboId = 0;
        TileArray2D data;
        GLfloat vertexArr[CHUNK_BUFFER_SIZE];
};

// =============================================================================
// Chunk Static Definitions
// =============================================================================
bool Chunk::isStaticInitialized = false;
Shader Chunk::shader = Shader();
ChunkAtlas Chunk::atlas = ChunkAtlas();

// =============================================================================
// Construct Chunk
// =============================================================================
Chunk::Chunk() {

    // reset tile data
    for (int y = 0; y < CHUNK_TILES_X; y++) {
        for (int x = 0; x < CHUNK_TILES_X; x++) {
            data[y][x] = 0;

            // TODO: temporary for drawing chunk border
            if (x == CHUNK_TILES_X - 1 || y == CHUNK_TILES_Y - 1) {
                data[y][x] = 1;
            }
        }
    }

    // setup OpenGL objects
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);

    // static runtime definitions: required becase OpenGL needs to be
    // initialized before these members can be setup
    if (!isStaticInitialized) {

        // setup shader
        shader = Shader(
            std::string(CHUNK_VERT_SHADER_FILEPATH),
            std::string(CHUNK_FRAG_SHADER_FILEPATH)
        );

        // initialize texture atlas
        atlas.init();

        isStaticInitialized = true;
    }
}

// =============================================================================
// Deconstruct Chunk
// =============================================================================
Chunk::~Chunk() {
    glDeleteBuffers(1, &vboId);
    // glDeleteVertexArrays(1, &vaoId); // TODO: understand why this doesnt work
}

// =============================================================================
// Load Data
// =============================================================================
void Chunk::loadData(TileArray2D& tiles) {
    for (int y = 0; y < CHUNK_TILES_X; y++) {
        for (int x = 0; x < CHUNK_TILES_X; x++) {
            data[y][x] = tiles[y][x];
        }
    }
}

// =============================================================================
// Update Chunk Position
// =============================================================================
void Chunk::updatePosition(int x, int y) {

    pos.x = x;
    pos.y = y;

    // calculate chunk position offsets
    int cX = (x * CHUNK_PIXELS_X) - CHUNK_PIXELS_HALF_X;
    int cY = (y * CHUNK_PIXELS_Y) - CHUNK_PIXELS_HALF_Y;

    // loop through each tile
    for (int y = 0; y < CHUNK_TILES_X; y++) {
        for (int x = 0; x < CHUNK_TILES_X; x++) {

            // calculate tile's position offsets
            int offsetX = x * TILE_PIXELS_X + cX;
            int offsetY = y * TILE_PIXELS_Y + cY;

            // calculate tile's vertex vector offsets
            int v = (y * CHUNK_TILES_X + x) * TILE_BUFFER_SIZE;

            // set tile's vertex positions
            vertexArr[v+ 0] = GLfloat(0 * TILE_PIXELS_X + offsetX); // x00;
            vertexArr[v+ 1] = GLfloat(0 * TILE_PIXELS_Y + offsetY); // y00;
            vertexArr[v+ 4] = GLfloat(1 * TILE_PIXELS_X + offsetX); // x10;
            vertexArr[v+ 5] = GLfloat(0 * TILE_PIXELS_Y + offsetY); // y10;
            vertexArr[v+ 8] = GLfloat(1 * TILE_PIXELS_X + offsetX); // x11;
            vertexArr[v+ 9] = GLfloat(1 * TILE_PIXELS_Y + offsetY); // y11;
            vertexArr[v+12] = GLfloat(0 * TILE_PIXELS_X + offsetX); // x00;
            vertexArr[v+13] = GLfloat(0 * TILE_PIXELS_Y + offsetY); // y00;
            vertexArr[v+16] = GLfloat(0 * TILE_PIXELS_X + offsetX); // x01;
            vertexArr[v+17] = GLfloat(1 * TILE_PIXELS_Y + offsetY); // y01;
            vertexArr[v+20] = GLfloat(1 * TILE_PIXELS_X + offsetX); // x11;
            vertexArr[v+21] = GLfloat(1 * TILE_PIXELS_Y + offsetY); // y11;
        }
    }
}

// =============================================================================
// Update Chunk Tile Texture Coordinates
// =============================================================================
void Chunk::updateTiles(){

    int numTilesU = atlas.getNumTilesU();
    float stepU = atlas.getStepU();
    float stepV = atlas.getStepV();

    /// // update noise
    // int seed = 0;
    // FastNoiseLite noise(seed);
    // ///noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    // noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    // loop through each tile
    for (int y = 0; y < CHUNK_TILES_X; y++) {
        for (int x = 0; x < CHUNK_TILES_X; x++){

            // // update noise
            // float nx = x + pos.x * CHUNK_TILES_X;
            // float ny = y + pos.y * CHUNK_TILES_Y;
            // float noiseData = noise.GetNoise(nx, ny);
            // data[y][x] = noiseData > 0.5 ? 1 : 0;

            // calculate tile's texture atlas coordinates
            float offsetU = (data[y][x] % numTilesU) * stepU;
            float offsetV = (data[y][x] / numTilesU) * stepV;

            // calculate tile's vertex vector offsets
            int v = (y * CHUNK_TILES_X + x) * TILE_BUFFER_SIZE;

            vertexArr[v+ 2] = GLfloat(0 * stepU + offsetU); // u00;
            vertexArr[v+ 3] = GLfloat(0 * stepV + offsetV); // v00;
            vertexArr[v+ 6] = GLfloat(1 * stepU + offsetU); // u10;
            vertexArr[v+ 7] = GLfloat(0 * stepV + offsetV); // v10;
            vertexArr[v+10] = GLfloat(1 * stepU + offsetU); // u11;
            vertexArr[v+11] = GLfloat(1 * stepV + offsetV); // v11;
            vertexArr[v+14] = GLfloat(0 * stepU + offsetU); // u00;
            vertexArr[v+15] = GLfloat(0 * stepV + offsetV); // v00;
            vertexArr[v+18] = GLfloat(0 * stepU + offsetU); // u01;
            vertexArr[v+19] = GLfloat(1 * stepV + offsetV); // v01;
            vertexArr[v+22] = GLfloat(1 * stepU + offsetU); // u11;
            vertexArr[v+23] = GLfloat(1 * stepV + offsetV); // v11;
        }
    }
}

// =============================================================================
// Buffer Chunk Render Data
// =============================================================================
void Chunk::bufferData() {

    // bind OpenGL objects
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // send vertex buffer data to GPU
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, TILE_VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, TILE_VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBufferData(GL_ARRAY_BUFFER, CHUNK_BUFFER_SIZE * sizeof(GLfloat), &vertexArr, GL_STATIC_DRAW);

    // unbind OpenGL objects
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

// =============================================================================
// Render Tiles
// =============================================================================
void Chunk::render(Camera& camera) {

    GLuint program = shader.getProgId();
    GLint projLocation = glGetUniformLocation(program, "projection");
    GLint viewLocation = glGetUniformLocation(program, "view");

    // bind OpenGL objects
    glUseProgram(program);
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindTexture(GL_TEXTURE_2D, atlas.getTextureId());

    // render
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &camera.projMat.flat[0]);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &camera.viewMat.flat[0]);
    glDrawArrays(GL_TRIANGLES, 0, CHUNK_BUFFER_SIZE);

    // unbind OpenGL objects (TODO: is this needed?)
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif // CHUNK_H