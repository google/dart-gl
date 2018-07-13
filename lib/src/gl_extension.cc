// Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
// Please see the AUTHORS file for details. Use of this source code is governed
// by a BSD-style license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include <string.h>

#include "dart_api.h"

#include "generated/function_list.h"
#include "generated/gl_bindings.h"
#include "gl_extension.h"
#include "gl_extension_info.h"
#include "manual_bindings.h"
#include "util.h"

DART_EXPORT Dart_Handle gl_extension_Init(Dart_Handle parent_library) {
  if (Dart_IsError(parent_library)) {
    return parent_library;
  }

  Dart_Handle result_code =
      Dart_SetNativeResolver(parent_library, ResolveName, NULL);
  if (Dart_IsError(result_code)) {
    return result_code;
  }

  // Look up the GL library and stash it in the info for this isolate.
  auto gl_lib = HandleError(Dart_NewPersistentHandle(HandleError(
      Dart_LookupLibrary(Dart_NewStringFromCString("package:gl/gl.dart")))));
  auto core_lib = HandleError(Dart_NewPersistentHandle(
      HandleError(Dart_LookupLibrary(Dart_NewStringFromCString("dart:core")))));

  GlExtensionInfo::create(gl_lib, core_lib);

  loadFunctions();

  return Dart_Null();
}

Dart_NativeFunction ResolveName(Dart_Handle name, int argc,
                                bool* auto_setup_scope) {
  if (!Dart_IsString(name)) {
    return NULL;
  }
  Dart_NativeFunction result = NULL;

  const char* cname;
  HandleError(Dart_StringToCString(name, &cname));

  for (int i = 0; function_list[i].name != NULL; ++i) {
    if (strcmp(function_list[i].name, cname) == 0) {
      result = function_list[i].function;
      break;
    }
  }
  return result;
}
