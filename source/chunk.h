#ifndef CHUNK_H
#define CHUNK_H

#include <GL/glew.h>
#include <SDL.h>
#include <vector>

#define TILE_COUNT_X 16
#define TILE_COUNT_Y 16
#define TILE_COUNT (TILE_COUNT_X * TILE_COUNT_Y)
#define TILE_PIXELS_X 32
#define TILE_PIXELS_Y 32
#define CHUNK_PIXELS_X (TILE_COUNT_X * TILE_PIXELS_X)
#define CHUNK_PIXELS_Y (TILE_COUNT_Y * TILE_PIXELS_Y)
#define NUM_ELEMENTS_PER_VERTEX 4
#define NUM_VERTICES_PER_TILE 6
#define NUM_ELEMENTS_PER_TILE (NUM_ELEMENTS_PER_VERTEX * NUM_VERTICES_PER_TILE)

#define ATLAS_TILE_COUNT_X 8
#define ATLAS_TILE_COUNT_Y 8

// =============================================================================
// Chunk Class
// =============================================================================
class Chunk {
    public:
        Chunk();
        //~Chunk();
        void setVertexXY(int x, int y);
        void setVertexUV();
        void bufferData();
        void render();
    
    public:
        int x;
        int y;
        GLuint vaoId = 0;
        GLuint vboId = 0;
        std::vector<GLfloat> vertexVec;
        std::vector<int> tileIds;
};

// =============================================================================
// Construct Chunk
// =============================================================================
Chunk::Chunk() {

    vertexVec.resize(NUM_ELEMENTS_PER_TILE * TILE_COUNT);
    tileIds.resize(TILE_COUNT);

    for (int i = 0; i < TILE_COUNT; i++) {
        tileIds[i] = 0;
    }

    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);
}

// =============================================================================
// Update Tile Positions
// =============================================================================
void Chunk::setVertexXY(int x, int y) {
    this->x = x;
    this->y = y;

    int cX = (x * CHUNK_PIXELS_X) - (CHUNK_PIXELS_X / 2);
    int cY = (y * CHUNK_PIXELS_Y) - (CHUNK_PIXELS_Y / 2);

    for (int i = 0; i < TILE_COUNT; i++) {
        int j = i * NUM_ELEMENTS_PER_TILE;
        int tX = (i / TILE_COUNT_X) * TILE_PIXELS_X;
        int tY = (i % TILE_COUNT_Y) * TILE_PIXELS_Y;

        vertexVec[j+ 0] = GLfloat(cX + tX + (0 * TILE_PIXELS_X)); // x00;
        vertexVec[j+ 1] = GLfloat(cY + tY + (0 * TILE_PIXELS_Y)); // y00;
        vertexVec[j+ 4] = GLfloat(cX + tX + (1 * TILE_PIXELS_X)); // x10;
        vertexVec[j+ 5] = GLfloat(cY + tY + (0 * TILE_PIXELS_Y)); // y10;
        vertexVec[j+ 8] = GLfloat(cX + tX + (1 * TILE_PIXELS_X)); // x11;
        vertexVec[j+ 9] = GLfloat(cY + tY + (1 * TILE_PIXELS_Y)); // y11;
        vertexVec[j+12] = GLfloat(cX + tX + (0 * TILE_PIXELS_X)); // x00;
        vertexVec[j+13] = GLfloat(cY + tY + (0 * TILE_PIXELS_Y)); // y00;
        vertexVec[j+16] = GLfloat(cX + tX + (0 * TILE_PIXELS_X)); // x01;
        vertexVec[j+17] = GLfloat(cY + tY + (1 * TILE_PIXELS_Y)); // y01;
        vertexVec[j+20] = GLfloat(cX + tX + (1 * TILE_PIXELS_X)); // x11;
        vertexVec[j+21] = GLfloat(cY + tY + (1 * TILE_PIXELS_Y)); // y11;
    }
}

// =============================================================================
// Update Tile Texture Coordinates
// =============================================================================
void Chunk::setVertexUV(){

    // calculate atlas step size
    float stepU = 1.0f / float(ATLAS_TILE_COUNT_X);
    float stepV = 1.0f / float(ATLAS_TILE_COUNT_Y);

    for (int i = 0; i < TILE_COUNT; i++) {
        int j = i * NUM_ELEMENTS_PER_TILE;

        // calculate atlas offset texture coordinates
        float aU = (tileIds[i] % 8) * stepU;
        float aV = (tileIds[i] / 8) * stepV;

        vertexVec[j+ 2] = (0.0f * stepU) + aU; // u00;
        vertexVec[j+ 3] = (0.0f * stepV) + aV; // v00;
        vertexVec[j+ 6] = (1.0f * stepU) + aU; // u10;
        vertexVec[j+ 7] = (0.0f * stepV) + aV; // v10;
        vertexVec[j+10] = (1.0f * stepU) + aU; // u11;
        vertexVec[j+11] = (1.0f * stepV) + aV; // v11;
        vertexVec[j+14] = (0.0f * stepU) + aU; // u00;
        vertexVec[j+15] = (0.0f * stepV) + aV; // v00;
        vertexVec[j+18] = (0.0f * stepU) + aU; // u01;
        vertexVec[j+19] = (1.0f * stepV) + aV; // v01;
        vertexVec[j+22] = (1.0f * stepU) + aU; // u11;
        vertexVec[j+23] = (1.0f * stepV) + aV; // v11;
    }
}

// =============================================================================
// Buffer Chunk Render Data
// =============================================================================
void Chunk::bufferData() {
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertexVec.size() * sizeof(GLfloat), vertexVec.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

// =============================================================================
// Render Tiles
// =============================================================================
void Chunk::render() {
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexVec.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0); // TODO: needed?
    glBindVertexArray(0); // TODO: needed?
}

#endif // CHUNK_H
