// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef THIRD_PARTY_DART_GL_LIB_SRC_UTIL_H_
#define THIRD_PARTY_DART_GL_LIB_SRC_UTIL_H_

#include "third_party/GL/gl/include/GLES2/gl2.h"
#include "third_party/dart_lang/runtime/include/dart_api.h"

Dart_Handle HandleError(Dart_Handle handle);
Dart_Handle Dart_IntegerToUInt(Dart_Handle integer, unsigned int* value);
Dart_Handle Dart_NewStringFromGLubyteString(const GLubyte* string);

// Fetches the length of the array of return values for a call to
// glGetBooleanv, glGetFloatv, or glGetIntegerv.
GLint GetGlGetResultLength(GLenum param);

#endif  // THIRD_PARTY_DART_GL_LIB_SRC_UTIL_H_
