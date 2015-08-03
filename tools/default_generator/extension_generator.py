# Copyright (c) 2015, the Dart GLFW extension authors. All rights reserved.
# Please see the AUTHORS file for details. Use of this source code is governed
# by a BSD-style license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd
"""Module containing the CFunction and the DefaultExtensionGenerator classes.

This module cannot be run by itself; you must write a script that creates
a DefaultExtensionGenerator object or a subclass and use those to write the
entire file. C headers need ifdef guards and includes, and Dart files need
`part of` statements. You may also want to include a license header,
and run a formatter over the generated code.
"""

from default_templates import *


def normalize_pointers(initial_type, initial_name):
  """Normalizes pointers to sit with the type.

  Examples:
    (int, *hello) -> (int*, hello)
    (const int, **const*hello) -> (const int**const, hello)

  cdecl.org says the second one means
  "declare hello as const pointer to pointer to const int"

  Args:
    initial_type: String with initial type
    initial_name: String with initial name
  Returns:
    A tuple with the normalized type and name (type, name)
  """
  while initial_name.startswith(("*", "&", "const*")):
    if initial_name.startswith("*"):
      initial_type += "*"
      initial_name = initial_name[1:]
    elif initial_name.startswith("&"):
      initial_type += "&"
      initial_name = initial_name[1:]
    elif initial_name.startswith("const*"):
      initial_type += "const*"
      initial_name = initial_name[6:]

  return (initial_type, initial_name)


class CFunction(object):
  """This class represents a C function used by the extension generators.

  The parsing is really really poor and will fail in all sorts of very simple
  cases. For example, it will fail just with `(int)my_function(int i);` because
  of the int in parenthesis.

  Attributes:
    c_return_type: The return type of the original C function
    c_arguments: The arguments of the original C function
    dart_ret_type: The return type of the corresponding Dart function
    dart_argument: The arguments of the corresponding Dart function
    needs_manual_bindings: Does this function have a return type or
        argument type that we cannot map?
    has_manual_bindings: Does this function have a manual binding in
        in the context of the current generator?
  """

  def __init__(
      self, c_def, type_map,
      ret_type_hint_map=None,
      arg_type_hint_map=None):
    self.function_name = str()

    self.c_ret_type = str()
    self.c_arguments = list()

    self.dart_ret_type = str()
    self.dart_arguments = list()

    self.needs_manual_bindings = False
    self.has_manual_bindings = False

    self.type_map = type_map
    self.ret_type_hint_map = ret_type_hint_map
    self.arg_type_hint_map = arg_type_hint_map

    self.c_ret_type = " ".join(c_def.split("(")[0].split()[:-1])
    self.function_name = c_def.split("(")[0].split()[-1]

    self.c_ret_type, self.function_name = normalize_pointers(self.c_ret_type,
                                                             self.function_name)

    self.extension_function_name = self.generate_extension_function_name(
        self.function_name)

    split_c_arguments = c_def.split("(")[1][:-2].split(", ")

    try:
      if self.function_name in self.ret_type_hint_map:
        self.dart_ret_type = self.type_map[
            self.ret_type_hint_map[self.function_name]
        ]
      else:
        self.dart_ret_type = self.type_map[self.c_ret_type]
    except KeyError:
      # there is some type we cannot handle, so this function's binding has to
      # be hand-written
      self.needs_manual_bindings = True

    for arg in split_c_arguments:
      try:
        arg_name = arg.split()[-1]
        arg_type = " ".join(arg.split()[:-1])
        arg_type, arg_name = normalize_pointers(arg_type, arg_name)

        self.c_arguments.append([arg_type, arg_name])

        if arg_name == "void":
          self.dart_arguments.append(["", "void"])
          break

        if " ".join([arg_type, arg_name]) in self.arg_type_hint_map:
          self.dart_arguments.append([self.arg_type_hint_map[" ".join(
              [arg_type, arg_name])], arg_name])
        else:
          self.dart_arguments.append([self.type_map[arg_type], arg_name])
      except KeyError:
        # there is some type we cannot handle, so this function's binding has to
        # be hand-written
        self.needs_manual_bindings = True
        continue

  def generate_extension_function_name(self, function_name):
    """Returns the name of the corresponding C extension function.

    Override this method to change the generated C extension functions'
    name scheme.

    Args:
      function_name: The name of a C function.
    Returns:
      The name of the corresponding C extension function.
    """
    return function_name + "_native"

  def __str__(self):
    return "{crt} {fn}({carg});".format(
        crt=self.c_ret_type,
        fn=self.function_name,
        carg=", ".join([" ".join(arg) for arg in self.c_arguments]))


