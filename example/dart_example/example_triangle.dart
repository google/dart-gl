// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

import 'dart:io';

import 'dart:typed_data';

import 'package:gl/gl.dart';
import 'package:glfw/glfw.dart';

int vbo;

int v_shader, f_shader, shader_program;

Float32List verts = new Float32List.fromList(const [
  -1.0, -1.0, 1.0, 0.0, 0.0, 1.0, // x, y, r, g, b, a
  0.0, 1.0, 0.0, 1.0, 0.0, 1.0,
  1.0, -1.0, 0.0, 0.0, 1.0, 1.0,
]);
const int float32Size = Float32List.BYTES_PER_ELEMENT;

const String vshader_text = """
#version 100
attribute vec4 position;
attribute vec4 color;
varying vec4 vcolor;

void main() {
  gl_Position = position;
  vcolor = color;
}
""";

const String fshader_text = """
#version 100
precision mediump float;
varying vec4 vcolor;

void main() {
  gl_FragColor = vcolor;
}
""";

void check_error() {
  int err = glGetError();
  while (err != GL_NO_ERROR) {
    stderr.writeln("OpenGL error: $err");
    err = glGetError();
  }
}

void get_shader_error(int shader) {
  String strInfoLog = glGetShaderInfoLog(shader);
  if (strInfoLog.length > 0) {
    stderr.writeln("Compile failure in shader:\n$strInfoLog\n");

    String shaderSource = glGetShaderSource(shader);
    stderr.writeln("Shader source: \n$shaderSource\n");
  }
}

void get_program_error(int program) {
  String strInfoLog = glGetProgramInfoLog(program);
  if (strInfoLog.length > 0) {
    stderr.writeln("Program error:\n$strInfoLog\n");
  }
}

void setup_buffers() {
  vbo = glGenBuffers(1).first;

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(
      GL_ARRAY_BUFFER, verts.length * float32Size, verts, GL_STATIC_DRAW);

  check_error();
}

void setup_shaders() {
  v_shader = glCreateShader(GL_VERTEX_SHADER);
  f_shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(v_shader, vshader_text);
  glShaderSource(f_shader, fshader_text);

  glCompileShader(v_shader);
  get_shader_error(v_shader);

  glCompileShader(f_shader);
  get_shader_error(f_shader);
  check_error();

  shader_program = glCreateProgram();

  glAttachShader(shader_program, v_shader);
  glAttachShader(shader_program, f_shader);
  glLinkProgram(shader_program);
  get_program_error(shader_program);
  check_error();

  glUseProgram(shader_program);
  get_program_error(shader_program);
  check_error();
}

void render_triangle() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  var vertexId = glGetAttribLocation(shader_program, 'position');
  glEnableVertexAttribArray(vertexId);
  glVertexAttribPointer(
      vertexId, // Describe vertex information.
      2, // Two values: x, y
      GL_FLOAT, // Each has sizeof float.
      GL_FALSE, // Don't normalize values.
      6 * float32Size, // Stride to next vertex is 6 elements.
      0); // Offset to vertex info.

  var colorId = glGetAttribLocation(shader_program, 'color');
  glEnableVertexAttribArray(colorId);
  glVertexAttribPointer(
      colorId, // Describe color information.
      4, // Four values: r, g, b, a
      GL_FLOAT, // Each has sizeof float.
      GL_FALSE, // Don't normalize values.
      6 * float32Size, // Stride to next vertex is 6 elements.
      2 * float32Size); // Offset to vertex info (skip x,y).

  glUseProgram(shader_program);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  check_error();
}

void main() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);

  GLFWwindow window =
      glfwCreateWindow(1280, 720, "Hello Dart GLFW", null, null);

  glfwMakeContextCurrent(window);
  print(glGetString(GL_VERSION));

  setup_buffers();
  setup_shaders();

  glViewport(0, 0, 1280, 720);
  glClearColor(0.0, 0.0, 0.0, 1.0);

  while (!glfwWindowShouldCloseAsBool(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_triangle();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}
