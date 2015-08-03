# Copyright (c) 2015, the Dart GLFW extension authors. All rights reserved.
# Please see the AUTHORS file for details. Use of this source code is governed
# by a BSD-style license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd
"""Default templates for the DefaultExtensionGenerator.

These really don't have a reason to be overridden. You should define your own
templates for other types or cases.
"""

DART_CONSTANT_TEMPLATE = """const {0} {1} = {2};"""
DART_DECLARATION_TEMPLATE = (
    """{return_type} {function_name}({arguments}) native "{extension_function}";"""
)

C_EXTENSION_DECLARATION_TEMPLATE = (
    """void {function_name}(Dart_NativeArguments arguments);"""
)

ARGUMENT_HANDLE_TEMPLATE = (
    """Dart_Handle {argument_name}_obj = HandleError(Dart_GetNativeArgument(arguments, {i}));
"""
)

INT_HANDLE_TO_C_VAR_TEMPLATE = (
    """
  int64_t {argument_name} = 0;
  if (Dart_IsInteger({argument_name}_obj)) {{
    HandleError(Dart_IntegerToInt64({argument_name}_obj, &{argument_name}));
  }}
"""
)

DOUBLE_HANDLE_TO_C_VAR_TEMPLATE = (
    """
  double {argument_name} = 0.0;
  if (Dart_IsDouble({argument_name}_obj)) {{
    HandleError(Dart_DoubleValue({argument_name}_obj, &{argument_name}));
  }}
"""
)

BOOL_HANDLE_TO_C_VAR_TEMPLATE = (
    """
  bool {argument_name} = 0;
  if (Dart_IsBoolean({argument_name}_obj)) {{
    HandleError(Dart_BooleanValue({argument_name}_obj, &{argument_name}));
  }}
"""
)

STRING_HANDLE_TO_C_VAR_TEMPLATE = (
    """
  const char* {argument_name} = NULL;
  if (Dart_IsString({argument_name}_obj)) {{
    HandleError(Dart_StringToCString({argument_name}_obj, &{argument_name}));
  }}
"""
)

TYPEDDATA_HANDLE_TO_C_VAR_TEMPLATE = (
    """
  void* {argument_name}_data = NULL;
  Dart_TypedData_Type {argument_name}_typeddata_type = Dart_TypedData_kInvalid;
  intptr_t {argument_name}_typeddata_length = 0;
  if (Dart_IsTypedData({argument_name}_obj) && !Dart_IsNull({argument_name}_obj)) {{
    HandleError(Dart_TypedDataAcquireData({argument_name}_obj, &{argument_name}_typeddata_type, &{argument_name}_data, &{argument_name}_typeddata_length));
  }}
  {argument_type} {argument_name} = static_cast<{argument_type}>({argument_name}_data);
"""
)

TYPEDDATA_RELEASE_TEMPLATE = (
    """
  if (Dart_IsTypedData({argument_name}_obj) && !Dart_IsNull({argument_name}_obj)) {{
    HandleError(Dart_TypedDataReleaseData({argument_name}_obj));
  }}"""
)

C_EXTENSION_DEFINITION_TEMPLATE = (
    """void {extension_function_name}(Dart_NativeArguments arguments) {{
  {get_argument_handles}
  {function_return}
  {free_allocated_memory}
}}
"""
)

C_EXTENSION_RETURN_TEMPLATE = (
    """{return_variable}{function_name}({arguments});
  {set_dart_return}"""
)

C_EXTENSION_RETURN_STATEMENT_TEMPLATE = ("""{ret_type} ret = """)

C_EXTENSION_SET_RETURN_TEMPLATE = (
    """Dart_SetReturnValue(arguments, HandleError({return_handle}));"""
)
