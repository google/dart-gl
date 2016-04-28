// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

// This file contains utility functions for the GL Dart native extension.

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "GLES2/gl2.h"

#include "util.h"

Dart_Handle HandleError(Dart_Handle handle) {
  if (Dart_IsError(handle)) {
    Dart_PropagateError(Dart_NewUnhandledExceptionError(handle));
  }
  return handle;
}

Dart_Handle Dart_IntegerToUInt(Dart_Handle integer, unsigned int *value) {
  int64_t actual;
  HandleError(Dart_IntegerToInt64(integer, &actual));

  if (actual < UINT_MAX) {
    *value = static_cast<unsigned int>(actual);
    return Dart_True();
  } else {
    char buf[50];  // Technically we only need 46 characters for this.
    snprintf(buf, sizeof(buf), "%ld does not fit into an unsigned int.",
             actual);
    return Dart_NewApiError(buf);
  }
}

Dart_Handle Dart_NewStringFromGLubyteString(const GLubyte *string) {
  return Dart_NewStringFromCString(reinterpret_cast<const char *>(string));
}
