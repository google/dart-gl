# Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
# Please see the AUTHORS file for details. Use of this source code is governed
# by a BSD-style license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd
"""Test cases for the DefaultExtensionGenerator.

Test various forms of the DefaultExtensionGenerator.
"""

import unittest
from extension_generator import DefaultExtensionGenerator

TEST_VOID_DEF = """void test_native(Dart_NativeArguments arguments) {
  test();
}
"""

TEST_INT_RET_DEF = """void test_native(Dart_NativeArguments arguments) {
  int ret = test();
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(ret)));
}
"""

TEST_INT_ARG_DEF = """void test_native(Dart_NativeArguments arguments) {
  Dart_Handle arg1_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t arg1 = 0;
  if (Dart_IsInteger(arg1_obj)) {
    HandleError(Dart_IntegerToInt64(arg1_obj, &arg1));
  }

  test(arg1);
}
"""

TEST_INT_ARG_ALIAS_DEF = """void test_native(Dart_NativeArguments arguments) {
  Dart_Handle arg1_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t arg1 = 0;
  if (Dart_IsInteger(arg1_obj)) {
    HandleError(Dart_IntegerToInt64(arg1_obj, &arg1));
  }

  test(arg1);
}
"""

TEST_INT_RET_ALIAS_DEF = """void test_native(Dart_NativeArguments arguments) {
  myInt ret = test();
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(ret)));
}
"""


TEST_BOOL_RET_INT_ARG_DEF = """void test_native(Dart_NativeArguments arguments) {
  Dart_Handle arg1_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t arg1 = 0;
  if (Dart_IsInteger(arg1_obj)) {
    HandleError(Dart_IntegerToInt64(arg1_obj, &arg1));
  }

  bool ret = test(arg1);
  Dart_SetReturnValue(arguments, HandleError(Dart_NewBoolean(ret)));
}
"""

TEST_BOOL_HINT_RET_DEF = """void testBool_native(Dart_NativeArguments arguments) {
  int ret = testBool();
  Dart_SetReturnValue(arguments, HandleError(Dart_NewBoolean(ret)));
}
"""

TEST_UPDATE_TYPE_MAP_RET_DEF = """void test_native(Dart_NativeArguments arguments) {
  int64_t ret = test();
  Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(ret)));
}
"""

TEST_UPDATE_TYPE_MAP_ARG_DEF = """void test_native(Dart_NativeArguments arguments) {
  Dart_Handle arg1_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t arg1 = 0;
  if (Dart_IsInteger(arg1_obj)) {
    HandleError(Dart_IntegerToInt64(arg1_obj, &arg1));
  }

  test(arg1);
}
"""

TEST_UPDATE_HANDLE_TO_C_MAP_ARG_DEF = """void test_native(Dart_NativeArguments arguments) {
  Dart_Handle fakeTypeArg1_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  FakeType* fakeTypeArg1 = NULL;
  if (!Dart_IsNull(fakeTypeArg1_obj)) {
    HandleError(Dart_GetNativeInstanceField(fakeTypeArg1_obj, 0, reinterpret_cast<intptr_t *>(&fakeTypeArg1)));
  }

  test(fakeTypeArg1);
}
"""

TEST_UPDATE_NEW_HANDLE_MAP_RET_DEF = """void test_native(Dart_NativeArguments arguments) {
  Dart_Handle arg1_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  int64_t arg1 = 0;
  if (Dart_IsInteger(arg1_obj)) {
    HandleError(Dart_IntegerToInt64(arg1_obj, &arg1));
  }

  fakeType* ret = test(arg1);
  Dart_SetReturnValue(arguments, HandleError(NewFakeType(ret)));
}
"""

TEST_FREE_TYPEDDATA_DEF = """void test_native(Dart_NativeArguments arguments) {
  Dart_Handle data_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  void* data_data = NULL;
  Dart_TypedData_Type data_typeddata_type = Dart_TypedData_kInvalid;
  intptr_t data_typeddata_length = 0;
  if (Dart_IsTypedData(data_obj) && !Dart_IsNull(data_obj)) {
    HandleError(Dart_TypedDataAcquireData(data_obj, &data_typeddata_type, &data_data, &data_typeddata_length));
  }
  const void* data = static_cast<const void*>(data_data);

  test(data);
  if (Dart_IsTypedData(data_obj) && !Dart_IsNull(data_obj)) {
    HandleError(Dart_TypedDataReleaseData(data_obj));
  }
}
"""

TEST_FREE_CUSTOM_TYPE_DEF = """void test_native(Dart_NativeArguments arguments) {
  Dart_Handle fakeStructArg1_obj = HandleError(Dart_GetNativeArgument(arguments, 0));

  FakeStruct* fakeStructArg1 = NULL;
  if (!Dart_IsNull(fakeStructArg1_obj)) {
    fakeStructArg1 = NewFakeStructFromDart(fakeStructArg1_obj);
  }

  test(fakeStructArg1);

  if (fakeStructArg1 != NULL) {
    free(fakeStructArg1->fakeMember);
    free(fakeStructArg1);
  }
}
"""

