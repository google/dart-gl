// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <stdlib.h>

#include "GLES2/gl2.h"
#include "dart_api.h"

#include "manual_bindings.h"

#include "instantiate_gl_classes.h"
#include "util.h"

// This file contains native extension functions which have to be manually
// written due to the corresponding C functions returning values via pointer
// arguments or taking Dart Lists as arguments that need to be parsed to C
// arrays.

void glDeleteBuffers_native(Dart_NativeArguments arguments) {
  Dart_Handle n_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t n = 0;
  if (Dart_IsInteger(n_obj)) {
    HandleError(Dart_IntegerToInt64(n_obj, &n));
  }

  Dart_Handle buffers_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  GLuint *buffers = NULL;
  if (Dart_IsList(buffers_obj)) {
    buffers = static_cast<GLuint *>(malloc(sizeof(GLuint) * n));
    for (int i = 0; i < n; i++) {
      Dart_Handle i_obj = HandleError(Dart_ListGetAt(buffers_obj, i));

      HandleError(Dart_IntegerToUInt(i_obj, &buffers[i]));
    }
  }
  glDeleteBuffers(n, buffers);
  free(buffers);
}

void glDeleteFramebuffers_native(Dart_NativeArguments arguments) {
  Dart_Handle n_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t n = 0;
  if (Dart_IsInteger(n_obj)) {
    HandleError(Dart_IntegerToInt64(n_obj, &n));
  }

  Dart_Handle framebuffers_obj =
      HandleError(Dart_GetNativeArgument(arguments, 1));

  GLuint *framebuffers = NULL;
  if (Dart_IsList(framebuffers_obj)) {
    framebuffers = static_cast<GLuint *>(malloc(sizeof(GLuint) * n));
    for (int i = 0; i < n; i++) {
      Dart_Handle i_obj = HandleError(Dart_ListGetAt(framebuffers_obj, i));

      HandleError(Dart_IntegerToUInt(i_obj, &framebuffers[i]));
    }
  }
  glDeleteFramebuffers(n, framebuffers);
  free(framebuffers);
}

void glDeleteRenderbuffers_native(Dart_NativeArguments arguments) {
  Dart_Handle n_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t n = 0;
  if (Dart_IsInteger(n_obj)) {
    HandleError(Dart_IntegerToInt64(n_obj, &n));
  }

  Dart_Handle renderbuffers_obj =
      HandleError(Dart_GetNativeArgument(arguments, 1));

  GLuint *renderbuffers = NULL;
  if (Dart_IsList(renderbuffers_obj)) {
    renderbuffers = static_cast<GLuint *>(malloc(sizeof(GLuint) * n));
    for (int i = 0; i < n; i++) {
      Dart_Handle i_obj = HandleError(Dart_ListGetAt(renderbuffers_obj, i));

      HandleError(Dart_IntegerToUInt(i_obj, &renderbuffers[i]));
    }
  }
  glDeleteRenderbuffers(n, renderbuffers);
  free(renderbuffers);
}

void glDeleteTextures_native(Dart_NativeArguments arguments) {
  Dart_Handle n_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t n = 0;
  if (Dart_IsInteger(n_obj)) {
    HandleError(Dart_IntegerToInt64(n_obj, &n));
  }

  Dart_Handle textures_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  GLuint *textures = NULL;
  if (Dart_IsList(textures_obj)) {
    textures = static_cast<GLuint *>(malloc(sizeof(GLuint) * n));
    for (int i = 0; i < n; i++) {
      Dart_Handle i_obj = HandleError(Dart_ListGetAt(textures_obj, i));

      HandleError(Dart_IntegerToUInt(i_obj, &textures[i]));
    }
  }
  glDeleteTextures(n, textures);
  free(textures);
}

void glGenBuffers_native(Dart_NativeArguments arguments) {
  Dart_Handle n_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t n = 0;
  if (Dart_IsInteger(n_obj)) {
    HandleError(Dart_IntegerToInt64(n_obj, &n));
  }

  GLuint *buffers = static_cast<GLuint *>(malloc(sizeof(GLuint) * n));
  glGenBuffers(n, buffers);

  Dart_Handle buffers_obj = Dart_NewList(n);
  for (int i = 0; i < n; i++) {
    Dart_Handle i_obj = HandleError(Dart_NewInteger(buffers[i]));
    HandleError(Dart_ListSetAt(buffers_obj, i, i_obj));
  }
  Dart_SetReturnValue(arguments, buffers_obj);
  free(buffers);
}

