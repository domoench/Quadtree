CC = g++
CFLAGS = -Wall -ggdb
INCLUDE = -I/usr/include
LIBDIR = -L/usr/lib/x86_64-linux-gnu
# Libraries that use native graphics hardware --
LIBS = -lglut -lGLU -lGL -lpthread -lm -lglfw

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -Wall -g -D__MAC__
INCLUDE =
LIBDIR = -L/lusr/X11/lib
LIBS = -lglfw -framework OpenGL -framework GLUT
endif

###########################################################
quadtree: main.cpp bb.cpp bb.h constants.h geometry.cpp geometry.h qtnode.cpp qtnode.h scene.cpp scene.h shader.hpp shader.cpp
	${CC} ${CFLAGS} ${INCLUDE} -o quadtree ${LIBDIR} main.cpp bb.cpp geometry.cpp qtnode.cpp scene.cpp shader.cpp ${LIBS}

clean:
	rm -f quadtree *.o
