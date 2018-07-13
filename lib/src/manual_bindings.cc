// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <stdlib.h>

#include <GLES2/gl2.h>
#include "dart_api.h"

#include "gl_extension_info.h"
#include "instantiate_gl_classes.h"
#include "manual_bindings.h"
#include "util.h"

// This file contains native extension functions which have to be manually
// written due to the corresponding C functions returning values via pointer
// arguments or taking Dart Lists as arguments that need to be parsed to C
// arrays.

void glGetActiveAttrib_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetActiveAttrib_);
  Dart_Handle program_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  HANDLE(Dart_IntegerToInt64(program_obj, &program));

  Dart_Handle index_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t index = 0;
  HANDLE(Dart_IntegerToInt64(index_obj, &index));

  GLsizei bufSize;
  glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize);

  GLint size;
  GLenum type;
  GLchar *name = static_cast<GLchar *>(malloc(sizeof(GLchar) * bufSize));
  glGetActiveAttrib(program, index, bufSize, NULL, &size, &type, name);

  Dart_SetReturnValue(arguments, HANDLE(NewActiveInfo(size, type, name)));
  free(name);
  TRACE_END(glGetActiveAttrib_);
}

void glGetActiveUniform_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetActiveUniform_);
  Dart_Handle program_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  HANDLE(Dart_IntegerToInt64(program_obj, &program));

  Dart_Handle index_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t index = 0;
  HANDLE(Dart_IntegerToInt64(index_obj, &index));

  GLsizei bufSize;
  glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);

  GLint size;
  GLenum type;
  GLchar *name = static_cast<GLchar *>(malloc(sizeof(GLchar) * bufSize));
  glGetActiveUniform(program, index, bufSize, NULL, &size, &type, name);

  Dart_SetReturnValue(arguments, HANDLE(NewActiveInfo(size, type, name)));
  free(name);
  TRACE_END(glGetActiveUniform_);
}

void glGetAttachedShaders_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetAttachedShaders_);
  Dart_Handle program_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  HANDLE(Dart_IntegerToInt64(program_obj, &program));

  GLsizei maxCount;
  glGetProgramiv(program, GL_ATTACHED_SHADERS, &maxCount);
  GLuint *shaders = static_cast<GLuint *>(malloc(sizeof(GLuint) * maxCount));

  glGetAttachedShaders(program, maxCount, NULL, shaders);

  Dart_Handle shaders_obj = Dart_NewListOf(Dart_CoreType_Int, maxCount);
  for (int i = 0; i < maxCount; i++) {
    Dart_ListSetAt(shaders_obj, i, Dart_NewInteger(shaders[i]));
  }
  Dart_SetReturnValue(arguments, shaders_obj);
  TRACE_END(glGetAttachedShaders_);
}

void glGetBooleanv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetBooleanv_);
  int64_t pname = 0;
  HANDLE(Dart_GetNativeIntegerArgument(arguments, 0, &pname));

  int length = GetGlGetResultLength(pname);

  auto core_lib = GlExtensionInfo::current().core_library();
  Dart_Handle bool_type = HandleError(
      Dart_GetType(core_lib, Dart_NewStringFromCString("bool"), 0, nullptr));
  Dart_Handle list;
  if (length > 0) {
    list = HANDLE(Dart_NewListOfType(bool_type, length));
    GLboolean *data = (GLboolean *)malloc(sizeof(GLboolean) * length);
    glGetBooleanv(pname, data);
    for (int i = 0; i < length; i++) {
      Dart_Handle item = HANDLE(Dart_NewBoolean((bool)data[i]));
      HANDLE(Dart_ListSetAt(list, (intptr_t)i, item));
    }
    free(data);
  } else {
    list = HANDLE(Dart_NewListOfType(bool_type, 0));
  }
  Dart_SetReturnValue(arguments, list);
  TRACE_END(glGetBooleanv_);
}

