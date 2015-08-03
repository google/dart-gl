# Copyright (c) 2015, the Dart GL extension authors. All rights reserved.
# Please see the AUTHORS file for details. Use of this source code is governed
# by a BSD-style license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd

# pylint: disable=superfluous-parens
# Parenthesis around print statements are fine.
"""Generator script for the GL extension.

Rather than running this script directly, consider running genbindings.sh if you
have clang-formatter and dartfmt installed.

This script does not generate the entire project, only function_list.h,
gl_bindings.h, gl_bindings.cc, gl_native_functions.dart, and
gl_constants.dart.
"""

import sys

from default_generator.extension_generator import DefaultExtensionGenerator
from gl_templates import *
from gl_type_maps import *

# These functions have manual bindings defined in lib/src/manual_bindings.cc
HAS_MANUAL_BINDINGS = [
    "glDeleteBuffers",
    "glDeleteFramebuffers",
    "glDeleteRenderbuffers",
    "glDeleteTextures",
    "glGenBuffers",
    "glGenFramebuffers",
    "glGenRenderbuffers",
    "glGenTextures",
    "glGetActiveAttrib",
    "glGetActiveUniform",
    "glGetAttachedShaders",
    "glGetBooleanv",
    "glGetBufferParameteriv",
    "glGetFloatv",
    "glGetFramebufferAttachmentParameteriv",
    "glGetIntegerv",
    "glGetProgramiv",
    "glGetProgramInfoLog",
    "glGetRenderbufferParameteriv",
    "glGetShaderiv",
    "glGetShaderInfoLog",
    "glGetShaderPrecisionFormat",
    "glGetShaderSource",
    "glGetTexParameterfv",
    "glGetTexParameteriv",
    "glGetUniformfv",
    "glGetUniformiv",
    "glGetVertexAttribfv",
    "glGetVertexAttribiv",
    "glGetVertexAttribPointerv",
    "glReadPixels",
    "glShaderBinary",
    "glShaderSource",
    "glTexParameterfv",
    "glTexParameteriv",
]


class GLExtensionGenerator(DefaultExtensionGenerator):
  """This class automatically generates GL extension code.

  It genererates the C extension and Dart declarations, and the C definitions.

  All functions besides init and generate_bindings are essentially private.
    You may call them, but at your own risk.

  Below attributes are those in addition to DefaultExtensionGenerator's.

  Attributes:
      None
  """
  pass


def main():
  if len(sys.argv) != 2:
    print("This script takes a single argument, the location of your system's "
          "gl2.h header file.")
    return

  header_file = None
  try:
    header_file = open(sys.argv[1], "r")
  except IOError as err:
    print(err)
    return

  const_lines = list()

  api_lines = list()

  for line in header_file:
    if line.startswith("#define GL_"):
      const_lines.append(line.strip())
    elif line.startswith("GL_APICALL "):
      api_lines.append(line.strip().replace("GL_APICALL ",
                                            "").replace("GL_APIENTRY ", ""))

  gl_gen = GLExtensionGenerator(
      constant_defines=const_lines,
      function_declarations=api_lines,
      has_manual_bindings=HAS_MANUAL_BINDINGS)
  gl_gen.alias_map.update(gl_alias_map)
  gl_gen.type_map.update(gl_type_map)
  gl_gen.handle_to_c_map.update(gl_handle_c_map)
  gl_gen.new_handle_map.update(gl_new_handle_map)
  gl_gen.ret_hint_map.update(gl_return_type_hint_map)
  gl_gen.arg_hint_map.update(gl_argument_type_hint_map)
  gl_gen.needs_free_map.update(gl_needs_free_map)

  gl_gen.generate_bindings()

  print("Writing function_list.h...")
  with open("generated/function_list.h", "w") as f:
    f.write(GL_FUNCTION_LIST_HEADER_BODY)

  print("Writing function_list.cc...")
  with open("generated/function_list.cc", "w") as f:
    f.write(GL_FUNCTION_LIST_HEADER)
    f.write("\n".join(gl_gen.function_list))
    f.write(GL_FUNCTION_LIST_FOOTER)

  print("Writing gl_bindings.h...")
  with open("generated/gl_bindings.h", "w") as f:
    f.write(GL_GENERATED_BINDINGS_HEADER_HEADER)
    f.write("// Header file for generated GL function bindings.\n\n")
    f.write("\n".join(gl_gen.c_extension_declarations))
    f.write(GL_GENERATED_BINDINGS_HEADER_FOOTER)

  print("Writing gl_bindings.cc...")
  with open("generated/gl_bindings.cc", "w") as f:
    f.write(GL_GENERATED_BINDINGS_HEADER)
    f.write("// Generated GL function bindings for Dart.\n\n")
    f.write("\n".join(gl_gen.c_extension_definitions))

  print("Writing gl_native_functions.dart...")
  with open("generated/gl_native_functions.dart", "w") as f:
    f.write(GL_DART_GENERATED_FUNCTIONS_HEADER)
    f.write("/// Dart definitions for GL native extension.\n\n")
    f.write("\n".join(gl_gen.dart_extension_declarations))

  print("Writing gl_constants.dart...")
  with open("generated/gl_constants.dart", "w") as f:
    f.write(GL_DART_GENERATED_CONSTANTS_HEADER)
    f.write("/// Generated GL constants.\n\n")
    f.write("\n".join(gl_gen.dart_constants))
    f.write("\n\n")
  print("")

  print("THESE REQUIRE MANUAL BINDING:")
  for func in gl_gen.c_functions:
    if func.needs_manual_bindings and not func.has_manual_bindings:
      print(func)
  print("")
  print("")


if __name__ == "__main__":
  main()
