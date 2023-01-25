#ifndef CHUNK_H
#define CHUNK_H

#include <GL/glew.h>
#include <SDL.h>

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

#define TILE_COUNT_X 16
#define TILE_COUNT_Y 16
#define TILE_COUNT (TILE_COUNT_X * TILE_COUNT_Y)
#define TILE_SIZE_X 32
#define TILE_SIZE_Y 32
#define CHUNK_SIZE_X (TILE_COUNT_X * TILE_SIZE_X)
#define CHUNK_SIZE_Y (TILE_COUNT_Y * TILE_SIZE_Y)
#define NUM_VERTICES_PER_TILE 6
#define NUM_ELEMENTS_PER_VERTEX 4
#define NUM_ELEMENTS_PER_TILE (NUM_VERTICES_PER_TILE * NUM_ELEMENTS_PER_VERTEX)
#define NUM_ELEMENTS (NUM_ELEMENTS_PER_TILE * TILE_COUNT)

#define ATLAS_COUNT_U 8
#define ATLAS_COUNT_V 8
#define ATLAS_STEP_U (1.0f / float(ATLAS_COUNT_U))
#define ATLAS_STEP_V (1.0f / float(ATLAS_COUNT_V))

// =============================================================================
// Chunk Class
// =============================================================================
class Chunk {
    public:
        Chunk();
        //~Chunk();
        void loadData(std::vector<std::uint8_t>& data);
        void setVertexXY(int x, int y);
        void setVertexUV();
        void bufferData();
        void render();
    
    public:
        int x;
        int y;
        GLuint vaoId = 0;
        GLuint vboId = 0;
        std::vector<std::uint8_t> data;
        std::vector<GLfloat> vertexVec;

        std::vector<int> autotiles;
        std::vector<int> water_map;
};

// =============================================================================
// Construct Chunk
// =============================================================================
Chunk::Chunk() {

    vertexVec.resize(NUM_ELEMENTS);
    data.resize(TILE_COUNT);
    autotiles.resize(TILE_COUNT);

    for (int i = 0; i < TILE_COUNT; i++) {
        data[i] = 0;
        autotiles[i] = 62;
    }

    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);

    // tile map stuff

    water_map.resize(256);

    for(int i = 0; i < water_map.size(); i++) {
        water_map[i] = 7;
    }

    water_map[  0] =  0;
    water_map[  1] =  1;
    water_map[  7] =  2;
    water_map[197] =  3;
    water_map[ 69] =  4;
    water_map[ 68] =  5;
    water_map[ 65] =  6;

    water_map[  5] =  8;
    water_map[ 84] =  9;
    water_map[ 93] = 10;
    water_map[119] = 11;
    water_map[215] = 12;
    water_map[193] = 13;
    water_map[ 17] = 14;

    water_map[ 23] = 16;
    water_map[199] = 17;
    water_map[213] = 18;
    water_map[ 95] = 19;
    water_map[255] = 20;
    water_map[245] = 21;
    water_map[ 81] = 22;

    water_map[ 31] = 24;
    water_map[253] = 25;
    water_map[113] = 26;
    water_map[ 28] = 27;
    water_map[127] = 28;
    water_map[247] = 29;
    water_map[209] = 30;

    water_map[ 29] = 32;
    water_map[117] = 33;
    water_map[ 85] = 34;
    water_map[ 71] = 35;
    water_map[221] = 36;
    water_map[125] = 37;
    water_map[112] = 38;

    water_map[ 16] = 40;
    water_map[ 20] = 41;
    water_map[ 87] = 42;
    water_map[223] = 43;
    water_map[241] = 44;
    water_map[ 21] = 45;
    water_map[ 64] = 46;

    water_map[  4] = 49;
    water_map[ 92] = 50;
    water_map[124] = 51;
    water_map[116] = 52;
    water_map[ 80] = 53;
}