class DefaultExtensionGenerator(object):
  """The DefaultExtensionGenerator for generating C extension code.

  Generates C and Dart extension declarations and definitions for given C
  functions.

  Usage:
    gen = DefaultExtensioNGenerator()
    gen.type_map.update({'GLvoid':'void'})
    gen.generate_bindings()

  Attributes:
    function_list: A list of {function_name, "function_name"} strings for all
        generated functions.
    c_extension_declarations: A list of C declarations for all generated
        functions.
    c_extension_definitions: A list of C function definitions for all generated
        functions.
    dart_constants: A list of Dart constant definitions for all defined
      constants passed in constant_defines.
    alias_map: A dict of C types:C types to undo typedefs, basically.
    type_map: A dict of C types:Dart types to supplement the default map.
    handle_to_c_map: A dict of Dart types:C templates to marshal Dart_Handle
        objects into native C types.
    new_handle_map: A dict of C types:C templates to marshal the
        C types to Dart_Handle objects.
    needs_free_map: A dict of Dart types:C templates to free any C memory that
        may have been allocated while marshalling Dart objects to C objects.
    constant_type_hint_map: A dict of #define variable names:Dart types to hint
        what type the Dart constant should be.
    ret_hint_map: A dict of C functions:C types to hint what type the Dart
        function should return. Yes, the values are C types.
    arg_hint_map: A dict of C argument:Dart type to hint what type the Dart
        argument should be for all arguments with the exact type and name.
  """
  default_type_map = {
      "int": "int",
      "bool": "bool",
      "void": "void",
      "const char*": "String",
      "double": "double",
      "float": "double",
  }

  default_handle_to_c_map = {
      "int": INT_HANDLE_TO_C_VAR_TEMPLATE,
      "double": DOUBLE_HANDLE_TO_C_VAR_TEMPLATE,
      "bool": BOOL_HANDLE_TO_C_VAR_TEMPLATE,
      "String": STRING_HANDLE_TO_C_VAR_TEMPLATE,
      "TypedData": TYPEDDATA_HANDLE_TO_C_VAR_TEMPLATE,
  }

  default_new_handle_map = {
      "int": "Dart_NewInteger({ret})",
      "int64_t": "Dart_NewInteger({ret})",
      "double": "Dart_NewDouble({ret})",
      "float": "Dart_NewDouble({ret})",
      "bool": "Dart_NewBoolean({ret})",
      "const char*": "Dart_NewStringFromCString({ret})",
  }

  default_needs_free_map = {"TypedData": TYPEDDATA_RELEASE_TEMPLATE,}

  def __init__(self, constant_defines, function_declarations,
               has_manual_bindings=None):
    """Inits the DefaultExtensionGenerator.

    Args:
      constant_defines: A list of C define statements that should be parsed into
          Dart constants.
      function_declarations: A list of C function declarations to generate a
         Dart native extension from.
      has_manual_bindings: A list of C function names that already have manual
          bindings.
    """
    self.c_declarations = function_declarations
    self.constant_defines = constant_defines

    self.c_functions = list()

    self.alias_map = dict()

    self.type_map = {k: v for k, v in self.default_type_map.items()}
    self.handle_to_c_map = {
        k: v
        for k, v in self.default_handle_to_c_map.items()
    }
    self.new_handle_map = {k: v for k, v in self.default_new_handle_map.items()}
    self.ret_hint_map = dict()
    self.arg_hint_map = dict()
    self.constant_type_hint_map = dict()
    self.needs_free_map = {k: v for k, v in self.default_needs_free_map.items()}

    self.dart_extension_declarations = list()
    self.c_extension_definitions = list()
    self.c_extension_declarations = list()
    self.function_list = list()

    self.needs_manual_binding_functions = list()

    self.has_manual_bindings = list()
    if has_manual_bindings is not None:
      self.has_manual_bindings = has_manual_bindings

    self.dart_constants = list()

  def generate_bindings(self):
    """Generate the extension constants, definitions, and declarations."""
    self.update_maps()
    self.dart_constants = self.parse_constant_defines(self.constant_defines)
    self.parse_functions()
    for func in self.c_functions:
      if func.needs_manual_bindings:
        self.needs_manual_binding_functions.append(func)
      else:
        self.bind_function(func)

  # Functions below are internal to the generator.

  def update_maps(self):
    """Update type_map and new_handle_map based on the alias_map."""
    for c_type, alias in self.alias_map.items():
      if alias in self.type_map:
        self.type_map[c_type] = self.type_map[alias]
      if alias in self.new_handle_map:
        self.new_handle_map[c_type] = self.new_handle_map[alias]

  def parse_functions(self):
    """Parse the C declarations into CFunctions and generate function_list."""
    self.c_functions = self.parse_function_declarations(self.c_declarations)
    for func in self.c_functions:
      if func.needs_manual_bindings and (
          func.function_name in self.has_manual_bindings):
        func.has_manual_bindings = True

    self.function_list = self.generate_function_list(self.c_functions)

  def parse_constant_defines(self, defines):
    """Parse the list of C defines into a list of Dart consts.

    Args:
      defines: A list of C define strings
    Returns:
      A list of Dart const variable strings.
    """
    constants = list()
    for line in defines:
      constants.append(self.parse_constant_define(line))
    return constants

  def parse_constant_define(self, define):
    """Parse a C define line into a Dart const variable line.

    Args:
      define: A C define statement string of the form
        `#define MYCONST 0x1234`
    Returns:
      A Dart const variable assignment string of the form
        `const int MYCONST = 0x1234`
    """
    define = define.split()
    if define[1] in self.constant_type_hint_map:
      DART_CONSTANT_TEMPLATE.format(self.constant_type_hint_map[define[1]],
                                    define[1], define[2])
    else:
      return DART_CONSTANT_TEMPLATE.format("int", define[1], define[2])

  def parse_function_declarations(self, declarations):
    """Parse a list of C function declarations to a list of CFunction objects.

    Args:
      declarations: A list of strings containing C function declarations.
    Returns:
      A list of corresponding CFunction objects.
    """
    extension_declarations = list()
    for line in declarations:
      extension_declarations.append(self.parse_function_declaration(line))
    return extension_declarations

  def parse_function_declaration(self, declaration):
    """Returns a CFunction object with the type context of this generator.

    Args:
      declaration: A string containing a C function declaration.
    Returns:
      The corresponding CFunction object.
    """
    return CFunction(
        declaration, self.type_map, self.ret_hint_map, self.arg_hint_map)

  def generate_function_list(self, c_functions):
    """Generates a list of C arrays for name resolution.

    The arrays are in C initializer format. e.g.
    `{"glfwInit", glfwInit}`

    Args:
      c_functions: A list of CFunction objects.
    Returns:
      A list of strings containing the name resolution information.
    """
    function_list = list()
    for func in c_functions:
      function_list.append(self.generate_function_list_line(func))
    return function_list

  def generate_function_list_line(self, c_function):
    """Generates a single C extension function mapping.

    Args:
      c_function: A CFunction object.
    Returns:
      Function names in C initializer format. e.g.
        `{"glfwInit", glfwInit}`
    """
    line = str()
    if c_function.needs_manual_bindings and not c_function.has_manual_bindings:
      line = "   // {{\"{function_name}\", {extension_function_name}}},".format(
          function_name=c_function.function_name,
          extension_function_name=c_function.extension_function_name)
    else:
      line = "    {{\"{function_name}\", {extension_function_name}}},".format(
          function_name=c_function.function_name,
          extension_function_name=c_function.extension_function_name)
    return line

  def generate_dart_declaration(self, c_function):
    """Generates a Dart function declaration.

    Args:
      c_function: A CFunction object.
    Returns:
      A string containing a Dart extension function declaration.
    """
    arguments = ", ".join([" ".join(arg) for arg in c_function.dart_arguments
                           if arg[1] != "void"])

    extension_function = c_function.function_name
    return DART_DECLARATION_TEMPLATE.format(
        return_type=c_function.dart_ret_type,
        function_name=c_function.function_name,
        arguments=arguments,
        extension_function=extension_function)

  def generate_c_extension_declaration(self, c_function):
    """Generates a C extension function declaration.

    Args:
      c_function: A CFunction object.
    Returns:
      The corresponding C extension function declaration.
    """
    return C_EXTENSION_DECLARATION_TEMPLATE.format(
        function_name=c_function.extension_function_name)

  def dart_handle_to_c_variable(self, index, c_arg, dart_arg):
    """Generates code that converts a Dart_Handle to a C variable.

    For a single argument in a function.

    Args:
      index: The index of the DartNativeArguments
      c_arg: A list containing the C function argument
      dart_arg: A list containing the Dart function argument
    Returns:
      The lines of the C extension function that take a given Dart_Handle
        from the extension functions's arguments and copy it to a C variable
    """
    get_handle = ARGUMENT_HANDLE_TEMPLATE.format(
        argument_name=c_arg[1],
        i=index)
    create_c_var = self.handle_to_c_map[dart_arg[
        0
    ]].format(argument_name=c_arg[1],
              argument_type=c_arg[0])
    return get_handle + create_c_var

  def generate_argument_handles(self, c_function):
    """Generates the lines that get a Dart_Handle from the arguments.

    For all arguments in a function.

    Args:
      c_function: A CFunction object.
    Returns:
      The lines of code that get all of the extension arguments' Dart_Handles
        and creates C variables from them.
    """
    native_argument_handles = list()

    for i, arg in enumerate(c_function.c_arguments):
      if arg[1] != "void":
        native_argument_handles.append(
            self.dart_handle_to_c_variable(i, arg,
                                           c_function.dart_arguments[i]))

    return native_argument_handles

  def generate_return_statement(self, c_function):
    """Generates the function's return statement.

    This code sets the NativeArguments' return value if it needs to be set.
    The actual C function call site is inside this code block.

    Args:
      c_function: A CFunction object.
    Returns:
      The return statement for the C extension function.
    """
    function_name = c_function.function_name
    arguments = [arg[1] for arg in c_function.c_arguments if arg[1] != "void"]

    return_variable = str()
    set_dart_return = str()
    if c_function.c_ret_type != "void":
      return_variable = C_EXTENSION_RETURN_STATEMENT_TEMPLATE.format(
          ret_type=c_function.c_ret_type)

      return_handle = str()
      if function_name in self.ret_hint_map:
        return_handle = self.new_handle_map[self.ret_hint_map[
            function_name
        ]].format(ret="ret")
      else:
        return_handle = self.new_handle_map[c_function.c_ret_type].format(
            ret="ret")
      set_dart_return = C_EXTENSION_SET_RETURN_TEMPLATE.format(
          return_handle=return_handle)

    return C_EXTENSION_RETURN_TEMPLATE.format(
        function_name=c_function.function_name,
        arguments=", ".join(arguments),
        return_variable=return_variable,
        set_dart_return=set_dart_return)

  def generate_function_frees(self, c_function):
    """Generates the code that frees any memory the generated code allocated.

    If an argument needs to be copied from the Dart objects to a C variable,
    and an array is malloc'd, or TypedData is acquired, the corresponding frees
    or ReleaseData is generated here.

    Args:
      c_function: A CFunction object.
    Returns:
      The free/dealloc/release code for the C extension function.
    """
    function_frees = list()

    for arg in c_function.dart_arguments:
      if arg[1] != "void" and arg[0] in self.needs_free_map:
        function_frees.append(self.needs_free_map[arg[0]].format(
            argument_name=arg[1]))

    return function_frees

  def generate_function_definition(self, c_function):
    """Generate a function definition.

    Args:
      c_function: A CFunction object.
    Returns:
      A C extension function definition corresponding to c_function.
    """
    extension_function_name = c_function.extension_function_name

    argument_handles = self.generate_argument_handles(c_function)
    function_return = self.generate_return_statement(c_function)
    function_frees = self.generate_function_frees(c_function)

    return C_EXTENSION_DEFINITION_TEMPLATE.format(
        extension_function_name=extension_function_name,
        get_argument_handles="\n  ".join(argument_handles),
        function_return=function_return,
        free_allocated_memory="\n".join(function_frees))

  def format_definition(self, definition):
    """Format a given C function definition.

    You may override this function and call out to clang-format
    or some other code formatter.
    Args:
      definition: A C function definition string.
    Returns:
      A C function definition formatted slightly more nicely.
    """
    return definition.replace("  \n", "").replace("    \n", "")

  def bind_function(self, c_function):
    """Creates Dart and C definitions and declarations.

    Args:
      c_function: A CFunction object.
    """
    self.dart_extension_declarations.append(
        self.generate_dart_declaration(c_function))
    self.c_extension_declarations.append(
        self.generate_c_extension_declaration(c_function))
    definition = self.generate_function_definition(c_function)
    self.c_extension_definitions.append(self.format_definition(definition))
