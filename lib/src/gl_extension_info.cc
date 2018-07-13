// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "gl_extension_info.h"

// Create GlExtensionInfo for each new library instance.
void GlExtensionInfo::create(Dart_Handle gl_library, Dart_Handle core_library) {
  std::lock_guard<std::mutex> guard(info_for_isolate_mutex_);
  info_for_isolate_[Dart_CurrentIsolate()] =
      new GlExtensionInfo(gl_library, core_library);
}

// Query GlExtensionInfo for the current context.
GlExtensionInfo& GlExtensionInfo::current() {
  return *info_for_isolate_.at(Dart_CurrentIsolate());
}

GlExtensionInfo::GlExtensionInfo(Dart_Handle gl_library,
                                 Dart_Handle core_library)
    : gl_library_(gl_library), core_library_(core_library) {}

std::map<Dart_Isolate, GlExtensionInfo*> GlExtensionInfo::info_for_isolate_;
std::mutex GlExtensionInfo::info_for_isolate_mutex_;
