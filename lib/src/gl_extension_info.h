// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef DART_GL_LIB_SRC_GL_EXTENSION_INFO_H_
#define DART_GL_LIB_SRC_GL_EXTENSION_INFO_H_

#include <map>
#include <mutex>

#include "dart_api.h"

// Extra data needed for this extension in each isolate.
class GlExtensionInfo {
 public:
  static void create(Dart_Handle gl_library);

  // Returns extra info associated with this library for the Dart isolate
  // in which it is executing.
  static GlExtensionInfo& current();

  const Dart_Handle& gl_library() const { return gl_library_; }

 private:
  GlExtensionInfo(Dart_Handle gl_library);

  // Info unique to this isolate:
  Dart_Handle gl_library_;

  static std::map<Dart_Isolate, GlExtensionInfo*> info_for_isolate_;
  static std::mutex info_for_isolate_mutex_;
};

#endif  // DART_GL_LIB_SRC_GL_EXTENSION_INFO_H_
