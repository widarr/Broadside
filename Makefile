#!/bin/make
CC=g++
LDFLAGS=-lGL -lGLU -lglfw -lboost_system -lboost_filesystem -lcgutils
CFLAGS=-g -O3 -pipe -Wall -fomit-frame-pointer
CXXFLAGS=$(CFLAGS)
APPNAME=Broadside
BUILDNAME=$(APPNAME)
SRCS=Broadside.cpp
OBJDIR=.
LIBCGUTILSDIR=libcgutils/build/
OBJ3DINCLUDE=libcgutils/Obj3D/
CGUTILSINCLUDE=libcgutils/

all: $(SRCS) $(APPNAME)

objects: clean
	$(CC) $(CXXFLAGS) -I${CGUTILSINCLUDE} -I${OBJ3DINCLUDE} -c $(SRCS)

$(APPNAME): objects
	$(CC) $(LDFLAGS) -L${LIBCGUTILSDIR} -o $(BUILDNAME) $(OBJDIR)/*.o

run: $(SRCS) $(APPNAME)
	./$(BUILDNAME) &

clean:
	rm -vf $(BUILDNAME)
	rm -vf $(OBJDIR)/*.o
