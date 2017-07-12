// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

import 'package:gl/gl.dart';
import 'package:glfw/glfw.dart';

void main() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);

  GLFWwindow window = glfwCreateWindow(100, 100, "", null, null);

  glfwMakeContextCurrent(window);
  print(glGetString(GL_VERSION));
  print("Bits: r:${glGetIntegerv(GL_RED_BITS).single} "
      "g:${glGetIntegerv(GL_GREEN_BITS).single} "
      "b:${glGetIntegerv(GL_BLUE_BITS).single} "
      "a:${glGetIntegerv(GL_ALPHA_BITS).single}");
  print("      depth:${glGetIntegerv(GL_DEPTH_BITS).single} "
      "stencil:${glGetIntegerv(GL_STENCIL_BITS).single}");
  var writemask = glGetBooleanv(GL_COLOR_WRITEMASK);
  print("Color Writemask: r:${writemask[0]} g:${writemask[1]} "
      "b:${writemask[2]} a:${writemask[3]}");
  var viewport = glGetIntegerv(GL_VIEWPORT);
  print("Viewport: x:${viewport[0]} y:${viewport[1]} "
      "w:${viewport[2]} h:${viewport[3]}");
  var depthRange = glGetFloatv(GL_DEPTH_RANGE);
  var intDepthRange = glGetIntegerv(GL_DEPTH_RANGE);
  print("Depth range: ${depthRange[0]} - ${depthRange[1]} "
      "(${intDepthRange[0]} - ${intDepthRange[1]})");
  print("");
  print("Max Texture Units:");
  print("  Vertex:   ${glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS)}");
  print("  Fragment: ${glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS)}");
  print("  Combined: ${glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)}");
  print("Max Uniform Vectors:");
  print("  Vertex:   ${glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS)}");
  print("  Fragment: ${glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS)}");
  print("Max Vertex Attributes: ${glGetIntegerv(GL_MAX_VERTEX_ATTRIBS)}");
  print("Max Varying Vectors:   ${glGetIntegerv(GL_MAX_VARYING_VECTORS)}");
  print("");
  var formats = glGetIntegerv(GL_SHADER_BINARY_FORMATS)
      .map((fmt) => "0x${fmt.toRadixString(16)}")
      .toList();
  print("Shader Binary Formats: $formats");
  formats = glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS)
      .map((fmt) => "0x${fmt.toRadixString(16)}")
      .toList();
  print("Compressed Texture Formats: $formats");

  var empty = glGetIntegerv(0xf00d);
  if (empty.isNotEmpty) {
    throw new ArgumentError("ERROR! Got data for an invalid GLenum!");
  }

  var extensions = glGetString(GL_EXTENSIONS).split(' ');
  print("Extensions:");
  for (var extension in extensions) {
    print("  $extension");
  }
  glfwSetWindowShouldClose(window, GL_TRUE);
  glfwTerminate();
}
