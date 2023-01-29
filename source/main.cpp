#include <GL/glew.h>
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "application.h"
#include "camera.h"
#include "chunk.h"
#include "chunk_manager.h"
#include "types.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

// =============================================================================
// Main
// =============================================================================
int main(int argc, char* argv[]) {

    // setup application
    Application app;

    // setup camera
    Camera camera;
    camera.initOrthographic(app.screenX, app.screenY);
    camera.initView(0.0, 0.0, 0.0);

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
    ChunkManager cm;

    // main loop
    while(app.isRunning) {

        // handle input events
        app.handleInputEvents();

        // update objects
        if(app.cameraVelX != 0 || app.cameraVelY != 0) {
            camera.moveView(
                camera.pos.x + app.cameraVelX,
                camera.pos.y + app.cameraVelY
            );

            // update chunks
            cm.update(camera.pos);
        }

        // draw
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        cm.render(camera);

        // update screen
        SDL_GL_SwapWindow(app.window);
    }

    // cleanup
    stbi_image_free(pixels);

    return 0;
}