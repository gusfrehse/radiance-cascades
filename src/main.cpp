#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "Failed to initialized SDL." << std::endl;
    }

    return 0;
}
