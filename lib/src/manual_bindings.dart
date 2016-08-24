// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

part of gl;

/// Delete a [List] of buffer objects
void glDeleteBuffers(int n, List<int> buffers) native "glDeleteBuffers";

/// Delete a [List] of framebuffer objects
void glDeleteFramebuffers(int n, List<int> framebuffers)
    native "glDeleteFramebuffers";

/// Delete a [List] of renderbuffer objects
void glDeleteRenderbuffers(int n, List<int> renderbuffers)
    native "glDeleteRenderbuffers";

/// Delete a [List] of texture objects
void glDeleteTextures(int n, List<int> textures) native "glDeleteTextures";

/// Generate a [List] of buffer objects
List<int> glGenBuffers(int n) native "glGenBuffers";

/// Generate a [List] of framebuffer objects
List<int> glGenFramebuffers(int n) native "glGenFramebuffers";

/// Generate a [List] of renderbuffer objects
List<int> glGenRenderbuffers(int n) native "glGenRenderbuffers";

/// Generate a [List] of texture objects
List<int> glGenTextures(int n) native "glGenTextures";

/// Create an [ActiveInfo] object for the given [program] and [index]
ActiveInfo glGetActiveAttrib(int program, int index) native "glGetActiveAttrib";

/// Create an [ActiveInfo] object for the given [program] and [index]
ActiveInfo glGetActiveUniform(int program, int index)
    native "glGetActiveUniform";

/// Returns a list of shaders attached to [program]
List<int> glGetAttachedShaders(int program) native "glGetAttachedShaders";

/// Returns the [bool] result of [glGetBooleanv]
bool glGetBooleanv(int pname) native "glGetBooleanv";

/// Returns the [int] result of [glGetBufferParameteriv]
int glGetBufferParameteriv(int target, int pname)
    native "glGetBufferParameteriv";

/// Returns the [double] result of [glGetFloatv]
double glFloatv(int pname) native "glGetFloatv";

/// Returns the [int] result of [glGetFramebufferAttachmentParameteriv]
int glGetFramebufferAttachmentParameteriv(int target, int attachment, int pname)
    native "glGetFramebufferAttachmentParameteriv";

/// Returns the [int] result of [glGetIntegerv]
int glGetIntegerv(int pname) native "glGetIntegerv";

/// Return the programiv as an [int]
int glGetProgramiv(int program, int pname) native "glGetProgramiv";

/// Return a [String] containing the info log for [program]
String glGetProgramInfoLog(int program) native "glGetProgramInfoLog";

/// Returns the [int] result of [glGetRenderbufferParameteriv]
int glGetRenderbufferParameteriv(int target, int pname)
    native "glGetRenderbufferParameteriv";

/// Return the shaderiv as an [int]
int glGetShaderiv(int shader, int pname) native "glGetShaderiv";

/// Return a [String] containing the info log for [shader]
String glGetShaderInfoLog(int shader) native "glGetShaderInfoLog";

/// Returns a [ShaderPrecisionFormat] describing the range and precision for
/// different shader numeric formats
ShaderPrecisionFormat glGetShaderPrecisionFormat(
    int shadertype, int precisiontype) native "glGetShaderPrecisionFormat";

/// Return the GLSL source code of [shader]
String glGetShaderSource(int shader) native "glGetShaderSource";

/// Returns the [double] result of [glGetTexParameterfv]
double glGetTexParameterfv(int target, int pname) native "glGetTexParameterfv";

/// Returns the [int] result of [glGetTexParameteriv]
int glGetTexParameteriv(int target, int pname) native "glGetTexParameteriv";

/// Returns the [double] result of [glGetUniformfv]
double glGetUniformfv(int program, int location) native "glGetUniformfv";

/// Returns the [int] result of [glGetUniformiv]
int glGetUniformiv(int program, int location) native "glGetUniformiv";

/// Returns the [double] result of [glGetVertexAttribfv]
double glGetVertexAttribfv(int index, int pname) native "glGetVertexAttribfv";

/// Returns the [int] result of [glGetAttribiv]
int glGetVertexAttribiv(int index, int pname) native "glGetVertexAttribiv";

/// Returns the address (i.e. offset in bytes of the first vertex attribute of
/// the array at [index]) of the specified generic vertex attribute pointer
int glGetVertexAttribPointerv(int index, int pname)
    native "glGetVertexAttribPointerv";

/// Reads a block of pixels from the framebuffer into the [TypedData] [pixels] parameter
void glReadPixels(int x, int y, int width, int height, int format, int type,
    TypedData pixels) native "glReadPixels";

/// Upload a GLSL shader in [string] to the GPU for compilation
void glShaderSource(int shader, String string) native "glShaderSource";

/// Set texture float parameters
void glTexParameterfv(int target, int pname, List<double> params)
    native "glTexParameterfv";

/// Set texture integer parameters
void glTexParameteriv(int target, int pname, List<int> params)
    native "glTexParameteriv";

/// Specifies the layout of data associated with a vertex attribute.
///
/// Note that in the underlying C-language call, [offset] is a void pointer
/// representing the offset of the attribute data within the buffer.  In this
/// Dart implementation, we represent the same offset as an integer number of
/// bytes.
void glVertexAttribPointer(int index, int size, int type, bool normalized,
    int stride, int offset) native "glVertexAttribPointer";
