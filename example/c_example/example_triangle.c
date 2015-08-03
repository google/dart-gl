// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/GL/GLFW/include/GLFW/glfw3.h"

GLuint vbo;

GLuint v_shader, f_shader, shader_program;

const float verts[] = {-1.0, -1.0, 0.0, 1.0, 1.0, -1.0,
                       0.0,  1.0,  0.0, 1.0, 0.0, 1.0};

const GLchar vs[] =
    "#version 100\n"
    "in vec4 position;\n"
    "void main() {\n"
    "  gl_Position = position;\n"
    "}\n";

const GLchar fs[] =
    "#version 100\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
    "}\n";

void check_error() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL error: %d\n", err);
  }
}

void get_shader_error(GLuint shader) {
  GLint infoLogLength;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

  GLchar *strInfoLog = (GLchar *)malloc(infoLogLength + 1);
  glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

  fprintf(stderr, "%d\n", infoLogLength);

  fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
}

void setup_buffers() {
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)NULL);
}

void setup_shaders() {
  v_shader = glCreateShader(GL_VERTEX_SHADER);
  f_shader = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar *vshader_text = strdup(vs);
  const GLchar *fshader_text = strdup(fs);

  glShaderSource(v_shader, 1, &vshader_text, NULL);
  glShaderSource(f_shader, 1, &fshader_text, NULL);

  glCompileShader(v_shader);
  glCompileShader(f_shader);

  shader_program = glCreateProgram();

  glAttachShader(shader_program, v_shader);
  glAttachShader(shader_program, f_shader);
  glLinkProgram(shader_program);

  glUseProgram(shader_program);
}

void render_triangle() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)NULL);

  glUseProgram(shader_program);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
  GLFWwindow *window;

  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);

  window = glfwCreateWindow(1280, 720, "Example triangle.", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  printf("%s\n", glGetString(GL_VERSION));

  setup_buffers();
  setup_shaders();

  glViewport(0, 0, 1280, 720);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_triangle();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
