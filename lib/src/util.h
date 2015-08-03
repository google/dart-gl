// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

#include "GLES2/gl2.h"
#include "dart_api.h"

Dart_Handle HandleError(Dart_Handle handle);
Dart_Handle Dart_IntegerToUInt(Dart_Handle integer, unsigned int* value);
Dart_Handle Dart_NewStringFromGLubyteString(const GLubyte* string);

#endif  // SRC_UTIL_H_
