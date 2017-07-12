// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef DART_GL_LIB_SRC_GL_EXTENSION_H_
#define DART_GL_LIB_SRC_GL_EXTENSION_H_

#include "dart_api.h"

extern Dart_Handle GLLibrary;

Dart_NativeFunction ResolveName(Dart_Handle name, int argc,
                                bool* auto_setup_scope);

DART_EXPORT Dart_Handle gl_extension_Init(Dart_Handle parent_library);

#endif  // DART_GL_LIB_SRC_GL_EXTENSION_H_
