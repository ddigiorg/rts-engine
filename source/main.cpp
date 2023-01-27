#include <GL/glew.h>
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "application.h"
#include "camera.h"
#include "chunk.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    TileArray2D data = {
        {2, 2, 1, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2},
        {0, 0, 0, 0, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 2, 2, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 2},
        {0, 0, 0, 2, 2, 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 2, 2, 2, 1, 1, 1, 2, 0, 0, 2, 2},
        {2, 2, 2, 0, 0, 0, 2, 0, 1, 1, 1, 0, 0, 0, 2, 2},
        {2, 2, 2, 2, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 2, 2},
        {2, 2, 2, 2, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {2, 2, 2, 0, 0, 0, 2, 2, 1, 1, 2, 2, 2, 0, 0, 0},
        {2, 2, 2, 2, 0, 0, 2, 1, 1, 1, 2, 2, 2, 2, 0, 0},
    };

    // setup application
    Application app;

    // setup camera
    Camera camera;
    camera.initOrthographic(app.screenX, app.screenY);
    camera.initView();

    // TODO: PUT THIS IN A CLASS
    // setup texture atlas
    GLuint textureId = 0;
    int tX, tY, tC;
    GLubyte* pixels = stbi_load("D:/_projects/rts-engine/resources/images/terrain16.png", &tX, &tY, &tC, 0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tX, tY, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // setup tiles
    Chunk chunk;
    //chunk.loadData(data);
    chunk.updatePosition(0, 0);
    chunk.updateTiles();
    chunk.bufferData();

    // std::vector<Chunk> chunks(9);
    // std::vector<int> chunkX = {-1,  0,  1, -1,  0,  1, -1,  0,  1};
    // std::vector<int> chunkY = {-1, -1, -1,  0,  0,  0,  1,  1,  1};

    // for(int i = 0; i < 9; i++) {
    //     chunks[i].setVertexXY(chunkX[i], chunkY[i]);
    //     chunks[i].setVertexUV();
    //     chunks[i].bufferData();
    // }

    // main loop
    while(app.isRunning) {

        // handle input events
        app.handleInputEvents();

        // int x = app.mouseX - (app.screenX / 2) + camera.x;
        // int y = (app.screenY - app.mouseY) - (app.screenY / 2) + camera.y;
        // std::cout << x << ", " << y << std::endl;

        // update objects
        if(app.cameraVelX != 0 || app.cameraVelY != 0) {
            camera.moveView(
                camera.x + app.cameraVelX,
                camera.y + app.cameraVelY
            );

            // get camera position
            //std::cout << "camera=(" << camera.x << ", " << camera.y << ")" << std::endl;

            // get camera position in chunk coords
            //int cameraChunkX = int(((camera.x + 256) / 512) + (int(camera.x) >> 31));
            //int cameraChunkY = int(((camera.y + 256) / 512) + (int(camera.y) >> 31));
            //std::cout << cameraChunkX << ", " << cameraChunkY << std::endl;
        }

        // draw
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        chunk.render(camera);
        // for(int i = 0; i < 9; i++) {
        //     chunks[i].render();
        // }


        // update screen
        SDL_GL_SwapWindow(app.window);
    }

    // cleanup
    stbi_image_free(pixels);

    return 0;
}