void glGetBufferParameteriv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetBufferParameteriv_);
  Dart_Handle target_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  HANDLE(Dart_IntegerToInt64(target_obj, &target));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLint params;
  glGetBufferParameteriv(target, pname, &params);
  Dart_SetReturnValue(arguments, HANDLE(Dart_NewInteger(params)));
  TRACE_END(glGetBufferParameteriv_);
}

void glGetFloatv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetFloatv_);
  int64_t pname = 0;
  HANDLE(Dart_GetNativeIntegerArgument(arguments, 0, &pname));

  int length = GetGlGetResultLength(pname);

  auto core_lib = GlExtensionInfo::current().core_library();
  Dart_Handle double_type = HandleError(
      Dart_GetType(core_lib, Dart_NewStringFromCString("double"), 0, nullptr));
  Dart_Handle list;
  if (length > 0) {
    list = HANDLE(Dart_NewListOfType(double_type, length));
    GLfloat *data = (GLfloat *)malloc(sizeof(GLfloat) * length);
    glGetFloatv(pname, data);
    for (int i = 0; i < length; i++) {
      Dart_Handle item = HANDLE(Dart_NewDouble((double)data[i]));
      HANDLE(Dart_ListSetAt(list, (intptr_t)i, item));
    }
    free(data);
  } else {
    list = HANDLE(Dart_NewListOfType(double_type, 0));
  }
  Dart_SetReturnValue(arguments, list);
  TRACE_END(glGetFloatv_);
}

void glGetFramebufferAttachmentParameteriv_native(
    Dart_NativeArguments arguments) {
  TRACE_START(glGetFramebufferAttachmentParameteriv_);
  Dart_Handle target_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  HANDLE(Dart_IntegerToInt64(target_obj, &target));

  Dart_Handle attachment_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t attachment = 0;
  HANDLE(Dart_IntegerToInt64(attachment_obj, &attachment));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 2));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLint params;
  glGetFramebufferAttachmentParameteriv(target, attachment, pname, &params);
  Dart_SetReturnValue(arguments, HANDLE(Dart_NewInteger(params)));
  TRACE_END(glGetFramebufferAttachmentParameteriv_);
}

void glGetIntegerv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetIntegerv_);
  int64_t pname = 0;
  HANDLE(Dart_GetNativeIntegerArgument(arguments, 0, &pname));

  int length = GetGlGetResultLength(pname);

  Dart_Handle list;
  if (length > 0) {
    list = HANDLE(Dart_NewListOf(Dart_CoreType_Int, length));
    GLint *data = (GLint *)malloc(sizeof(GLint) * length);
    glGetIntegerv(pname, data);
    for (int i = 0; i < length; i++) {
      Dart_Handle item = HANDLE(Dart_NewInteger((int64_t)data[i]));
      HANDLE(Dart_ListSetAt(list, (intptr_t)i, item));
    }
    free(data);
  } else {
    list = HANDLE(Dart_NewListOf(Dart_CoreType_Int, 0));
  }
  Dart_SetReturnValue(arguments, list);
  TRACE_END(glGetIntegerv_);
}

void glGetProgramiv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetProgramiv_);
  Dart_Handle program_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  HANDLE(Dart_IntegerToInt64(program_obj, &program));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLint params;
  glGetProgramiv(program, pname, &params);

  Dart_SetReturnValue(arguments, Dart_NewInteger(params));
  TRACE_END(glGetProgramiv_);
}

void glGetProgramInfoLog_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetProgramInfoLog_);
  Dart_Handle program_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  HANDLE(Dart_IntegerToInt64(program_obj, &program));

  GLsizei maxSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxSize);

  char *infoLog = static_cast<char *>(malloc(sizeof(char) * maxSize));
  glGetProgramInfoLog(program, maxSize, NULL, infoLog);

  Dart_SetReturnValue(arguments, HANDLE(Dart_NewStringFromCString(infoLog)));
  free(infoLog);
  TRACE_END(glGetProgramInfoLog_);
}

