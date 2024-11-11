#include "SDL3/SDL_error.h"
#include "SDL3/SDL_video.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

int main(int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "Failed to initialize SDL." << std::endl;
    }

    std::cout << "SDL initialized." << std::endl;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    SDL_Window *window = SDL_CreateWindow("Radiance Cascades", 1280, 720, SDL_WINDOW_OPENGL);

    if (!window) {
        std::cout << "Couldn't create window: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cout << "Couldn't create OpenGL context: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    glbinding::initialize(SDL_GL_GetProcAddress);

    bool shouldQuit = false;
    while (!shouldQuit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                shouldQuit = true;
            }
        }
    }

    return 0;
}
