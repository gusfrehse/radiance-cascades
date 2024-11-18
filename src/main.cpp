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
#include <cmath>

const int width = 854;
const int height = 480;

/// expects that framebuffer is already bound, correct vao bound and using correct shader program.
void jfa_pass(
  gl::GLuint input_texture,
  gl::GLuint output_texture,
  gl::GLuint framebuffer,
  gl::GLint step_location,
  int step
) {
  // attach output texture to framebuffer
  gl::glNamedFramebufferTexture(framebuffer, gl::GL_COLOR_ATTACHMENT0, output_texture, 0);
  gl::glBindTextureUnit(0, input_texture);
  gl::glDrawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
}

void jfa(gl::GLuint &input_texture, gl::GLuint &output_texture, gl::GLuint framebuffer, gl::GLuint program) {
  gl::glUseProgram(program);
  gl::GLint step_location = gl::glGetUniformLocation(program, "u_step");
  gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, framebuffer);
  for (int i = 0, max_steps = std::log2(std::max(width, height)); i < 4; i++) {
    jfa_pass(input_texture, output_texture, framebuffer, step_location, i);
    std::swap(input_texture, output_texture);
  }
  gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
}

int main(int argc, char **argv)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    std::cerr << "Failed to initialize SDL." << std::endl;
  }

  std::cerr << "SDL initialized." << std::endl;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

  SDL_Window *window =
      SDL_CreateWindow("Radiance Cascades", width, height, SDL_WINDOW_OPENGL);

  if (!window)
  {
    std::cerr << "Couldn't create window: " << SDL_GetError() << std::endl;
    std::exit(1);
  }

  std::cerr << "Window created." << std::endl;

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (!context)
  {
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
  
  Shader vertex_jfa("shaders/base.vert", gl::GL_VERTEX_SHADER);
  Shader fragment_jfa("shaders/jfa_pass.frag", gl::GL_FRAGMENT_SHADER);
  ShaderProgram jfa_program(vertex_jfa, fragment_jfa);

  gl::GLuint vao;
  gl::glCreateVertexArrays(1, &vao);
  gl::glBindVertexArray(vao);

  gl::GLuint framebuffers[4];
  gl::glCreateFramebuffers(4, framebuffers);

  gl::GLuint textures[4];
  gl::glCreateTextures(gl::GL_TEXTURE_2D, 4, textures);
  
  gl::GLint location = gl::glGetUniformLocation(program.get_id(), "input"); // sampler
  gl::glProgramUniform1i(program.get_id(), location, 0); // texture unit 0

  // TODO: giving invalid operation
  for (int i = 0; i < 4; i++)
  {
    gl::glTextureStorage2D(textures[i], 1, gl::GL_RGBA8, width, height);
    gl::glClearTexImage(textures[i], 0, gl::GL_RGBA, gl::GLenum::GL_FLOAT, NULL);
  }
  
  // add some data to the texture just for testing
  float *data0 = new float[width * height * 4];
  for (int i = 0; i < width * height * 4; i += 4) {
    if (i % (width / 5 + height)  == 0) {
      data0[i + 0] = float(i) / float(width * height * 4);
      data0[i + 1] = 1.0f - float(i) / float(width * height * 4);
      data0[i + 2] = 0.0f;
      data0[i + 3] = 1.0f;
    }
  }

  gl::glTextureSubImage2D(textures[0], 0, 0, 0, width, height, gl::GL_RGBA, gl::GL_FLOAT, data0);
  //delete data0;
  

  gl::glClearColor(0.4f, 0.5f, 0.2f, 1.0f);

  bool shouldQuit = false;
  while (!shouldQuit)
  {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_EVENT_QUIT)
      {
        shouldQuit = true;
      }
    }

    gl::glTextureSubImage2D(textures[0], 0, 0, 0, width, height, gl::GL_RGBA, gl::GL_FLOAT, data0);

    jfa(textures[0], textures[1], framebuffers[0], jfa_program.get_id());

    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    gl::glUseProgram(program.get_id());
    gl::glDrawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);

    SDL_GL_SwapWindow(window);
  }

  return 0;
}
