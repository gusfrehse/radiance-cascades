#include "SDL3/SDL_error.h"
#include "SDL3/SDL_video.h"
#include "shaders.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glbinding-aux/debug.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <iostream>

const int width = 1280;
const int height = 720;

int main(int argc, char **argv) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "Failed to initialize SDL." << std::endl;
  }

  std::cerr << "SDL initialized." << std::endl;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

  SDL_Window *window =
      SDL_CreateWindow("Radiance Cascades", width, height, SDL_WINDOW_OPENGL);

  if (!window) {
    std::cerr << "Couldn't create window: " << SDL_GetError() << std::endl;
    std::exit(1);
  }

  std::cerr << "Window created." << std::endl;

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (!context) {
    std::cerr << "Couldn't create OpenGL context: " << SDL_GetError()
              << std::endl;
    std::exit(1);
  }

  std::cerr << "Context created." << std::endl;

  glbinding::initialize(SDL_GL_GetProcAddress);
  glbinding::aux::enableGetErrorCallback();

  Shader vertex("shaders/base.vert", gl::GL_VERTEX_SHADER);
  Shader fragment("shaders/base.frag", gl::GL_FRAGMENT_SHADER);
  ShaderProgram program(vertex, fragment);

  gl::GLuint vao;
  gl::glCreateVertexArrays(1, &vao);
  gl::glBindVertexArray(vao);

  // gl::GLuint framebuffers[4];
  // gl::glGenFramebuffers(4, framebuffers);

  // gl::GLuint textures[4];
  // gl::glCreateTextures(gl::GL_TEXTURE_2D, 4, textures);

  // TODO: giving invalid operation
  // for (int i = 0; i < 4; i++) {
  //   gl::glTextureStorage2D(textures[i], 1, gl::GLenum::GL_RGBA8, width, height);
  //   gl::glNamedFramebufferTexture(framebuffers[i], gl::GL_COLOR_ATTACHMENT0,
  //                                 textures[i], 0);
  // }

  gl::glClearColor(0.4f, 0.5f, 0.2f, 1.0f);

  bool shouldQuit = false;
  while (!shouldQuit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        shouldQuit = true;
      }
    }

    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    gl::glUseProgram(program.get_id());
    gl::glDrawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);

    SDL_GL_SwapWindow(window);
  }

  return 0;
}
