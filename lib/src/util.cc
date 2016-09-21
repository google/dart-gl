// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

// This file contains utility functions for the GL Dart native extension.

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "third_party/GL/gl/include/GLES2/gl2.h"
#include "third_party/dart_lang/runtime/include/dart_api.h"

#include "third_party/dart/gl/lib/src/util.h"

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

// The length of the resulting array of values for a call to glGetBooleanv,
// glGetFloatv, or glGetIntegerv with the given parameter.  A resultLength
// of 0 means that the list is variable length, and must be determined by
// another call to glGetIntegerv.
typedef struct {
  GLenum param;
  GLint resultLength;
} GetParamResultLength;

// Hash table of glGet*v parameters to the length of array of returned values.
static const int kResultLengthTableSize = 96;
static GetParamResultLength glGetResultLengths[] = {
    {GL_ACTIVE_TEXTURE, 1},
    {GL_MAX_VIEWPORT_DIMS, 2},
    {GL_FRAMEBUFFER_BINDING, 1},
    {GL_SHADER_COMPILER, 1},
    {GL_STENCIL_VALUE_MASK, 1},
    {GL_TEXTURE_BINDING_CUBE_MAP, 1},
    {GL_GENERATE_MIPMAP_HINT, 1},
    {GL_BLEND_COLOR, 4},
    {GL_CULL_FACE_MODE, 1},
    {GL_SAMPLE_COVERAGE_INVERT, 1},
    {GL_SAMPLE_ALPHA_TO_COVERAGE, 1},
    {GL_DEPTH_TEST, 1},
    {GL_STENCIL_WRITEMASK, 1},
    {0x0, 0},
    {GL_BLEND_DST_ALPHA, 1},
    {GL_BLEND_EQUATION_ALPHA, 1},
    {GL_STENCIL_TEST, 1},
    {GL_COMPRESSED_TEXTURE_FORMATS, 0},
    {GL_DEPTH_BITS, 1},
    {GL_MAX_VERTEX_ATTRIBS, 1},
    {GL_MAX_VARYING_VECTORS, 1},
    {GL_STENCIL_BACK_REF, 1},
    {GL_VIEWPORT, 4},
    {GL_ELEMENT_ARRAY_BUFFER_BINDING, 1},
    {GL_SAMPLE_BUFFERS, 1},
    {GL_STENCIL_PASS_DEPTH_FAIL, 1},
    {0x0, 0},
    {GL_STENCIL_BACK_FAIL, 1},
    {0x0, 0},
    {0x0, 0},
    {GL_IMPLEMENTATION_COLOR_READ_TYPE, 1},
    {GL_ALIASED_POINT_SIZE_RANGE, 2},
    {GL_CURRENT_PROGRAM, 1},
    {GL_DEPTH_CLEAR_VALUE, 1},
    {GL_GREEN_BITS, 1},
    {GL_NUM_SHADER_BINARY_FORMATS, 1},
    {GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, 1},
    {GL_POLYGON_OFFSET_UNITS, 1},
    {GL_SAMPLE_COVERAGE, 1},
    {GL_PACK_ALIGNMENT, 1},
    {GL_POLYGON_OFFSET_FACTOR, 1},
    {GL_STENCIL_BACK_WRITEMASK, 1},
    {GL_STENCIL_FUNC, 1},
    {GL_SCISSOR_TEST, 1},
    {GL_CULL_FACE, 1},
    {GL_STENCIL_REF, 1},
    {GL_SAMPLE_COVERAGE_VALUE, 1},
    {0x0, 0},
    {GL_DEPTH_RANGE, 2},
    {GL_COLOR_WRITEMASK, 4},
    {GL_DITHER, 1},
    {GL_BLEND_EQUATION_RGB, 1},
    {GL_BLEND_SRC_RGB, 1},
    {GL_STENCIL_BACK_PASS_DEPTH_PASS, 1},
    {GL_BLEND, 1},
    {GL_ALPHA_BITS, 1},
    {GL_NUM_COMPRESSED_TEXTURE_FORMATS, 1},
    {GL_MAX_VERTEX_UNIFORM_VECTORS, 1},
    {GL_SUBPIXEL_BITS, 1},
    {GL_TEXTURE_BINDING_2D, 1},
    {GL_ARRAY_BUFFER_BINDING, 1},
    {GL_RENDERBUFFER_BINDING, 1},
    {GL_STENCIL_FAIL, 1},
    {GL_UNPACK_ALIGNMENT, 1},
    {GL_STENCIL_BACK_FUNC, 1},
    {GL_MAX_TEXTURE_SIZE, 1},
    {GL_FRONT_FACE, 1},
    {0x0, 0},
    {0x0, 0},
    {0x0, 0},
    {GL_DEPTH_WRITEMASK, 1},
    {GL_MAX_TEXTURE_IMAGE_UNITS, 1},
    {GL_RED_BITS, 1},
    {GL_BLEND_SRC_ALPHA, 1},
    {GL_SHADER_BINARY_FORMATS, 0},
    {GL_STENCIL_CLEAR_VALUE, 1},
    {GL_STENCIL_BACK_VALUE_MASK, 1},
    {GL_POLYGON_OFFSET_FILL, 1},
    {GL_STENCIL_BITS, 1},
    {GL_MAX_FRAGMENT_UNIFORM_VECTORS, 1},
    {GL_SCISSOR_BOX, 4},
    {0x0, 0},
    {GL_STENCIL_PASS_DEPTH_PASS, 1},
    {GL_SAMPLES, 1},
    {GL_MAX_CUBE_MAP_TEXTURE_SIZE, 1},
    {0x0, 0},
    {GL_COLOR_CLEAR_VALUE, 4},
    {GL_STENCIL_BACK_PASS_DEPTH_FAIL, 1},
    {GL_BLEND_DST_RGB, 1},
    {GL_IMPLEMENTATION_COLOR_READ_FORMAT, 1},
    {GL_ALIASED_LINE_WIDTH_RANGE, 2},
    {GL_LINE_WIDTH, 1},
    {GL_BLUE_BITS, 1},
    {GL_DEPTH_FUNC, 1},
    {GL_MAX_RENDERBUFFER_SIZE, 1},
    {GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, 1},
};

// Looks up the number of values returned for a call to glGet*v in the hash
// table.  Hashing function is (param & 0xfff) * 251 % 96, with collisions
// handled by forward linear search (with wrap-around).
GLint GetGlGetResultLength(GLenum param) {
  // There are currently two parameters that are variable length, so the
  // length must be queried.
  GLint length = 0;
  if (param == GL_COMPRESSED_TEXTURE_FORMATS) {
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &length);
    return length;
  } else if (param == GL_SHADER_BINARY_FORMATS) {
    glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &length);
    return length;
  }

  // Otherwise, we'll look up the length in the hash table.
  int hash = param & 0xfff * 251 % kResultLengthTableSize;
  int index = hash;
  do {
    if (glGetResultLengths[index].param == param) {
      return glGetResultLengths[index].resultLength;
    }
    index = (index + 1) % kResultLengthTableSize;
  } while (index != hash);
  return -1;
}
