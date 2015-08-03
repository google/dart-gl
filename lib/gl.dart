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

part 'src/generated/gl_constants.dart';
part 'src/generated/gl_native_functions.dart';
part 'src/gl_classes.dart';
part 'src/manual_bindings.dart';
