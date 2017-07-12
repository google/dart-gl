// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef DART_GL_LIB_SRC_INSTANTIATE_GL_CLASSES_H_
#define DART_GL_LIB_SRC_INSTANTIATE_GL_CLASSES_H_

#include <GLES2/gl2.h>
#include "dart_api.h"

Dart_Handle NewActiveInfo(GLint size, GLenum type, GLchar* name);
Dart_Handle NewShaderPrecisionFormat(GLint* range, GLint precision);

#endif  // DART_GL_LIB_SRC_INSTANTIATE_GL_CLASSES_H_
