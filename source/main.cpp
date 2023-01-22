#include <GL/glew.h>
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "application.h"
#include "camera.h"
#include "chunk.h"
#include "shader.h"

#include <iostream>
#include <vector>
#include <string>

// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    //stbi_set_flip_vertically_on_load(true);

    // setup application
    Application app;

    // setup camera
    Camera camera(app.screenX, app.screenY);
    camera.initOrthographic();
    camera.initView();

    // setup texture atlas
    GLuint textureId = 0;
    int tX, tY, tC;
    GLubyte* pixels = stbi_load("D:/_projects/rts-engine/resources/images/terrain32.png", &tX, &tY, &tC, 0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tX, tY, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // setup tiles
    Chunk chunk;
    chunk.tileIds[0] = 1;
    chunk.setVertexXY(0, 0);
    chunk.setVertexUV();
    chunk.bufferData();

    // std::vector<Chunk> chunks(9);
    // std::vector<int> chunkX = {-1,  0,  1, -1,  0,  1, -1,  0,  1};
    // std::vector<int> chunkY = {-1, -1, -1,  0,  0,  0,  1,  1,  1};

    // for(int i = 0; i < 9; i++) {
    //     chunks[i].setVertexXY(chunkX[i], chunkY[i]);
    //     chunks[i].setVertexUV();
    //     chunks[i].bufferData();
    // }

    // setup shaders
    std::string vsFilename = "D:/_projects/rts-engine/resources/shaders/chunk_vert.glsl";
    std::string fsFilename = "D:/_projects/rts-engine/resources/shaders/chunk_frag.glsl";
    Shader shader(vsFilename, fsFilename);

    // main loop
    while(app.isRunning) {

        // handle input events
        app.handleInputEvents();

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
        glUseProgram(shader.program);
        GLint projLocation = glGetUniformLocation(shader.program, "projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, &camera.projVec[0]);
        GLint viewLocation = glGetUniformLocation(shader.program, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &camera.viewVec[0]);
        chunk.render();
        // for(int i = 0; i < 9; i++) {
        //     chunks[i].render();
        // }
        glUseProgram(0);

        // update screen
        SDL_GL_SwapWindow(app.window);
    }

    // cleanup
    stbi_image_free(pixels);

    return 0;
}