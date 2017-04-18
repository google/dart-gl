# Dart native extension for [GLES2](https://www.khronos.org/opengles/2_X/)

NOTE: It is unlikely that programs written with these bindings will work on OSX
due to platform requirements that the main thread be the one handling events.

# Steps to generate the bindings
```shell
pub run tools/gl_generate.dart
mv tools/generated lib/src/
```
# Steps to compile the bindings

## Linux with GNU Make:
If your `dart_api.h`, `GLES2/gl2.h`, or `libGL.so` files are not in the
standard locations, set the enviroment variables `DART_INCLUDE`, `GL_INCLUDE`,
and/or `GL_LIB` and `GL_LIB_NAME` and run the following commands.

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
Tested to work with NVIDIA, just requires building glfw with EGL support.

## Other Platforms
TODO

# Functions with interfaces with APIs different from GLES2
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

# A note about OpenGL, the Dart VM, and native threads
OpenGL is a thread-bound API.  That is, an OpenGL context must be bound (or
"made current") on a thread before any other OpenGL functions may be called.

The Dart VM uses a pool of native threads under the hood to carry out tasks
on the event queue ([overview of the Dart event loop](https://webdev.dartlang.org/articles/performance/event-loop)).

This leads to an unfortunate restriction on the use of asynchronous code while
making OpenGL calls from Dart.  This is bad:

```dart
glfwMakeContextCurrent(context);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// other OpenGL calls ...

// Wait for an async task to finish.
await someFuture;

// Draw a triangle.
glDrawArrays(GL_TRIANGLES, 0, 3);
// etc...
```

The issue is that the context is made current, then there is a call to await,
which causes the current task to return to the event loop until `someFuture`
completes.  When control returns to the next line, it may be running on a
completely different native thread, where the context is not bound.

To avoid this issue, the code must be changed to something resembling the
following:

```dart
glfwMakeContextCurrent(context);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// other OpenGL calls ...

// Release the context before waiting.
glfwMakeContextCurrent(NULL);

// Wait for an async task to finish.
await someFuture;

// We're back!  Reacquire the context.
glfwMakeContextCurrent(context);

// Draw a triangle.
glDrawArrays(GL_TRIANGLES, 0, 3);
// etc...
```

This way, the context is released from the thread before control returns to
the event loop and then reacquired when it comes back.

Note that this applies to any asynchronous code (not just an await).  Here's
another bad example:

```dart
glfwMakeContextCurrent(context);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// other OpenGL calls ...

// Load an image, then create a texture.
var f = new File("image.rgb");
f.readFileAsBytes().then((bytes) {
  var tex = glGenTextures(1);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA, width, height, 0, GL_UNSIGNED_BYTE,
      GL_RGBA, bytes);
});

// etc...
```

In this case, there's no guarantee that the callback to the `Future` returned
by `readFileAsBytes` would be running on the same thread as the original task.

In practice, most OpenGL code is written synchronously, as it's generally not
advisable to be waiting for other tasks to complete while in the middle of
rendering a frame.  However, it's important to be aware of this restriction.
When making OpenGL calls, either avoid awaiting asynchronous methods and
making OpenGL calls in async callbacks, or properly release the context and
anytime control returns to the event loop, and reacquire it when ready to
make OpenGL calls again.

