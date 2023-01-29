#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include "types.h"
#include "chunk.h"

#include <unordered_map>
#include <iostream>
#include <vector>

// =============================================================================
// Chunk Class
// =============================================================================
class ChunkManager {
    public:
        ChunkManager();
        void update(vec3f_t cameraPos);
        void render(Camera& camera);

        vec2i_t getChunkPositionAt(vec3f_t cameraPos);

    private:
        size_t hash(const int& a, const int& b);
    
    private:
        int radiusX;
        int radiusY;
        vec2i_t chunkPosPrev;
        std::unordered_map<size_t, Chunk> chunks;
};

// =============================================================================
// Construct ChunkManager
// =============================================================================
ChunkManager::ChunkManager() {

    radiusX = 1;
    radiusY = 1;

    // TODO: need to set chunkPosPrev from camera instead
    // update(cameraPos);
    // loop through chunk activation area
    for (int y = -radiusY; y <= radiusY; y++) {
        for (int x = -radiusX; x <= radiusX; x++) {
            size_t h = hash(x, y);

            // setup new chunks
            chunks[h] = Chunk();
            chunks[h].updatePosition(x, y);
            chunks[h].updateTiles();
            chunks[h].bufferData();
        }
    }
}

// =============================================================================
// Update
// =============================================================================
void ChunkManager::update(vec3f_t cameraPos) {

    // get current chunk position
    vec2i_t chunkPos = getChunkPositionAt(cameraPos);

    // check if chunk position changed
    if (chunkPos.x == chunkPosPrev.x && chunkPos.y == chunkPosPrev.y) {
        return;
    }

    // deactivate all chunks
    for (auto &i : chunks) {
        i.second.active = false;
    }

    // loop through chunk activation area
    int xBeg = chunkPos.x - radiusX;
    int xEnd = chunkPos.x + radiusX;
    int yBeg = chunkPos.y - radiusY;
    int yEnd = chunkPos.y + radiusY;

    for (int y = yBeg; y <= yEnd; y++) {
        for (int x = xBeg; x <= xEnd; x++) {
            size_t h = hash(x, y);

            // setup new chunks if not currently in the chunks map
            std::unordered_map<size_t, Chunk>::const_iterator i = chunks.find(h);
            if (i == chunks.end()) {
                chunks[h] = Chunk();
                chunks[h].updatePosition(x, y);
                chunks[h].updateTiles();
                chunks[h].bufferData();
            }

            // activate chunks within chunk activation area
            chunks[h].active = true;
        }
    }

    // select inactive chunks
    std::vector<size_t> inactives;
    for (auto &i : chunks) {
        if (i.second.active == false) {
            vec2i_t pos = i.second.getPosition();
            size_t h = hash(pos.x, pos.y);
            inactives.emplace_back(h);
        }
    }

    // delete inactive chunks from the chunks map
    for (int i = 0; i < inactives.size(); i++) {
        chunks.erase(inactives[i]);
    }

    chunkPosPrev = chunkPos;

    std::cout << chunks.size() << std::endl;
}

// =============================================================================
// Render
// =============================================================================
void ChunkManager::render(Camera& camera) {
    for (auto &i : chunks) {
        i.second.render(camera);
    }
}

// =============================================================================
// Get Chunk Position At
// =============================================================================
vec2i_t ChunkManager::getChunkPositionAt(vec3f_t cameraPos) {

    int signX = int(cameraPos.x) >> 31;
    int signY = int(cameraPos.y) >> 31;

    // TODO: clean this up but be careful with the floating points!
    vec2i_t chunkPos;
    chunkPos.x = int(((cameraPos.x + CHUNK_PIXELS_HALF_X) / CHUNK_PIXELS_X) + signX);
    chunkPos.y = int(((cameraPos.y + CHUNK_PIXELS_HALF_Y) / CHUNK_PIXELS_Y) + signY);

    return chunkPos;
}

// =============================================================================
// Hash
// =============================================================================
size_t ChunkManager::hash(const int& a, const int& b) {
    return (size_t(a) << 32) + size_t(b);
}

#endif // CHUNK_MANAGER_H