void glGenFramebuffers_native(Dart_NativeArguments arguments) {
  Dart_Handle n_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t n = 0;
  if (Dart_IsInteger(n_obj)) {
    HandleError(Dart_IntegerToInt64(n_obj, &n));
  }

  GLuint *framebuffers = static_cast<GLuint *>(malloc(sizeof(GLuint) * n));
  glGenFramebuffers(n, framebuffers);

  Dart_Handle framebuffers_obj = Dart_NewList(n);
  for (int i = 0; i < n; i++) {
    Dart_Handle i_obj = HandleError(Dart_NewInteger(framebuffers[i]));
    HandleError(Dart_ListSetAt(framebuffers_obj, i, i_obj));
  }
  Dart_SetReturnValue(arguments, framebuffers_obj);
  free(framebuffers);
}

void glGenRenderbuffers_native(Dart_NativeArguments arguments) {
  Dart_Handle n_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t n = 0;
  if (Dart_IsInteger(n_obj)) {
    HandleError(Dart_IntegerToInt64(n_obj, &n));
  }

  GLuint *renderbuffers = static_cast<GLuint *>(malloc(sizeof(GLuint) * n));
  glGenRenderbuffers(n, renderbuffers);

  Dart_Handle renderbuffers_obj = Dart_NewList(n);
  for (int i = 0; i < n; i++) {
    Dart_Handle i_obj = HandleError(Dart_NewInteger(renderbuffers[i]));
    HandleError(Dart_ListSetAt(renderbuffers_obj, i, i_obj));
  }
  Dart_SetReturnValue(arguments, renderbuffers_obj);
  free(renderbuffers);
}

void glGenTextures_native(Dart_NativeArguments arguments) {
  Dart_Handle n_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t n = 0;
  if (Dart_IsInteger(n_obj)) {
    HandleError(Dart_IntegerToInt64(n_obj, &n));
  }

  GLuint *textures = static_cast<GLuint *>(malloc(sizeof(GLuint) * n));
  glGenTextures(n, textures);

  Dart_Handle textures_obj = Dart_NewList(n);
  for (int i = 0; i < n; i++) {
    Dart_Handle i_obj = HandleError(Dart_NewInteger(textures[i]));
    HandleError(Dart_ListSetAt(textures_obj, i, i_obj));
  }
  Dart_SetReturnValue(arguments, textures_obj);
  free(textures);
}

void glGetActiveAttrib_native(Dart_NativeArguments arguments) {
  Dart_Handle program_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  if (Dart_IsInteger(program_obj)) {
    HandleError(Dart_IntegerToInt64(program_obj, &program));
  }

  Dart_Handle index_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t index = 0;
  if (Dart_IsInteger(index_obj)) {
    HandleError(Dart_IntegerToInt64(index_obj, &index));
  }

  GLsizei bufSize;
  glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize);

  GLint size;
  GLenum type;
  GLchar *name = static_cast<GLchar *>(malloc(sizeof(GLchar) * bufSize));
  glGetActiveAttrib(program, index, bufSize, NULL, &size, &type, name);

  Dart_SetReturnValue(arguments, HandleError(NewActiveInfo(size, type, name)));
  free(name);
}

void glGetActiveUniform_native(Dart_NativeArguments arguments) {
  Dart_Handle program_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  if (Dart_IsInteger(program_obj)) {
    HandleError(Dart_IntegerToInt64(program_obj, &program));
  }

  Dart_Handle index_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t index = 0;
  if (Dart_IsInteger(index_obj)) {
    HandleError(Dart_IntegerToInt64(index_obj, &index));
  }

  GLsizei bufSize;
  glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);

  GLint size;
  GLenum type;
  GLchar *name = static_cast<GLchar *>(malloc(sizeof(GLchar) * bufSize));
  glGetActiveUniform(program, index, bufSize, NULL, &size, &type, name);

  Dart_SetReturnValue(arguments, HandleError(NewActiveInfo(size, type, name)));
  free(name);
}

void glGetAttachedShaders_native(Dart_NativeArguments arguments) {
  Dart_Handle program_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  if (Dart_IsInteger(program_obj)) {
    HandleError(Dart_IntegerToInt64(program_obj, &program));
  }

  GLsizei maxCount;
  glGetProgramiv(program, GL_ATTACHED_SHADERS, &maxCount);
  GLuint *shaders = static_cast<GLuint *>(malloc(sizeof(GLuint) * maxCount));

  glGetAttachedShaders(program, maxCount, NULL, shaders);

  Dart_Handle shaders_obj = Dart_NewList(maxCount);
  for (int i = 0; i < maxCount; i++) {
    Dart_ListSetAt(shaders_obj, i, Dart_NewInteger(shaders[i]));
  }
  Dart_SetReturnValue(arguments, shaders_obj);
}

