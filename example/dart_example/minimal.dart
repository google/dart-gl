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

  GLFWwindow window = glfwCreateWindow(1280, 720, "", null, null);

  glfwMakeContextCurrent(window);
  print(glGetString(GL_VERSION));

  var shader = glGetIntegerv(GL_CURRENT_PROGRAM);
  print('shader program: $shader');

  glfwTerminate();
}
