// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef DART_GL_LIB_SRC_MANUAL_BINDINGS_H_
#define DART_GL_LIB_SRC_MANUAL_BINDINGS_H_

#include "dart_api.h"

void glGetActiveAttrib_native(Dart_NativeArguments arguments);
void glGetActiveUniform_native(Dart_NativeArguments arguments);
void glGetAttachedShaders_native(Dart_NativeArguments arguments);
void glGetBooleanv_native(Dart_NativeArguments arguments);
void glGetBufferParameteriv_native(Dart_NativeArguments arguments);
void glGetFloatv_native(Dart_NativeArguments arguments);
void glGetFramebufferAttachmentParameteriv_native(
    Dart_NativeArguments arguments);
void glGetIntegerv_native(Dart_NativeArguments arguments);
void glGetProgramiv_native(Dart_NativeArguments arguments);
void glGetProgramInfoLog_native(Dart_NativeArguments arguments);
void glGetRenderbufferParameteriv_native(Dart_NativeArguments arguments);
void glGetShaderiv_native(Dart_NativeArguments arguments);
void glGetShaderInfoLog_native(Dart_NativeArguments arguments);
void glGetShaderPrecisionFormat_native(Dart_NativeArguments arguments);
void glGetShaderSource_native(Dart_NativeArguments arguments);
void glGetTexParameterfv_native(Dart_NativeArguments arguments);
void glGetTexParameteriv_native(Dart_NativeArguments arguments);
void glGetUniformfv_native(Dart_NativeArguments arguments);
void glGetUniformiv_native(Dart_NativeArguments arguments);
void glGetVertexAttribfv_native(Dart_NativeArguments arguments);
void glGetVertexAttribiv_native(Dart_NativeArguments arguments);
void glGetVertexAttribPointerv_native(Dart_NativeArguments arguments);
void glReadPixels_native(Dart_NativeArguments arguments);
void glShaderSource_native(Dart_NativeArguments arguments);
void glTexParameterfv_native(Dart_NativeArguments arguments);
void glTexParameteriv_native(Dart_NativeArguments arguments);
void glVertexAttribPointer_native(Dart_NativeArguments arguments);

#endif  // DART_GL_LIB_SRC_MANUAL_BINDINGS_H_
