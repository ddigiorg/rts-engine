#ifndef CHUNK_H
#define CHUNK_H

#include <GL/glew.h>
#include <SDL.h>

#include <vector>
#include <string>
#include <iostream>

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
        void loadData(std::vector<int>& data);
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

        std::vector<int> grass_map;
        std::vector<int> water_map;
        std::vector<int> forest_map;
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

    // auto tile map stuff
    grass_map.resize(256);
    water_map.resize(256);
    forest_map.resize(256);

    for(int i = 0; i < grass_map.size(); i++) {
        grass_map[i] = 64;
        water_map[i] = 64;
        forest_map[i] = 64;
    }
        
    grass_map[90] = 0;
    grass_map[ 0] = 1;
    grass_map[88] = 2;
    grass_map[82] = 3;
    grass_map[74] = 4;
    grass_map[26] = 5;
    grass_map[80] = 6;
    grass_map[72] = 7;
    grass_map[18] = 8;
    grass_map[10] = 9;
    grass_map[64] = 10;
    grass_map[66] = 11;
    grass_map[ 2] = 12;
    grass_map[16] = 13;
    grass_map[24] = 14;
    grass_map[ 8] = 15;

    water_map[90] = 16;
    water_map[ 0] = 17;
    water_map[88] = 18;
    water_map[82] = 19;
    water_map[74] = 20;
    water_map[26] = 21;
    water_map[80] = 22;
    water_map[72] = 23;
    water_map[18] = 24;
    water_map[10] = 25;
    water_map[64] = 26;
    water_map[66] = 27;
    water_map[ 2] = 28;
    water_map[16] = 29;
    water_map[24] = 30;
    water_map[ 8] = 31;

    forest_map[90] = 32;
    forest_map[ 0] = 33;
    forest_map[88] = 34;
    forest_map[82] = 35;
    forest_map[74] = 36;
    forest_map[26] = 37;
    forest_map[80] = 38;
    forest_map[72] = 39;
    forest_map[18] = 40;
    forest_map[10] = 41;
    forest_map[64] = 42;
    forest_map[66] = 43;
    forest_map[ 2] = 44;
    forest_map[16] = 45;
    forest_map[24] = 46;
    forest_map[ 8] = 47;
}

// =============================================================================
// Load Data
// =============================================================================
void Chunk::loadData(std::vector<int>& data) {

    // TODO: need to understand the coordinate system better... the indices of
    //       data vectors dont line up with the tile coordinates in a chunk...
    //       basically the y indicies are flipped right now

    // load data
    for (int i = 0; i < data.size(); i++) {
        int x = i % TILE_COUNT_X;
        int y = (TILE_COUNT_Y - 1) - (i / TILE_COUNT_Y);
        int j = y * TILE_COUNT_X + x;
        tileIds[j] = data[i];
    }

    std::vector<int> binaryValues(tileIds.size());

    // auto tile
    for (int i = 0; i < tileIds.size(); i++) {
        bool t = false;
        bool b = false;
        bool l = false;
        bool r = false;

        // handle tile coordinates (TODO: wonky coordinates again...)
        int x = i % TILE_COUNT_X;
        int y = (TILE_COUNT_Y - 1) - (i / TILE_COUNT_Y);

        // handle top tile (TODO: wonky coordinates again...)
        if (y - 1 < 0)
            t = true;
        else
            t = tileIds[i + TILE_COUNT_X] == tileIds[i] ? true : false;

        // handle bottom tile (TODO: wonky coordinates again...)
        if (y + 1 >= TILE_COUNT_Y)
            b = true;
        else
            b = tileIds[i - TILE_COUNT_X] == tileIds[i] ? true : false;

        // handle left tile
        if (x - 1 < 0)
            l = true;
        else
            l = tileIds[i - 1] == tileIds[i] ? true : false;

        // handle right tile
        if (x + 1 >= TILE_COUNT_X)
            r = true;
        else
            r = tileIds[i + 1] == tileIds[i] ? true : false;

        binaryValues[i] = 2*t + 8*l + 16*r + 64*b;

        // std::cout << "i=" << i << ", "
        //           << "tile=" << tileIds[i] << ", "
        //           << "t=" << t << ", "
        //           << "b=" << b << ", "
        //           << "l=" << l << ", "
        //           << "r=" << r << ", "
        //           << "val=" << binaryValue << std::endl;
    }

    for(int i = 0; i < tileIds.size(); i++) {
        if(tileIds[i] == 0)
            tileIds[i] = grass_map[binaryValues[i]];
        else if(tileIds[i] == 16)
            tileIds[i] = water_map[binaryValues[i]];
        else if(tileIds[i] == 32)
            tileIds[i] = forest_map[binaryValues[i]];
    }
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
        int tX = (i % TILE_COUNT_X) * TILE_PIXELS_X;
        int tY = (i / TILE_COUNT_X) * TILE_PIXELS_Y;

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
        float aU = (tileIds[i] % ATLAS_TILE_COUNT_X) * stepU;
        float aV = (tileIds[i] / ATLAS_TILE_COUNT_X) * stepV;

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
