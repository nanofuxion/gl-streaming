CFLAGS=-O3 -Wall -I../common -I. -I./include -fPIC -g -DGL_GLES_PROTOTYPES=1

# Use X11
CFLAGS+=-DUSE_X11

# Use screen size from server.
# CFLAGS+=-DGLS_USE_SRVSIZE

# Try to emulate client arrays with VBO.
CFLAGS+=-DGLS_EMULATE_VBO

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS+=-I/opt/X11/include -Wno-int-conversion -Wno-incompatible-pointer-types
	LIBS=-pthread -L/opt/X11/lib -lX11
	LIB_GLES=libGLESv3.dylib
	LIBEXT=.dylib
	SHARED_FLAGS=-dynamiclib -install_name @rpath/libGLESv3.dylib
else
	CFLAGS+=-I/opt/vc/include/GLES3
	LIBS=-pthread -ldl -lX11
	LIB_GLES=libGLESv3.so
	LIBEXT=.so
	SHARED_FLAGS=-shared -Wl,-init,gls_init_library,--no-undefined
endif

CFLAGS+= $(LIBS)
OBJS=fifo.o server.o clientegl.o clientgles.o glclient.o

GLS_LIB_DIR=/usr/local/lib/gl-streaming

EXES=$(LIB_GLES) libEGL$(LIBEXT)

all: $(EXES)

fifo.o: ../common/fifo.c ../common/fifo.h
	gcc -c -o fifo.o $(CFLAGS) ../common/fifo.c

server.o: ../common/server.c ../common/server.h fifo.o
	gcc -c -o server.o $(CFLAGS) ../common/server.c

clientegl.o: clientegl.c include/EGL/egl.h
	gcc -c -o clientegl.o $(CFLAGS) clientegl.c

clientgles.o: clientgles.c include/GLES2/gl2.h
	gcc -c -o clientgles.o $(CFLAGS) clientgles.c

clientgles3.o: clientgles3.c include/GLES3/gl32.h
	gcc -c -o clientgles3.o $(CFLAGS) clientgles3.c

glclient.o: glclient.c glclient.h ../common/gls_command.h server.o
	gcc -c -o glclient.o $(CFLAGS) glclient.c

$(LIB_GLES): $(OBJS)
	gcc $(SHARED_FLAGS) -o $(LIB_GLES) $(CFLAGS) $(OBJS) $(LIBS)

libEGL$(LIBEXT): $(LIB_GLES)
	ln -sf $(LIB_GLES) libEGL$(LIBEXT)

install:
	mkdir -p $(GLS_LIB_DIR)
	cp $(LIB_GLES) $(GLS_LIB_DIR)/
ifeq ($(UNAME_S),Darwin)
	ln -sf $(GLS_LIB_DIR)/$(LIB_GLES) $(GLS_LIB_DIR)/libEGL.dylib
	@echo "To use streaming on macOS, type: export DYLD_LIBRARY_PATH=$(GLS_LIB_DIR)"
else
	ln -sf $(GLS_LIB_DIR)/libGLESv3.so $(GLS_LIB_DIR)/libEGL.so.1
	@echo "To use streaming, type: export LD_LIBRARY_PATH=$(GLS_LIB_DIR)"
endif
	@echo "Then you are ready to get hardware acceleration!"

clean:
	rm -f $(EXES) $(OBJS)