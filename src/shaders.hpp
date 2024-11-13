#pragma once

#include <glbinding/gl/gl.h>

class Shader {
  gl::GLuint m_id;
  const char *m_path = nullptr;

public:
  Shader(const char *path, gl::GLenum type);
  auto get_id() -> gl::GLuint { return m_id; };
  auto get_path() -> const char * { return m_path; };
};

class ShaderProgram {
  gl::GLuint m_id;
  Shader m_vertex_shader;
  Shader m_fragment_shader;

public:
  ShaderProgram(const Shader &vertex_shader, const Shader &fragment_shader);
  auto get_id() -> gl::GLuint { return m_id; };
};
