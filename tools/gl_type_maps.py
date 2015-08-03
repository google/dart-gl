# Copyright (c) 2015, the Dart GLFW extension authors. All rights reserved.
# Please see the AUTHORS file for details. Use of this source code is governed
# by a BSD-style license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd
"""Custom type maps for the GL extension generator.

"""

# pylint: disable=wildcard-import
from gl_templates import *

gl_alias_map = dict()
gl_alias_map.update({
    "GLenum": "int",
    "GLint": "int",
    "GLfloat": "float",
    "const GLchar*": "const char*",
    "GLboolean": "bool",
    "GLuint": "int",
    "GLsizei": "int",
    "GLsizeiptr": "int",
    "GLintptr": "int",
    "GLbitfield": "int",
})

gl_type_map = dict()
gl_type_map.update({
    "const GLubyte*": "String",
    "const void*": "TypedData",
})

gl_handle_c_map = dict()

gl_new_handle_map = dict()
gl_new_handle_map.update({
    "const GLubyte*": "Dart_NewStringFromGLubyteString({ret})",
})

gl_needs_free_map = dict()

gl_return_type_hint_map = dict()

gl_argument_type_hint_map = dict()
gl_argument_type_hint_map.update({
    # for glUniform*iv
    "const GLint* value": "TypedData",
    # for glUniform*fv
    "const GLfloat* value": "TypedData",
    # for glVertexAttrib*fv
    "const GLfloat* v": "TypedData",
})
