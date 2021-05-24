# makefile for Zelvici, a 2D arcade game with lovely turtle
# Zelvici  Copyright (C) 2006, 2008 Dave Schwarz
# Makefile Copyright (C) 2008 Jaromir Hradilek

# This program is free software;  you can redistribute it  and/or modify it
# under the  terms of the  GNU General Public License  as published  by the
# Free Software Foundation;  either version 2  of the License, or  (at your
# option) any later version.
# 
# This  program is  distributed  in the  hope that  it will be useful,  but
# WITHOUT ANY WARRANTY;  without even the implied warranty of  MERCHANTABI-
# LITY  or  FITNESS FOR A PARTICULAR PURPOSE.  See  the  GNU General Public
# License for more details.
# 
# You should have received a copy of the  GNU General Public License  along
# with this program;  if not,  write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


# General settings; feel free to modify according to your actual situation:
SHELL    = /bin/sh
CXX      = g++
CXXFLAGS = -Wall -O3 -march=i686
LIBS    := $(shell sdl-config --libs --cflags) -lSDL_mixer
SRCS    := $(wildcard *.cpp)
OBJS    := $(patsubst %.cpp, %.o, $(SRCS))


# Make rules;  please do not edit these unless you really know what you are
# doing:
.PHONY: all clean

all: zelvici

clean:
	-rm -f $(OBJS) zelvici

zelvici: $(OBJS)
	$(CXX) $(LIBS) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