FAKETYPE_HANDLE_TO_C_TEMPLATE = """
  FakeType* {argument_name} = NULL;
  if (!Dart_IsNull({argument_name}_obj)) {{
    HandleError(Dart_GetNativeInstanceField({argument_name}_obj, 0, reinterpret_cast<intptr_t *>(&{argument_name})));
  }}
"""

FAKESTRUCT_HANDLE_TO_C_TEMPLATE = """
  FakeStruct* {argument_name} = NULL;
  if (!Dart_IsNull({argument_name}_obj)) {{
    {argument_name} = NewFakeStructFromDart({argument_name}_obj);
  }}
"""

FAKESTRUCT_FREE_TEMPLATE = """

  if ({argument_name} != NULL) {{
    free({argument_name}->fakeMember);
    free({argument_name});
  }}"""


class TestDefaultExtensionGenerator(unittest.TestCase):
  """TestCase subclass fot the DefaultExtensionGenerator.

  Does not test that the generated functions actually compile.
  """

  maxDiff = None

  def assertDefEqual(self, gen_def, test_def):
    """Assert if two function definitions are equal."""
    self.assertMultiLineEqual(gen_def, test_def)

  def test_instantiate(self):
    """Test instantiating a DefaultExtensionGenerator with empty parameters."""
    gen = DefaultExtensionGenerator(list(), list())
    gen.generate_bindings()

    self.assertIsInstance(gen, DefaultExtensionGenerator)
    self.assertEqual(len(gen.dart_constants), 0)
    self.assertEqual(len(gen.c_functions), 0)

  def test_constant(self):
    """Test parsing a single constant."""
    gen = DefaultExtensionGenerator(["#define CONSTANT 0x1234",], list())
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_constants), 1)
    self.assertEqual(gen.dart_constants[0], "const int CONSTANT = 0x1234;")

  def test_void(self):
    """Test parsing a void function with a void parameter."""
    gen = DefaultExtensionGenerator(list(), ["void test(void);",])
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_constants), 0)

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "void test() native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0], TEST_VOID_DEF)

  def test_int_return(self):
    """Test parsing a function that returns an int."""
    gen = DefaultExtensionGenerator(list(), ["int test(void);",])
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_constants), 0)

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "int test() native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0], TEST_INT_RET_DEF)

  def test_int_arg(self):
    """Test parsing a function that takes an int argument."""
    gen = DefaultExtensionGenerator(list(), ["void test(int arg1);",])
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_constants), 0)

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "void test(int arg1) native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0], TEST_INT_ARG_DEF)

  def test_int_arg_alias(self):
    """Test parsing a function that takes a typedef'd int argument."""
    gen = DefaultExtensionGenerator(list(), ["void test(myInt arg1);",])
    gen.alias_map.update({"myInt": "int"})
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_constants), 0)

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "void test(int arg1) native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0], TEST_INT_ARG_ALIAS_DEF)

  def test_int_ret_alias(self):
    """Test parsing a function that returns a typedef'd int argument."""
    gen = DefaultExtensionGenerator(list(), ["myInt test(void);",])
    gen.alias_map.update({"myInt": "int"})
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_constants), 0)

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "int test() native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0], TEST_INT_RET_ALIAS_DEF)

  def test_bool_return_int_arg(self):
    """Test parsing a function that returns a bool and takes an int argument."""
    gen = DefaultExtensionGenerator(list(), ["bool test(int arg1);",])
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_constants), 0)

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "bool test(int arg1) native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0],
                        TEST_BOOL_RET_INT_ARG_DEF)

  def test_int_bool_hint_return(self):
    """Test parsing a function that returns an int, but hinted to a bool.

    Use the ret_hint_map parameter to return a bool instead of an int only for
    the function testBool.
    """
    gen = DefaultExtensionGenerator(list(), ["int test(void);",
                                             "int testBool(void);"])
    gen.ret_hint_map.update({"testBool": "bool"})
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_constants), 0)

    self.assertEqual(len(gen.dart_extension_declarations), 2)
    self.assertEqual(len(gen.c_extension_declarations), 2)
    self.assertEqual(len(gen.c_extension_definitions), 2)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "int test() native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0], TEST_INT_RET_DEF)

    self.assertEqual(gen.dart_extension_declarations[1],
                     "bool testBool() native \"testBool\";")
    self.assertEqual(gen.c_extension_declarations[1],
                     "void testBool_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[1], TEST_BOOL_HINT_RET_DEF)

  def test_need_manual_binding_return(self):
    """Test parsing a function with a return type that requires manual binding.
    """
    gen = DefaultExtensionGenerator(list(), ["fakeType test(void);",])
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_extension_declarations), 0)
    self.assertEqual(len(gen.c_extension_declarations), 0)
    self.assertEqual(len(gen.c_extension_definitions), 0)

    self.assertEqual(len(gen.needs_manual_binding_functions), 1)

  def test_need_manual_binding_arg(self):
    """Test parsing a function with an arg type that requires manual binding.
    """
    gen = DefaultExtensionGenerator(list(),
                                    ["void test(int arg1, fakeType arg2);",])
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_extension_declarations), 0)
    self.assertEqual(len(gen.c_extension_declarations), 0)
    self.assertEqual(len(gen.c_extension_definitions), 0)

    self.assertEqual(len(gen.needs_manual_binding_functions), 1)

  def test_update_type_map_ret(self):
    """Test parsing a function with a user-defined return type.

    Test parsing a function with a user-defined return type that maps to an
    already-known type.
    """
    gen = DefaultExtensionGenerator(list(), ["int64_t test(void);",])
    gen.type_map.update({"int64_t": "int"})
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(len(gen.needs_manual_binding_functions), 0)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "int test() native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0],
                        TEST_UPDATE_TYPE_MAP_RET_DEF)

  def test_update_type_map_arg(self):
    """Test parsing a function with a user-defined argument type.

    Test parsing a function with a user-defined argument type that maps to an
    already-known type.
    """
    gen = DefaultExtensionGenerator(list(), ["void test(int64_t arg1);",])
    gen.type_map.update({"int64_t": "int"})
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(len(gen.needs_manual_binding_functions), 0)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "void test(int arg1) native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0],
                        TEST_UPDATE_TYPE_MAP_ARG_DEF)

  def test_update_new_handle_map(self):
    """Test returning user-defined Dart classes from extension functions.

    In order to return a custom type, it must be in the type_map and the
    new_handle_map.
    """
    gen = DefaultExtensionGenerator(list(), ["fakeType* test(int arg1);",])
    gen.type_map.update({"fakeType*": "FakeType"})
    gen.new_handle_map.update({"fakeType*": "NewFakeType({ret})"})
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(len(gen.needs_manual_binding_functions), 0)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "FakeType test(int arg1) native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0],
                        TEST_UPDATE_NEW_HANDLE_MAP_RET_DEF)

  def test_update_handle_to_c_map(self):
    """Test passing user-defined classes to extension functions.

    In order to take in a custom class as a Dart argument, it must be in the
    type_map and the handle_to_c_map.
    """
    gen = DefaultExtensionGenerator(
        list(), ["void test(fakeType* fakeTypeArg1);",])
    gen.type_map.update({"fakeType*": "FakeType"})
    gen.handle_to_c_map.update({"FakeType": FAKETYPE_HANDLE_TO_C_TEMPLATE})
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(len(gen.needs_manual_binding_functions), 0)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "void test(FakeType fakeTypeArg1) native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0],
                        TEST_UPDATE_HANDLE_TO_C_MAP_ARG_DEF)

  def test_free_TypedData(self):
    """Test acquiring and freeing TypedData handles.

    In order to use TypedData handles, a hint is usually required.
    """
    gen = DefaultExtensionGenerator(list(), ["void test(const void* data);",])
    gen.arg_hint_map.update({"const void* data": "TypedData"})
    gen.generate_bindings()

    self.assertEqual(len(gen.needs_manual_binding_functions), 0)

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(
        gen.dart_extension_declarations[0],
        "void test(TypedData data) native \"test\";")
    self.assertEqual(
        gen.c_extension_declarations[0],
        "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0], TEST_FREE_TYPEDDATA_DEF)

  def test_free_custom_type(self):
    """Test passing a user-defined type that needs to be freed.

    In this case we are passing in a Dart class, creating a matching C struct
    from
    the class's data, and passing that C struct into a C function.
    Sometimes this will require us to free memory after passing into the C
    function.
    """
    gen = DefaultExtensionGenerator(
        list(), ["void test(fakeStruct* fakeStructArg1);",])
    gen.type_map.update({"fakeStruct*": "FakeStruct"})
    gen.handle_to_c_map.update({"FakeStruct": FAKESTRUCT_HANDLE_TO_C_TEMPLATE})
    gen.needs_free_map.update({"FakeStruct": FAKESTRUCT_FREE_TEMPLATE})
    gen.generate_bindings()

    self.assertEqual(len(gen.dart_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_declarations), 1)
    self.assertEqual(len(gen.c_extension_definitions), 1)

    self.assertEqual(len(gen.needs_manual_binding_functions), 0)

    self.assertEqual(gen.dart_extension_declarations[0],
                     "void test(FakeStruct fakeStructArg1) native \"test\";")
    self.assertEqual(gen.c_extension_declarations[0],
                     "void test_native(Dart_NativeArguments arguments);")
    self.assertDefEqual(gen.c_extension_definitions[0],
                        TEST_FREE_CUSTOM_TYPE_DEF)


if __name__ == "__main__":
  suite = unittest.TestLoader().loadTestsFromTestCase(
      TestDefaultExtensionGenerator)
  unittest.TextTestRunner(verbosity=2).run(suite)