void glGetBooleanv_native(Dart_NativeArguments arguments) {
  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLboolean data;
  glGetBooleanv(pname, &data);
  Dart_SetReturnValue(arguments,
                      HandleError(Dart_NewBoolean(static_cast<bool>(data))));
}

void glGetBufferParameteriv_native(Dart_NativeArguments arguments) {
  Dart_Handle target_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  if (Dart_IsInteger(target_obj)) {
    HandleError(Dart_IntegerToInt64(target_obj, &target));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLint params;
  glGetBufferParameteriv(target, pname, &params);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(params)));
}

void glGetFloatv_native(Dart_NativeArguments arguments) {
  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLfloat data;
  glGetFloatv(pname, &data);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewDouble(data)));
}

void glGetFramebufferAttachmentParameteriv_native(
    Dart_NativeArguments arguments) {
  Dart_Handle target_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  if (Dart_IsInteger(target_obj)) {
    HandleError(Dart_IntegerToInt64(target_obj, &target));
  }

  Dart_Handle attachment_obj =
      HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t attachment = 0;
  if (Dart_IsInteger(attachment_obj)) {
    HandleError(Dart_IntegerToInt64(attachment_obj, &attachment));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 2));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLint params;
  glGetFramebufferAttachmentParameteriv(target, attachment, pname, &params);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(params)));
}

void glGetIntegerv_native(Dart_NativeArguments arguments) {
  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLint data;
  glGetIntegerv(pname, &data);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(data)));
}

void glGetProgramiv_native(Dart_NativeArguments arguments) {
  Dart_Handle program_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  if (Dart_IsInteger(program_obj)) {
    HandleError(Dart_IntegerToInt64(program_obj, &program));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLint params;
  glGetProgramiv(program, pname, &params);

  Dart_SetReturnValue(arguments, Dart_NewInteger(params));
}

void glGetProgramInfoLog_native(Dart_NativeArguments arguments) {
  Dart_Handle program_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  if (Dart_IsInteger(program_obj)) {
    HandleError(Dart_IntegerToInt64(program_obj, &program));
  }

  GLsizei maxSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxSize);

  char *infoLog = static_cast<char *>(malloc(sizeof(char) * maxSize));
  glGetProgramInfoLog(program, maxSize, NULL, infoLog);

  Dart_SetReturnValue(arguments,
                      HandleError(Dart_NewStringFromCString(infoLog)));
  free(infoLog);
}

void glGetRenderbufferParameteriv_native(Dart_NativeArguments arguments) {
  Dart_Handle target_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  if (Dart_IsInteger(target_obj)) {
    HandleError(Dart_IntegerToInt64(target_obj, &target));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLint params;
  glGetRenderbufferParameteriv(target, pname, &params);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(params)));
}

void glGetShaderiv_native(Dart_NativeArguments arguments) {
  Dart_Handle shader_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t shader = 0;
  if (Dart_IsInteger(shader_obj)) {
    HandleError(Dart_IntegerToInt64(shader_obj, &shader));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLint params;
  glGetShaderiv(shader, pname, &params);

  Dart_SetReturnValue(arguments, Dart_NewInteger(params));
}

void glGetShaderInfoLog_native(Dart_NativeArguments arguments) {
  Dart_Handle shader_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t shader = 0;
  if (Dart_IsInteger(shader_obj)) {
    HandleError(Dart_IntegerToInt64(shader_obj, &shader));
  }

  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

  char *infoLog = static_cast<char *>(malloc(sizeof(char) * bufSize));
  glGetShaderInfoLog(shader, bufSize, NULL, infoLog);

  Dart_SetReturnValue(arguments,
                      HandleError(Dart_NewStringFromCString(infoLog)));
  free(infoLog);
}

void glGetShaderPrecisionFormat_native(Dart_NativeArguments arguments) {
  Dart_Handle shadertype_obj =
      HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t shadertype = 0;
  if (Dart_IsInteger(shadertype_obj)) {
    HandleError(Dart_IntegerToInt64(shadertype_obj, &shadertype));
  }

  Dart_Handle precisiontype_obj =
      HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t precisiontype = 0;
  if (Dart_IsInteger(precisiontype_obj)) {
    HandleError(Dart_IntegerToInt64(precisiontype_obj, &precisiontype));
  }

  GLint range[2] = {0, 0};
  GLint precision = 0;

  glGetShaderPrecisionFormat(shadertype, precisiontype, range, &precision);

  Dart_SetReturnValue(arguments,
                      HandleError(NewShaderPrecisionFormat(range, precision)));
}

void glGetShaderSource_native(Dart_NativeArguments arguments) {
  Dart_Handle shader_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t shader = 0;
  if (Dart_IsInteger(shader_obj)) {
    HandleError(Dart_IntegerToInt64(shader_obj, &shader));
  }

  GLint bufSize;
  glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &bufSize);

  char *source = static_cast<char *>(malloc(sizeof(char) * bufSize));
  GLsizei length;
  glGetShaderSource(shader, bufSize, &length, source);

  Dart_SetReturnValue(arguments,
                      HandleError(Dart_NewStringFromCString(source)));
  free(source);
}

