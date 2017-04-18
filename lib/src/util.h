// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef DART_GL_LIB_SRC_UTIL_H_
#define DART_GL_LIB_SRC_UTIL_H_

#include "GLES2/gl2.h"
#include "dart_api.h"

Dart_Handle HandleError(Dart_Handle handle);
Dart_Handle Dart_IntegerToUInt(Dart_Handle integer, unsigned int* value);
Dart_Handle Dart_NewStringFromGLubyteString(const GLubyte* string);

// Fetches the length of the array of return values for a call to
// glGetBooleanv, glGetFloatv, or glGetIntegerv.
GLint GetGlGetResultLength(GLenum param);

#if defined(GL_TRACING)
#include "dart_tools_api.h"
#define TRACE_START(name)                                \
  Dart_TimelineEvent(#name, Dart_TimelineGetMicros(), 0, \
                     Dart_Timeline_Event_Begin, 0, NULL, NULL)
#define TRACE_END(name)                                  \
  Dart_TimelineEvent(#name, Dart_TimelineGetMicros(), 0, \
                     Dart_Timeline_Event_End, 0, NULL, NULL)
#else
#define TRACE_START(name) \
  do {                    \
  } while (0)
#define TRACE_END(name) \
  do {                  \
  } while (0)
#endif

#if defined(GL_TESTING)
#define HANDLE(handle) HandleError(handle)
#else
#define HANDLE(handle) handle
#endif

#endif  // DART_GL_LIB_SRC_UTIL_H_
