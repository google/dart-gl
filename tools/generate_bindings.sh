#!/bin/bash
#
# Copyright (c) 2015, the Dart GLFW extension authors. All rights reserved.
# Please see the AUTHORS file for details. Use of this source code is governed
# by a BSD-style license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd

[ -d generated ] || mkdir generated/
python gl_generator.py $1
clang-format -i -style=Google generated/*.{cc,h}
dartfmt -w generated/*.dart