// =============================================================================
// Load Data
// =============================================================================
void Chunk::loadData(std::vector<std::uint8_t>& input) {

    // TODO: need to understand the coordinate system better... the indices of
    //       data vectors dont line up with the tile coordinates in a chunk...
    //       basically the y indicies are flipped right now

    // load data
    for (int i = 0; i < TILE_COUNT; i++) {
        int x = i % TILE_COUNT_X;
        int y = (TILE_COUNT_Y - 1) - (i / TILE_COUNT_Y);
        int j = y * TILE_COUNT_X + x;
        data[j] = input[i];
    }

    // loop through tiles
    for (int i = 0; i < TILE_COUNT; i++) {

        // TODO: temporary stuffs
        if(data[i] == 0) {
            autotiles[i] = 62;
            continue;
        }
        if(data[i] == 2) {
            autotiles[i] = 63;
            continue;
        }

        // handle tile coordinates
        int x = i % TILE_COUNT_X;
        int y = (TILE_COUNT_Y - 1) - (i / TILE_COUNT_X);

        // get surrounding tile indicies
        int iT  = i + TILE_COUNT_X;     // top
        int iTR = i + TILE_COUNT_X + 1; // top right
        int iR  = i + 1;                // right
        int iBR = i - TILE_COUNT_X + 1; // bottom right
        int iB  = i - TILE_COUNT_X;     // bottom
        int iBL = i - TILE_COUNT_X - 1; // bottom left
        int iL  = i - 1;                // left
        int iTL = i + TILE_COUNT_X - 1; // top left

        // bool
        bool boundsT = y - 1 < 0;
        bool boundsR = x + 1 >= TILE_COUNT_X;
        bool boundsB = y + 1 >= TILE_COUNT_Y;
        bool boundsL = x - 1 < 0;

        // 
        bool t  = false;
        bool tr = false;
        bool r  = false;
        bool br = false;
        bool b  = false;
        bool bl = false;
        bool l  = false;
        bool tl = false;

        // handle top tile
        if (boundsT)
            t = true;
        else
            t = data[i] == data[iT] ? true : false;

        // handle right tile
        if (boundsR)
            r = true;
        else
            r = data[i] == data[iR] ? true : false;

        // handle bottom tile
        if (boundsB)
            b = true;
        else
            b = data[i] == data[iB] ? true : false;

        // handle left tile
        if (boundsL)
            l = true;
        else
            l = data[i] == data[iL] ? true : false;

        // handle top right tile
        if (boundsT || boundsR) {
            if (t && r)
                tr = true;
            else
                tr = false;
        }   
        else if (t && r)
            tr = data[i] == data[iTR] ? true : false;

        // handle bottom right tile
        if (boundsB || boundsR) {
            if (b && r)
                br = true;
            else
                br = false;
        }  
        else if (b && r)
            br = data[i] == data[iBR] ? true : false;

        // handle bottom left tile
        if (boundsB || boundsL) {
            if (b && l)
                bl = true;
            else
                bl = false;
        }  
        else if (b && l)
            bl = data[i] == data[iBL] ? true : false;

        // handle top left tile
        if (boundsT || boundsL) {
            if (t && l)
                tl = true;
            else
                tl = false;
        }  
        else if (t && l)
            tl = data[i] == data[iTL] ? true : false;

        int idx = t + 2*tr + 4*r + 8*br + 16*b + 32*bl + 64*l + 128*tl;

        autotiles[i] = water_map[idx];

        // std::cout << "i=" << i << ", "
        //           << "idx=" << idx << ", "
        //           << "tile=" << autotiles[i] << ", "
        //           << "t="  << t  << ", "
        //           << "tr=" << tr << ", "
        //           << "r="  << r  << ", "
        //           << "br=" << br << ", "
        //           << "b="  << b  << ", "
        //           << "bl=" << bl << ", "
        //           << "l="  << l  << ", "
        //           << "tl=" << tl << std::endl;
    }
}

