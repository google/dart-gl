// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

library gl;

/// A Dart library which uses a native extension to call OpenGL ES2 functions.
///
/// This library provides a wrapper for the OpenGL ES2 API.
///
/// Please see the README.md and the
/// [OpenGL ES2 man pages](https://www.khronos.org/opengles/sdk/docs/man/)
/// for more detail.

import 'dart-ext:gl_extension';
import 'dart:typed_data' show TypedData;

export 'src/generated/gl_constants.dart';

part 'src/generated/gl_native_functions.dart';
part 'src/manual_bindings.dart';

/// Contains data returned by [glGetActiveAttrib] or [glGetActiveUniform]
/// calls.
class ActiveInfo {
  /// Returns the size of the attribute variable.
  final int size;

  /// Returns the data type of the attribute variable.
  final int type;

  /// Returns the [String] containing the name of the attribute variable.
  final String name;

  ActiveInfo(this.size, this.type, this.name);
}

/// Contains data returned by [glGetShaderPrecisionFormat] about the
/// range and precision for different shader numeric (floating point and
/// integer) shader variable formats.
class ShaderPrecisionFormat {
  /// The precision of the format.
  final int precision;

  /// The maximum representable magnitude of the format.
  final int rangeMax;

  /// The minimum representable magnitude of the format.
  final int rangeMin;
  ShaderPrecisionFormat(this.rangeMin, this.rangeMax, this.precision);
}