void glGetRenderbufferParameteriv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetProgramInfoLog_);
  Dart_Handle target_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  HANDLE(Dart_IntegerToInt64(target_obj, &target));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLint params;
  glGetRenderbufferParameteriv(target, pname, &params);
  Dart_SetReturnValue(arguments, HANDLE(Dart_NewInteger(params)));
  TRACE_END(glGetProgramInfoLog_);
}

void glGetShaderiv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetShaderiv_);
  Dart_Handle shader_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t shader = 0;
  HANDLE(Dart_IntegerToInt64(shader_obj, &shader));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLint params;
  glGetShaderiv(shader, pname, &params);

  Dart_SetReturnValue(arguments, Dart_NewInteger(params));
  TRACE_END(glGetShaderiv_);
}

void glGetShaderInfoLog_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetShaderInfoLog_);
  Dart_Handle shader_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t shader = 0;
  HANDLE(Dart_IntegerToInt64(shader_obj, &shader));

  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

  char *infoLog = static_cast<char *>(malloc(sizeof(char) * bufSize));
  glGetShaderInfoLog(shader, bufSize, NULL, infoLog);

  Dart_SetReturnValue(arguments, HANDLE(Dart_NewStringFromCString(infoLog)));
  free(infoLog);
  TRACE_END(glGetShaderInfoLog_);
}

void glGetShaderPrecisionFormat_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetShaderPrecisionFormat_);
  Dart_Handle shadertype_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t shadertype = 0;
  HANDLE(Dart_IntegerToInt64(shadertype_obj, &shadertype));

  Dart_Handle precisiontype_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t precisiontype = 0;
  HANDLE(Dart_IntegerToInt64(precisiontype_obj, &precisiontype));

  GLint range[2] = {0, 0};
  GLint precision = 0;

  glGetShaderPrecisionFormat(shadertype, precisiontype, range, &precision);

  Dart_SetReturnValue(arguments,
                      HANDLE(NewShaderPrecisionFormat(range, precision)));
  TRACE_END(glGetShaderPrecisionFormat_);
}

void glGetShaderSource_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetShaderSource_);
  Dart_Handle shader_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t shader = 0;
  HANDLE(Dart_IntegerToInt64(shader_obj, &shader));

  GLint bufSize;
  glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &bufSize);

  char *source = static_cast<char *>(malloc(sizeof(char) * bufSize));
  GLsizei length;
  glGetShaderSource(shader, bufSize, &length, source);

  Dart_SetReturnValue(arguments, HANDLE(Dart_NewStringFromCString(source)));
  free(source);
  TRACE_END(glGetShaderSource_);
}

void glGetTexParameterfv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetTexParameterfv_);
  Dart_Handle target_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  HANDLE(Dart_IntegerToInt64(target_obj, &target));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLfloat params;
  glGetTexParameterfv(target, pname, &params);

  Dart_SetReturnValue(arguments, HANDLE(Dart_NewDouble(params)));
  TRACE_END(glGetTexParameterfv_);
}

void glGetTexParameteriv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetTexParameteriv_);
  Dart_Handle target_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  HANDLE(Dart_IntegerToInt64(target_obj, &target));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLint params;
  glGetTexParameteriv(target, pname, &params);

  Dart_SetReturnValue(arguments, HANDLE(Dart_NewInteger(params)));
  TRACE_END(glGetTexParameteriv_);
}

void glGetUniformfv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetTexParameteriv_);
  Dart_Handle program_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  HANDLE(Dart_IntegerToInt64(program_obj, &program));
  Dart_Handle location_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t location = 0;
  HANDLE(Dart_IntegerToInt64(location_obj, &location));

  GLfloat params;
  glGetUniformfv(program, location, &params);
  Dart_SetReturnValue(arguments, HANDLE(Dart_NewDouble(params)));
  TRACE_END(glGetTexParameteriv_);
}