// =============================================================================
// Update Tile Positions
// =============================================================================
void Chunk::setVertexXY(int x, int y) {
    this->x = x;
    this->y = y;

    // calculate chunk position offsets
    int cX = (x * CHUNK_SIZE_X) - (CHUNK_SIZE_X / 2);
    int cY = (y * CHUNK_SIZE_Y) - (CHUNK_SIZE_Y / 2);

    // loop through each tile
    for (int i = 0; i < TILE_COUNT; i++) {

        // calculate tile's position offsets
        int offsetX = (i % TILE_COUNT_X) * TILE_SIZE_X + cX;
        int offsetY = (i / TILE_COUNT_X) * TILE_SIZE_Y + cY;

        // calculate tile's vertex vector offsets
        int v = i * NUM_ELEMENTS_PER_TILE;

        // set tile's vertex positions
        vertexVec[v+ 0] = GLfloat(0 * TILE_SIZE_X + offsetX); // x00;
        vertexVec[v+ 1] = GLfloat(0 * TILE_SIZE_Y + offsetY); // y00;
        vertexVec[v+ 4] = GLfloat(1 * TILE_SIZE_X + offsetX); // x10;
        vertexVec[v+ 5] = GLfloat(0 * TILE_SIZE_Y + offsetY); // y10;
        vertexVec[v+ 8] = GLfloat(1 * TILE_SIZE_X + offsetX); // x11;
        vertexVec[v+ 9] = GLfloat(1 * TILE_SIZE_Y + offsetY); // y11;
        vertexVec[v+12] = GLfloat(0 * TILE_SIZE_X + offsetX); // x00;
        vertexVec[v+13] = GLfloat(0 * TILE_SIZE_Y + offsetY); // y00;
        vertexVec[v+16] = GLfloat(0 * TILE_SIZE_X + offsetX); // x01;
        vertexVec[v+17] = GLfloat(1 * TILE_SIZE_Y + offsetY); // y01;
        vertexVec[v+20] = GLfloat(1 * TILE_SIZE_X + offsetX); // x11;
        vertexVec[v+21] = GLfloat(1 * TILE_SIZE_Y + offsetY); // y11;
    }
}

// =============================================================================
// Update Tile Texture Coordinates
// =============================================================================
void Chunk::setVertexUV(){

    // loop through each tile
    for (int i = 0; i < TILE_COUNT; i++) {

        // calculate tile's texture atlas coordinates
        float offsetU = (autotiles[i] % ATLAS_COUNT_U) * ATLAS_STEP_U;
        float offsetV = (autotiles[i] / ATLAS_COUNT_V) * ATLAS_STEP_V;

        // calculate tile's vertex vector offsets
        int v = i * NUM_ELEMENTS_PER_TILE;

        vertexVec[v+ 2] = GLfloat(0 * ATLAS_STEP_U + offsetU); // u00;
        vertexVec[v+ 3] = GLfloat(0 * ATLAS_STEP_V + offsetV); // v00;
        vertexVec[v+ 6] = GLfloat(1 * ATLAS_STEP_U + offsetU); // u10;
        vertexVec[v+ 7] = GLfloat(0 * ATLAS_STEP_V + offsetV); // v10;
        vertexVec[v+10] = GLfloat(1 * ATLAS_STEP_U + offsetU); // u11;
        vertexVec[v+11] = GLfloat(1 * ATLAS_STEP_V + offsetV); // v11;
        vertexVec[v+14] = GLfloat(0 * ATLAS_STEP_U + offsetU); // u00;
        vertexVec[v+15] = GLfloat(0 * ATLAS_STEP_V + offsetV); // v00;
        vertexVec[v+18] = GLfloat(0 * ATLAS_STEP_U + offsetU); // u01;
        vertexVec[v+19] = GLfloat(1 * ATLAS_STEP_V + offsetV); // v01;
        vertexVec[v+22] = GLfloat(1 * ATLAS_STEP_U + offsetU); // u11;
        vertexVec[v+23] = GLfloat(1 * ATLAS_STEP_V + offsetV); // v11;
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
