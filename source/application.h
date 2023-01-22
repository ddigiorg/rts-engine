#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <SDL.h>

#include <iostream>

// =============================================================================
// Application Class
// =============================================================================
class Application {
    public:
        Application();
        ~Application();
        void printOpenGLInfo();
        void handleInputEvents();

    public:
        bool isRunning;
        int screenX;
        int screenY;
        float cameraVelX;
        float cameraVelY;
        SDL_Window* window = nullptr;
        SDL_GLContext context = nullptr;
};

// =============================================================================
// Construct Application
// =============================================================================
Application::Application() {
    isRunning = true;
    screenX = 800;
    screenY = 600;
    cameraVelX = 0.0f;
    cameraVelY = 0.0f;

    // setup SDL
    if(SDL_Init(SDL_INIT_VIDEO < 0)) {
        std::cout << "ERROR: SDL could not be innitialized." << std::endl
                  << SDL_GetError() << std::endl;
        exit(1);
    }

    // setup opengl attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // setup sdl window
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

    // setup graphics context
    context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cout << "ERROR: OpenGL context could not be created." << std::endl
                  << SDL_GetError() << std::endl;
        exit(1);
    }

    // setup glew
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
}

// =============================================================================
// Destruct Application
// =============================================================================
Application::~Application() {
    SDL_DestroyWindow(window);
    SDL_Quit();
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

// =============================================================================
// Print OpenGL Informationn
// =============================================================================
void Application::handleInputEvents() {
    SDL_Event event;

    // poll for events
    while(SDL_PollEvent(&event)) {

        // handle mouse down event
        if(event.type == SDL_MOUSEBUTTONDOWN) {
            int x = 0;
            int y = 0;
            SDL_GetMouseState(&x, &y);
            std::cout << x << ", " << y << std::endl;
        }

        // handle key down event
        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE: {
                    isRunning = false;
                    break;
                }
                case SDLK_UP: {
                    cameraVelY = 8.0f;
                    break;
                }
                case SDLK_DOWN: {
                    cameraVelY = -8.0f;
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
                    if(cameraVelY > 0.0f)
                        cameraVelY = 0.0f;
                    break;
                }
                case SDLK_DOWN: {
                    if(cameraVelY < 0.0f)
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

#endif // APPLICATION_H