void glGetUniformiv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetUniformiv_);
  Dart_Handle program_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t program = 0;
  HANDLE(Dart_IntegerToInt64(program_obj, &program));

  Dart_Handle location_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t location = 0;
  HANDLE(Dart_IntegerToInt64(location_obj, &location));

  GLint params;
  glGetUniformiv(program, location, &params);
  Dart_SetReturnValue(arguments, HANDLE(Dart_NewInteger(params)));
  TRACE_END(glGetUniformiv_);
}

void glGetVertexAttribfv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetVertexAttribfv_);
  Dart_Handle index_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t index = 0;
  HANDLE(Dart_IntegerToInt64(index_obj, &index));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLfloat params;
  glGetVertexAttribfv(index, pname, &params);

  Dart_SetReturnValue(arguments, HANDLE(Dart_NewDouble(params)));
  TRACE_END(glGetVertexAttribfv_);
}

void glGetVertexAttribiv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetVertexAttribiv_);
  Dart_Handle index_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t index = 0;
  HANDLE(Dart_IntegerToInt64(index_obj, &index));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLint params;
  glGetVertexAttribiv(index, pname, &params);
  Dart_SetReturnValue(arguments, HANDLE(Dart_NewInteger(params)));
  TRACE_END(glGetVertexAttribiv_);
}

void glGetVertexAttribPointerv_native(Dart_NativeArguments arguments) {
  TRACE_START(glGetVertexAttribPointerv_);
  Dart_Handle index_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t index = 0;
  HANDLE(Dart_IntegerToInt64(index_obj, &index));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  GLvoid *pointer;
  glGetVertexAttribPointerv(index, pname, &pointer);

  Dart_SetReturnValue(
      arguments, HANDLE(Dart_NewInteger(reinterpret_cast<int64_t>(pointer))));
  TRACE_END(glGetVertexAttribPointerv_);
}

void glReadPixels_native(Dart_NativeArguments arguments) {
  TRACE_START(glReadPixels_);
  Dart_Handle x_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t x = 0;
  HANDLE(Dart_IntegerToInt64(x_obj, &x));

  Dart_Handle y_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t y = 0;
  HANDLE(Dart_IntegerToInt64(y_obj, &y));

  Dart_Handle width_obj = HANDLE(Dart_GetNativeArgument(arguments, 2));

  int64_t width = 0;
  HANDLE(Dart_IntegerToInt64(width_obj, &width));

  Dart_Handle height_obj = HANDLE(Dart_GetNativeArgument(arguments, 3));

  int64_t height = 0;
  HANDLE(Dart_IntegerToInt64(height_obj, &height));

  Dart_Handle format_obj = HANDLE(Dart_GetNativeArgument(arguments, 4));

  int64_t format = 0;
  HANDLE(Dart_IntegerToInt64(format_obj, &format));

  Dart_Handle type_obj = HANDLE(Dart_GetNativeArgument(arguments, 5));

  int64_t type = 0;
  HANDLE(Dart_IntegerToInt64(type_obj, &type));

  Dart_Handle pixels_obj = HANDLE(Dart_GetNativeArgument(arguments, 6));

  void *pixels_data = NULL;
  Dart_TypedData_Type pixels_typeddata_type = Dart_TypedData_kInvalid;
  intptr_t pixels_typeddata_length = 0;
  if (!Dart_IsNull(pixels_obj)) {
    HANDLE(Dart_TypedDataAcquireData(pixels_obj, &pixels_typeddata_type,
                                     &pixels_data, &pixels_typeddata_length));
  }
  void *pixels = static_cast<void *>(pixels_data);

  glReadPixels(x, y, width, height, format, type, pixels);

  if (!Dart_IsNull(pixels_obj)) {
    Dart_TypedDataReleaseData(pixels_obj);
  }
  TRACE_END(glReadPixels_);
}

