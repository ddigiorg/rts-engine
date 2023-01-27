#ifndef CHUNK_H
#define CHUNK_H

#include "shader.h"
#include "camera.h"

#include <GL/glew.h>
#include <SDL.h>

#include <iostream>
#include <cstdint>
#include <string>

#define VERTEX_SIZE 4 // XYUV

#define TILE_PIXELS_X 32
#define TILE_PIXELS_Y 32
#define TILE_VERTICIES 6
#define TILE_BUFFER_SIZE (TILE_VERTICIES * VERTEX_SIZE)

#define CHUNK_TILES_X 32
#define CHUNK_TILES_Y 32
#define CHUNK_PIXELS_X (CHUNK_TILES_X * TILE_PIXELS_X)
#define CHUNK_PIXELS_Y (CHUNK_TILES_Y * TILE_PIXELS_Y)
#define CHUNK_BUFFER_SIZE (TILE_BUFFER_SIZE * CHUNK_TILES_X * CHUNK_TILES_X)

#define ATLAS_COUNT_U 8
#define ATLAS_COUNT_V 8
#define ATLAS_STEP_U (1.0f / float(ATLAS_COUNT_U))
#define ATLAS_STEP_V (1.0f / float(ATLAS_COUNT_V))

typedef std::uint8_t TileArray2D [CHUNK_TILES_Y][CHUNK_TILES_Y];

// =============================================================================
// Chunk Class
// =============================================================================
class Chunk {
    public:
        Chunk();
        //~Chunk();
        void loadData(TileArray2D& tiles);
        void updatePosition(int x, int y);
        void updateTiles();
        void bufferData();
        void render(Camera& camera);
    
    public:
        int x;
        int y;
        GLuint vaoId = 0;
        GLuint vboId = 0;
        Shader shader;

        TileArray2D data;
        GLfloat vertexArr[CHUNK_BUFFER_SIZE];
};

// =============================================================================
// Construct Chunk
// =============================================================================
Chunk::Chunk() {

    // reset tile data
    for (int y = 0; y < CHUNK_TILES_X; y++) {
        for (int x = 0; x < CHUNK_TILES_X; x++) {
            data[y][x] = 0;
        }
    }

    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);

    // setup shaders
    shader.init(
        std::string("D:/_projects/rts-engine/resources/shaders/chunk_vert.glsl"),
        std::string("D:/_projects/rts-engine/resources/shaders/chunk_frag.glsl")
    );
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
    this->x = x;
    this->y = y;

    // calculate chunk position offsets
    int cX = (x * CHUNK_PIXELS_X) - (CHUNK_PIXELS_X / 2);
    int cY = (y * CHUNK_PIXELS_Y) - (CHUNK_PIXELS_Y / 2);

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

    // loop through each tile
    for (int y = 0; y < CHUNK_TILES_X; y++) {
        for (int x = 0; x < CHUNK_TILES_X; x++){

            // calculate tile's texture atlas coordinates
            float offsetU = (data[y][x] % ATLAS_COUNT_U) * ATLAS_STEP_U;
            float offsetV = (data[y][x] / ATLAS_COUNT_V) * ATLAS_STEP_V;

            // calculate tile's vertex vector offsets
            int v = (y * CHUNK_TILES_X + x) * TILE_BUFFER_SIZE;

            vertexArr[v+ 2] = GLfloat(0 * ATLAS_STEP_U + offsetU); // u00;
            vertexArr[v+ 3] = GLfloat(0 * ATLAS_STEP_V + offsetV); // v00;
            vertexArr[v+ 6] = GLfloat(1 * ATLAS_STEP_U + offsetU); // u10;
            vertexArr[v+ 7] = GLfloat(0 * ATLAS_STEP_V + offsetV); // v10;
            vertexArr[v+10] = GLfloat(1 * ATLAS_STEP_U + offsetU); // u11;
            vertexArr[v+11] = GLfloat(1 * ATLAS_STEP_V + offsetV); // v11;
            vertexArr[v+14] = GLfloat(0 * ATLAS_STEP_U + offsetU); // u00;
            vertexArr[v+15] = GLfloat(0 * ATLAS_STEP_V + offsetV); // v00;
            vertexArr[v+18] = GLfloat(0 * ATLAS_STEP_U + offsetU); // u01;
            vertexArr[v+19] = GLfloat(1 * ATLAS_STEP_V + offsetV); // v01;
            vertexArr[v+22] = GLfloat(1 * ATLAS_STEP_U + offsetU); // u11;
            vertexArr[v+23] = GLfloat(1 * ATLAS_STEP_V + offsetV); // v11;
        }
    }
}

// =============================================================================
// Buffer Chunk Render Data
// =============================================================================
void Chunk::bufferData() {
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, CHUNK_BUFFER_SIZE * sizeof(GLfloat), &vertexArr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

// =============================================================================
// Render Tiles
// =============================================================================
void Chunk::render(Camera& camera) {
    glUseProgram(shader.program);
    GLint projLocation = glGetUniformLocation(shader.program, "projection");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &camera.projVec[0]);
    GLint viewLocation = glGetUniformLocation(shader.program, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &camera.viewVec[0]);
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glDrawArrays(GL_TRIANGLES, 0, CHUNK_BUFFER_SIZE);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // TODO: needed?
    glBindVertexArray(0); // TODO: needed?
    glUseProgram(0);
}

#endif // CHUNK_H