#!/bin/make
CC=g++
LDFLAGS=-lGL -lGLU -lglfw -lGLEW
CFLAGS=-g -O3 -pipe -Wall -fomit-frame-pointer
CXXFLAGS=$(CFLAGS)
LINCXXFLAGS=$(CXXFLAGS) -D__LINUX__
APPNAME=gfxtest
BUILDNAME=$(APPNAME)
SRCS=gfxtest.cpp shader.cpp
OBJDIR=.

all: $(SRCS) $(APPNAME)

objects: clean
	$(CC) $(LINCXXFLAGS) -c $(SRCS)

$(APPNAME): objects
	$(CC) $(LDFLAGS) -o $(BUILDNAME) $(OBJDIR)/*.o

run: $(SRCS) $(APPNAME)
	./$(BUILDNAME) &

clean:
	rm -vf $(BUILDNAME)
	rm -vf $(OBJDIR)/*.o
	rm -vf $(OBJDIR)/*_win.o