void glShaderSource_native(Dart_NativeArguments arguments) {
  TRACE_START(glShaderSource_);
  Dart_Handle shader_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t shader = 0;
  HANDLE(Dart_IntegerToInt64(shader_obj, &shader));

  Dart_Handle string_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  const char *string = NULL;
  HANDLE(Dart_StringToCString(string_obj, &string));

  glShaderSource(shader, 1, &string, NULL);
  TRACE_END(glShaderSource_);
}

void glTexParameterfv_native(Dart_NativeArguments arguments) {
  TRACE_START(glTexParameterfv_);
  Dart_Handle target_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  HANDLE(Dart_IntegerToInt64(target_obj, &target));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  Dart_Handle params_obj = HANDLE(Dart_GetNativeArgument(arguments, 2));
  GLfloat *params = NULL;
  if (!Dart_IsNull(params_obj)) {
    intptr_t length;
    HANDLE(Dart_ListLength(params_obj, &length));

    params = static_cast<GLfloat *>(malloc(sizeof(GLfloat) * length));

    for (int i = 0; i < length; i++) {
      double ith;
      HANDLE(Dart_DoubleValue(HANDLE(Dart_ListGetAt(params_obj, i)), &ith));
      params[i] = static_cast<GLfloat>(ith);
    }
  }
  glTexParameterfv(target, pname, params);
  free(params);
  TRACE_END(glTexParameterfv_);
}

void glTexParameteriv_native(Dart_NativeArguments arguments) {
  TRACE_START(glTexParameteriv_);
  Dart_Handle target_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t target = 0;
  HANDLE(Dart_IntegerToInt64(target_obj, &target));

  Dart_Handle pname_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t pname = 0;
  HANDLE(Dart_IntegerToInt64(pname_obj, &pname));

  Dart_Handle params_obj = HANDLE(Dart_GetNativeArgument(arguments, 2));
  GLint *params = NULL;
  if (!Dart_IsNull(params_obj)) {
    intptr_t length;
    HANDLE(Dart_ListLength(params_obj, &length));

    params = static_cast<GLint *>(malloc(sizeof(GLint) * length));

    for (int i = 0; i < length; i++) {
      int64_t ith;
      HANDLE(Dart_IntegerToInt64(HANDLE(Dart_ListGetAt(params_obj, i)), &ith));
      params[i] = static_cast<GLint>(ith);
    }
  }
  glTexParameteriv(target, pname, params);
  free(params);
  TRACE_END(glTexParameteriv_);
}

void glVertexAttribPointer_native(Dart_NativeArguments arguments) {
  TRACE_START(glVertexAttribPointer_);
  Dart_Handle index_obj = HANDLE(Dart_GetNativeArgument(arguments, 0));

  int64_t index = 0;
  HANDLE(Dart_IntegerToInt64(index_obj, &index));

  Dart_Handle size_obj = HANDLE(Dart_GetNativeArgument(arguments, 1));

  int64_t size = 0;
  HANDLE(Dart_IntegerToInt64(size_obj, &size));

  Dart_Handle type_obj = HANDLE(Dart_GetNativeArgument(arguments, 2));

  int64_t type = 0;
  HANDLE(Dart_IntegerToInt64(type_obj, &type));

  Dart_Handle normalized_obj = HANDLE(Dart_GetNativeArgument(arguments, 3));

  bool normalized = 0;
  HANDLE(Dart_BooleanValue(normalized_obj, &normalized));

  Dart_Handle stride_obj = HANDLE(Dart_GetNativeArgument(arguments, 4));

  int64_t stride = 0;
  HANDLE(Dart_IntegerToInt64(stride_obj, &stride));

  Dart_Handle offset_obj = HANDLE(Dart_GetNativeArgument(arguments, 5));

  int64_t offset = 0;
  HANDLE(Dart_IntegerToInt64(offset_obj, &offset));
  const void *pointer = reinterpret_cast<const void *>(offset);

  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  TRACE_END(glVertexAttribPointer_);
}