void glGetTexParameterfv_native(Dart_NativeArguments arguments) {
  Dart_Handle target_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  if (Dart_IsInteger(target_obj)) {
    HandleError(Dart_IntegerToInt64(target_obj, &target));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLfloat params;
  glGetTexParameterfv(target, pname, &params);

  Dart_SetReturnValue(arguments, HandleError(Dart_NewDouble(params)));
}

void glGetTexParameteriv_native(Dart_NativeArguments arguments) {
  Dart_Handle target_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  if (Dart_IsInteger(target_obj)) {
    HandleError(Dart_IntegerToInt64(target_obj, &target));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLint params;
  glGetTexParameteriv(target, pname, &params);

  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(params)));
}

void glGetUniformfv_native(Dart_NativeArguments arguments) {
  Dart_Handle program_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  if (Dart_IsInteger(program_obj)) {
    HandleError(Dart_IntegerToInt64(program_obj, &program));
  }

  Dart_Handle location_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t location = 0;
  if (Dart_IsInteger(location_obj)) {
    HandleError(Dart_IntegerToInt64(location_obj, &location));
  }

  GLfloat params;
  glGetUniformfv(program, location, &params);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewDouble(params)));
}

void glGetUniformiv_native(Dart_NativeArguments arguments) {
  Dart_Handle program_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  if (Dart_IsInteger(program_obj)) {
    HandleError(Dart_IntegerToInt64(program_obj, &program));
  }

  Dart_Handle location_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t location = 0;
  if (Dart_IsInteger(location_obj)) {
    HandleError(Dart_IntegerToInt64(location_obj, &location));
  }

  GLint params;
  glGetUniformiv(program, location, &params);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(params)));
}

void glGetVertexAttribfv_native(Dart_NativeArguments arguments) {
  Dart_Handle index_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t index = 0;
  if (Dart_IsInteger(index_obj)) {
    HandleError(Dart_IntegerToInt64(index_obj, &index));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLfloat params;
  glGetVertexAttribfv(index, pname, &params);

  Dart_SetReturnValue(arguments, HandleError(Dart_NewDouble(params)));
}

void glGetVertexAttribiv_native(Dart_NativeArguments arguments) {
  Dart_Handle index_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t index = 0;
  if (Dart_IsInteger(index_obj)) {
    HandleError(Dart_IntegerToInt64(index_obj, &index));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLint params;
  glGetVertexAttribiv(index, pname, &params);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(params)));
}

