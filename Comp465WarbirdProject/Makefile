# Makefile for use with Mac OSX for Comp 465/L
# Rename this file from Makefile-mac to Makefile before using.
#
# There are two options:
#    $ make		will make the Target
#    $ make clean	will remove the Target to force rebuilding on next make
#
# Edit the "SRC="  and "TARGET=" lines to set a new source file and target
#
# Notes on Makefile
# http://www.csun.edu/~renzo/programming/makefile.htm
#
# Mike Barnes 9/3/2016

# SRC specifies which files to compile as part of the project
SRC = manyModelsStatic.cpp

# CC specifies which compiler we're using
CC = g++

# INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include 

# LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L/usr/local/lib -L/opt/X11/lib 

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w   to supresses warnings
# -v   for verbose output
# -std=c++11  to set c++ version
COMPILER_FLAGS = -w

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -framework OpenGL -framework GLUT -lglew

# TARGET specifies the name of our exectuable
TARGET = project465

$(TARGET) :	 $(SRC)
	$(CC) $(SRC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)

clean:	
	rm -r $(TARGET)
