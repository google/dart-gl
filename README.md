# Dart native extension for [GLES2](https://www.khronos.org/opengles/2_X/)

Supports Linux and Windows. OSX is not supported due to inherent
incompatibilites in the OSX UI threading model, as well as Apple announcing the
discontinuing of support of OpenGL on OSX (https://goo.gl/qQdeQ5).

# Steps to generate the bindings

```shell
mkdir -p lib/src/generated
pub run tools/gl_generator.dart
clang-format -i --style=Google generated/*.{cc,h}
dartfmt -w generated/*.dart
mv generated/* lib/src/generated/
```

## Whitelisting

Some GL libraries come with headers that list functions not implemented. This
will fail at link time. To avoid this, you can dump the symbols in libGLESv2.so
to be whitelisted and use the `--whitelist flag in`tools/gl_generator.dart`

```shell
nm -D /lib/libGLESv2.so | grep " T " | awk '{print $3}' > whitelist.txt
```

# Steps to compile the bindings

The previous method for compiling the bindings is no longer available. We are
working on a new solution for both Linux and Windows.

In the meantime, the Makefile in the lib/ directory is a good starting point for
compiling on Linux.

# Functions with interfaces with APIs different from GLES2

If a function's only difference is moving pointer parameters to the return
value, it is not listed. See `lib/src/manual_bindings.dart` for a full list.

-   `glGetActiveAttrib` doesn't take a bufSize parameter, and returns an
    `ActiveAttrib` instance.
-   `glGetActiveUniform` doesn't take a bufSize parameter, and returns an
    `ActiveAttrib` instance.
-   `glGetAttachedShaders` doesn't take a maxCount parameter, returns a
    `List<int>`.
-   `glGetProgramInfoLog` doesn't take a maxSize parameter, and returns a
    `String`.
-   `glGetShaderPrecisionFormat` returns a `ShaderPrecisionFormat` instance.
-   `glGetShaderSource` doesn't take a bufSize parameter and just returns a
    `String`.
-   `glReadPixels` takes a `TypedData pixels` parameter and the data is put into
    it, like in the WebGL API. Your program will most likely crash if the
    TypedData object you pass in is not big enough, or possibly if it's in the
    wrong format.
-   `glShaderSource` just takes the parameters `int shader` and `String string`.
    The original API is not really appropriate for Dart.

# A note about OpenGL, the Dart VM, and native threads

OpenGL is a thread-bound API. That is, an OpenGL context must be bound (or "made
current") on a thread before any other OpenGL functions may be called.

The Dart VM uses a pool of native threads under the hood to carry out tasks on
the event queue
([overview of the Dart event loop](https://webdev.dartlang.org/articles/performance/event-loop)).

This leads to an unfortunate restriction on the use of asynchronous code while
making OpenGL calls from Dart. This is bad:

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
completes. When control returns to the next line, it may be running on a
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

This way, the context is released from the thread before control returns to the
event loop and then reacquired when it comes back.

Note that this applies to any asynchronous code (not just an await). Here's
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

In this case, there's no guarantee that the callback to the `Future` returned by
`readFileAsBytes` would be running on the same thread as the original task.

In practice, most OpenGL code is written synchronously, as it's generally not
advisable to be waiting for other tasks to complete while in the middle of
rendering a frame. However, it's important to be aware of this restriction. When
making OpenGL calls, either avoid awaiting asynchronous methods and making
OpenGL calls in async callbacks, or properly release the context anytime control
returns to the event loop, and reacquire it when ready to make OpenGL calls
again.
