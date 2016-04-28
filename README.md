# Dart native extension for [GLES2](https://www.khronos.org/opengles/2_X/)

NOTE: It is unlikely that programs written with these bindings will work on OSX
due to platform requirements that the main thread be the one handling events.

# Steps to generate the bindings
```shell
mkdir lib/src/generated/
cd tools
./generate_bindings.sh
cp generated/* ../lib/src/generated/
```
# Steps to compile the bindings

Set DART_INCLUDE, GL_INCLUDE, GL_LIB and GL_LIB_NAME.

```shell
cd lib/
dart ../tools/gl_compile.dart
```
Note that if you set the `GL_LIB` variable when compiling, you must also set
`LD_LIBRARY_PATH` to include the same directory when running your program or
the Dart VM will not be able to find `libGL.so`/`lib$(GL_LIB_NAME).so`.

TODO(hstern): It is convenient for development to use the .so file,
but for distribution purposes it is less useful. It would be nice to have an
option to use the .a library as well.

## Windows
Test to work with NVIDIA, just requires building glfw with EGL support.

## Other Platforms
TODO

# Functions with interfaces with APIs different from GLES2.
If a function's only difference is moving pointer parameters to the return
value, it is not listed. See `lib/src/manual_bindings.dart` for a full list.

- `glGetActiveAttrib` doesn't take a bufSize parameter, and returns an
  `ActiveAttrib` instance.
- `glGetActiveUniform` doesn't take a bufSize parameter, and returns an
  `ActiveAttrib` instance.
- `glGetAttachedShaders` doesn't take a maxCount parameter, returns a
  `List<int>`.
- `glGetProgramInfoLog` doesn't take a maxSize parameter, and returns a
  `String`.
- `glGetShaderPrecisionFormat` returns a `ShaderPrecisionFormat` instance.
- `glGetShaderSource` doesn't take a bufSize parameter and just returns a
  `String`.
- `glReadPixels` takes a `TypedData pixels` parameter and the data is put into
  it, like in the WebGL API. Your program will most likely crash if the
  TypedData object you pass in is not big enough, or possibly if it's in the wrong
  format.
- `glShaderSource` just takes the parameters `int shader` and `String string`.
  The original API is not really appropriate for Dart.
