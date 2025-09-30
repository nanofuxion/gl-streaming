CFLAGS=-O3 -Wall -DGLS_SERVER -DGL_GLES_PROTOTYPES=1

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	MACOS_ANGLE_PATH=../angle
	CFLAGS+=-DUSE_ANGLE -I$(MACOS_ANGLE_PATH)/include -Wno-int-conversion -Wno-incompatible-pointer-types
	LIBS+=-L$(MACOS_ANGLE_PATH)/lib -lEGL -lGLESv2 -framework CoreFoundation -framework IOKit -framework CoreGraphics -framework Cocoa -framework Metal
	INCLUDES=-I../common -I. -I./include
else
	INCLUDES=-I/opt/vc/include/ -I/opt/vc/include/GLES3 -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I../common -I. -I./include
endif
OBJS=fifo.o glcontrol.o server.o serveregl.o servergles.o servergles3.o glserver.o

all: check_angle $(OBJS) gl_server

.PHONY: check_angle
check_angle:
ifeq ($(UNAME_S),Darwin)
	@if [ ! -d "$(MACOS_ANGLE_PATH)/include" ]; then \
		echo "Error: ANGLE not found at $(MACOS_ANGLE_PATH)"; \
		echo "Please install ANGLE libraries:"; \
		echo "  1. Download ANGLE from https://github.com/google/angle"; \
		echo "  2. Build ANGLE or download pre-built binaries"; \
		echo "  3. Create ../angle directory with include/ and lib/ subdirectories"; \
		echo "  4. Copy ANGLE headers to ../angle/include"; \
		echo "  5. Copy libEGL.dylib and libGLESv2.dylib to ../angle/lib"; \
		exit 1; \
	fi
endif

fifo.o: ../common/fifo.c ../common/fifo.h
	gcc -c -o fifo.o $(CFLAGS) ../common/fifo.c

glcontrol.o: glcontrol.c glcontrol.h
	gcc -c -o glcontrol.o $(CFLAGS) $(INCLUDES) glcontrol.c

server.o: ../common/server.c ../common/server.h fifo.o
	gcc -c -o server.o $(CFLAGS) ../common/server.c

serveregl.o: serveregl.c glserver.h ../common/gls_command.h
	gcc -c -o serveregl.o $(CFLAGS) $(INCLUDES) serveregl.c

servergles.o: servergles.c glserver.h ../common/gls_command.h
	gcc -c -o servergles.o $(CFLAGS) $(INCLUDES) servergles.c

servergles3.o: servergles3.c glserver.h ../common/gls_command.h
	gcc -c -o servergles3.o $(CFLAGS) $(INCLUDES) servergles3.c

glserver.o: glserver.c glserver.h ../common/gls_command.h glcontrol.o server.o
	gcc -c -o glserver.o $(CFLAGS) $(INCLUDES) glserver.c

gl_server: main.c glserver.o
	gcc -o gl_server $(CFLAGS) $(INCLUDES) main.c $(OBJS) $(LIBS)

clean:
	rm gl_server $(OBJS)