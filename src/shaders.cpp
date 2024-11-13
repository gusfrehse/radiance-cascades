#include "shaders.hpp"

#include <fstream>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/gl.h>
#include <glbinding/gl/types.h>
#include <ios>
#include <iostream>

Shader::Shader(const char *path, gl::GLenum type) : m_path(path) {
  char *source;
  int source_length;

  { // Open file and copy contents to `source`
    std::ifstream file(path, std::ios::binary);

    if (!file) {
      std::cerr << "Failed to open shader file '" << path << "'." << std::endl;
      std::exit(1);
    }

    file.seekg(0, std::ios::end);
    source_length = file.tellg();
    file.seekg(0, std::ios::beg);

    source = new char[source_length];

    if (!file.read(source, source_length)) {
      std::cerr << "Failed to read shader file '" << path << "'." << std::endl;
      std::exit(1);
    }

    file.close();
  }

  m_id = gl::glCreateShader(type);
  gl::glShaderSource(m_id, 1, &source, &source_length);

  gl::glCompileShader(m_id);

  gl::GLint compile_successfull;
  gl::glGetShaderiv(m_id, gl::GL_COMPILE_STATUS, &compile_successfull);

  if ((gl::GLboolean)compile_successfull != gl::GL_TRUE) {
    gl::GLsizei log_length;
    gl::GLchar log[1024];
    gl::glGetShaderInfoLog(m_id, sizeof(log), &log_length, log);

    std::cerr << "Failed to compile shader '" << path << "':" << std::endl;
    std::cerr << log << std::endl;
    // std::exit(1);
  }
}

ShaderProgram::ShaderProgram(const Shader &vertex_shader,
                             const Shader &fragment_shader)
    : m_vertex_shader(vertex_shader), m_fragment_shader(fragment_shader) {
  m_id = gl::glCreateProgram();
  gl::glAttachShader(m_id, m_vertex_shader.get_id());
  gl::glAttachShader(m_id, m_fragment_shader.get_id());
  gl::glLinkProgram(m_id);

  gl::GLint link_successfull;
  gl::glGetProgramiv(m_id, gl::GL_LINK_STATUS, &link_successfull);

  if ((gl::GLboolean)link_successfull != gl::GL_TRUE) {
    gl::GLsizei log_length;
    gl::GLchar log[1024];
    gl::glGetProgramInfoLog(m_id, sizeof(log), &log_length, log);

    std::cerr << "Failed to link program, with vertex shader '"
              << m_vertex_shader.get_path() << "' and fragment shader '"
              << m_fragment_shader.get_path() << "':" << std::endl;
    std::cerr << log << std::endl;
    // std::exit(1);
  }
};
