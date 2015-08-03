// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

// Functions to instantiate GL-specific classes from the C extension.
// Most of these classes are designed to mirror the WebGL interface.

#include <stdlib.h>

#include "GLES2/gl2.h"
#include "dart_api.h"

#include "gl_extension.h"
#include "util.h"

#include "instantiate_gl_classes.h"

Dart_Handle NewActiveInfo(GLint size, GLenum type, GLchar* name) {
  Dart_Handle ActiveInfo_type = HandleError(Dart_GetType(
      GLLibrary, Dart_NewStringFromCString("ActiveInfo"), 0, NULL));

  const int num_arguments = 3;
  Dart_Handle arguments[num_arguments];
  arguments[0] = Dart_NewInteger(size);
  arguments[1] = Dart_NewInteger(type);
  arguments[2] = Dart_NewStringFromCString(name);

  return Dart_New(ActiveInfo_type, Dart_Null(), num_arguments, arguments);
}

Dart_Handle NewShaderPrecisionFormat(GLint* range, GLint precision) {
  Dart_Handle ShaderPrecisionFormat_type = HandleError(Dart_GetType(
      GLLibrary, Dart_NewStringFromCString("ShaderPrecisionFormat"), 0, NULL));

  const int num_arguments = 3;
  Dart_Handle arguments[num_arguments];
  arguments[0] = Dart_NewInteger(range[0]);
  arguments[1] = Dart_NewInteger(range[1]);
  arguments[2] = Dart_NewInteger(precision);

  return Dart_New(ShaderPrecisionFormat_type, Dart_Null(), num_arguments,
                  arguments);
}
