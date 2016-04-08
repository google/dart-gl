CC=gcc
#CC=clang

CFLAGS = -Wall -Werror

ifneq ($(strip $(GL_INCLUDE)),)
CFLAGS += -I$(GL_INCLUDE)
endif

ifneq ($(strip $(DART_INCLUDE)),)
CFLAGS += -I$(DART_INCLUDE)
endif

CFLAGS += -fPIC -DDART_SHARED_LIB


LDFLAGS = -shared -Wl,-soname=libgl_extension.so

ifneq ($(strip $(GL_LIB)),)
LDFLAGS += -L$(GL_LIB)
endif

ifneq ($(strip $(GL_LIB_NAME)),)
LDFLAGS += -l$(GL_LIB)
else
LDFLAGS += -lGL
endif


all: gl_extension

OBJS = src/gl_extension.o src/instantiate_gl_classes.o src/manual_bindings.o src/util.o src/generated/function_list.o src/generated/gl_bindings.o

gl_extension: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o libgl_extension.so

src/gl_extension.o: src/gl_extension.cc
	$(CC) $(CFLAGS) -c $< -o $@

src/instantiate_gl_classes.o: src/instantiate_gl_classes.cc
	$(CC) $(CFLAGS) -c $< -o $@

src/manual_bindings.o: src/manual_bindings.cc
	$(CC) $(CFLAGS) -c $< -o $@

src/util.o: src/util.cc
	$(CC) $(CFLAGS) -c $< -o $@

src/generated/function_list.o: src/generated/function_list.cc
	$(CC) $(CFLAGS) -c $< -o $@

src/generated/gl_bindings.o: src/generated/gl_bindings.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm src/*.o src/generated/*.o libgl_extension.so