void glGetVertexAttribPointerv_native(Dart_NativeArguments arguments) {
  Dart_Handle index_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t index = 0;
  if (Dart_IsInteger(index_obj)) {
    HandleError(Dart_IntegerToInt64(index_obj, &index));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  GLvoid *pointer;
  glGetVertexAttribPointerv(index, pname, &pointer);

  Dart_SetReturnValue(
      arguments,
      HandleError(Dart_NewInteger(reinterpret_cast<int64_t>(pointer))));
}

void glReadPixels_native(Dart_NativeArguments arguments) {
  Dart_Handle x_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t x = 0;
  if (Dart_IsInteger(x_obj)) {
    HandleError(Dart_IntegerToInt64(x_obj, &x));
  }

  Dart_Handle y_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t y = 0;
  if (Dart_IsInteger(y_obj)) {
    HandleError(Dart_IntegerToInt64(y_obj, &y));
  }

  Dart_Handle width_obj = HandleError(Dart_GetNativeArgument(arguments, 2));

  int64_t width = 0;
  if (Dart_IsInteger(width_obj)) {
    HandleError(Dart_IntegerToInt64(width_obj, &width));
  }

  Dart_Handle height_obj = HandleError(Dart_GetNativeArgument(arguments, 3));

  int64_t height = 0;
  if (Dart_IsInteger(height_obj)) {
    HandleError(Dart_IntegerToInt64(height_obj, &height));
  }

  Dart_Handle format_obj = HandleError(Dart_GetNativeArgument(arguments, 4));

  int64_t format = 0;
  if (Dart_IsInteger(format_obj)) {
    HandleError(Dart_IntegerToInt64(format_obj, &format));
  }

  Dart_Handle type_obj = HandleError(Dart_GetNativeArgument(arguments, 5));

  int64_t type = 0;
  if (Dart_IsInteger(type_obj)) {
    HandleError(Dart_IntegerToInt64(type_obj, &type));
  }

  Dart_Handle pixels_obj = HandleError(Dart_GetNativeArgument(arguments, 6));

  void *pixels_data = NULL;
  Dart_TypedData_Type pixels_typeddata_type = Dart_TypedData_kInvalid;
  intptr_t pixels_typeddata_length = 0;
  if (Dart_IsTypedData(pixels_obj) && !Dart_IsNull(pixels_obj)) {
    HandleError(Dart_TypedDataAcquireData(pixels_obj, &pixels_typeddata_type,
                                          &pixels_data,
                                          &pixels_typeddata_length));
  }
  void *pixels = static_cast<void *>(pixels_data);

  glReadPixels(x, y, width, height, format, type, pixels);

  if (Dart_IsTypedData(pixels_obj) && !Dart_IsNull(pixels_obj)) {
    Dart_TypedDataReleaseData(pixels_obj);
  }
}

void glShaderSource_native(Dart_NativeArguments arguments) {
  Dart_Handle shader_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t shader = 0;
  if (Dart_IsInteger(shader_obj)) {
    HandleError(Dart_IntegerToInt64(shader_obj, &shader));
  }

  Dart_Handle string_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  const char *string = NULL;
  if (Dart_IsString(string_obj)) {
    HandleError(Dart_StringToCString(string_obj, &string));
  }

  glShaderSource(shader, 1, &string, NULL);
}

void glTexParameterfv_native(Dart_NativeArguments arguments) {
  Dart_Handle target_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  if (Dart_IsInteger(target_obj)) {
    HandleError(Dart_IntegerToInt64(target_obj, &target));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  Dart_Handle params_obj = HandleError(Dart_GetNativeArgument(arguments, 2));
  GLfloat *params = NULL;
  if (Dart_IsList(params_obj) && !Dart_IsNull(params_obj)) {
    intptr_t length;
    HandleError(Dart_ListLength(params_obj, &length));

    params = static_cast<GLfloat *>(malloc(sizeof(GLfloat) * length));

    for (int i = 0; i < length; i++) {
      double ith;
      HandleError(
          Dart_DoubleValue(HandleError(Dart_ListGetAt(params_obj, i)), &ith));
      params[i] = static_cast<GLfloat>(ith);
    }
  }
  glTexParameterfv(target, pname, params);
  free(params);
}

void glTexParameteriv_native(Dart_NativeArguments arguments) {
  Dart_Handle target_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  if (Dart_IsInteger(target_obj)) {
    HandleError(Dart_IntegerToInt64(target_obj, &target));
  }

  Dart_Handle pname_obj = HandleError(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  if (Dart_IsInteger(pname_obj)) {
    HandleError(Dart_IntegerToInt64(pname_obj, &pname));
  }

  Dart_Handle params_obj = HandleError(Dart_GetNativeArgument(arguments, 2));
  GLint *params = NULL;
  if (Dart_IsList(params_obj) && !Dart_IsNull(params_obj)) {
    intptr_t length;
    HandleError(Dart_ListLength(params_obj, &length));

    params = static_cast<GLint *>(malloc(sizeof(GLint) * length));

    for (int i = 0; i < length; i++) {
      int64_t ith;
      HandleError(Dart_IntegerToInt64(
          HandleError(Dart_ListGetAt(params_obj, i)), &ith));
      params[i] = static_cast<GLint>(ith);
    }
  }
  glTexParameteriv(target, pname, params);
  free(params);
}
