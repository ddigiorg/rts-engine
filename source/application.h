#ifndef APPLICATION_H
#define APPLICATION_H

// local includes
#include "types.h"
#include "camera.h"
#include "chunk_manager.h"

// third party includes
#include <GL/glew.h>
#include <SDL.h>

// STL includes
#include <iostream>
#include <random>

// =============================================================================
// Application Class
// =============================================================================
class Application {
    public:
        Application();
        ~Application();
        void printOpenGLInfo();
        void run();

    private:
        void handleInputEvents();

        bool isRunning = true;
        int screenX = 800;
        int screenY = 600;
        int mouseX = 0;
        int mouseY = 0;
        float cameraVelX = 0.0f;
        float cameraVelY = 0.0f;

        unsigned int seed = 0;
        std::mt19937 rng;

        SDL_Window* window = nullptr;
        SDL_GLContext context = nullptr;

        Camera camera;
        ChunkManager chunkManager;
};

// =============================================================================
// Construct Application
// =============================================================================
Application::Application() {

    // setup random number generator
    seed = 0;
    rng = std::mt19937(seed);

    // TODO: figure out if this is needed
    //std::random_device rd;
    //std::mt19937 rng(rd());

    // setup SDL
    if(SDL_Init(SDL_INIT_VIDEO < 0)) {
        std::cout << "ERROR: SDL could not be innitialized." << std::endl
                  << SDL_GetError() << std::endl;
        exit(1);
    }

    // setup OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // setup SDL window
    window = SDL_CreateWindow(
        "OpenGL Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenX,
        screenY,
        SDL_WINDOW_OPENGL
    );
    if(window == nullptr) {
        std::cout << "ERROR: SDL window could not be created." << std::endl
                  << SDL_GetError() << std::endl;
        exit(1);
    }

    // setup OpenGL graphics context
    context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cout << "ERROR: OpenGL context could not be created." << std::endl
                  << SDL_GetError() << std::endl;
        exit(1);
    }

    // setup GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK){
        std::cout << "ERROR: GLEW could not be initialized." << std::endl
                  << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    // setup OpenGL capabilities
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, screenX, screenY);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // setup camera
    camera.initOrthographic(screenX, screenY);
    camera.initView(0.0, 0.0, 0.0);

    // setup chunk manager
    chunkManager.update({0.0, 0.0, 0.0});
}

// =============================================================================
// Destruct Application
// =============================================================================
Application::~Application() {
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);
    SDL_Quit();
}

// =============================================================================
// Handle Input Events
// =============================================================================
void Application::handleInputEvents() {
    SDL_Event event;

    // poll for events
    while(SDL_PollEvent(&event)) {

        // handle mouse down event
        if(event.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&mouseX, &mouseY);
        }

        // handle key down event
        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE: {
                    isRunning = false;
                    break;
                }
                case SDLK_UP: {
                    cameraVelY = -8.0f;
                    break;
                }
                case SDLK_DOWN: {
                    cameraVelY = 8.0f;
                    break;
                }
                case SDLK_LEFT: {
                    cameraVelX = -8.0f;
                    break;
                }
                case SDLK_RIGHT: {
                    cameraVelX = 8.0f;
                    break;
                }
            }
        }

        // handle key up event
        if(event.type == SDL_KEYUP) {
            switch(event.key.keysym.sym){
                case SDLK_UP: {
                    if(cameraVelY < 0.0f)
                        cameraVelY = 0.0f;
                    break;
                }
                case SDLK_DOWN: {
                    if(cameraVelY > 0.0f)
                        cameraVelY = 0.0f;
                    break;
                }
                case SDLK_LEFT: {
                    if(cameraVelX < 0.0f)
                        cameraVelX = 0.0f;
                    break;
                }
                case SDLK_RIGHT: {
                    if(cameraVelX > 0.0f)
                        cameraVelX = 0.0f;
                    break;
                }
            }
        }

        // handle quit event
        if(event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

// =============================================================================
// Run Application
// =============================================================================
void Application::run() {

    // loop
    while(isRunning) {

        // handle input events
        handleInputEvents();

        // update objects
        if(cameraVelX != 0 || cameraVelY != 0) {
            camera.moveView(
                camera.pos.x + cameraVelX,
                camera.pos.y + cameraVelY
            );

            // update chunks
            chunkManager.update(camera.pos);
        }

        // draw
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        chunkManager.render(camera);

        // update screen
        SDL_GL_SwapWindow(window);
    }
}

// =============================================================================
// Print OpenGL Informationn
// =============================================================================
void Application::printOpenGLInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

#endif // APPLICATION_H