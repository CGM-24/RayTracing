# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -std=c++11

# Include paths for libraries
INCLUDES = -I/usr/include -I/usr/local/include

# Libraries to link with (GLFW, GLEW, OpenGL)
LIBS = -lglfw -lGLEW -lGL -lm

# Source files and object files
SRCS = sphere.cpp
OBJS = $(SRCS:.cpp=.o)

# Name of the output executable
MAIN = sphere

# Rule to build the executable
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

# Rule to build object files from source files
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	$(RM) *.o *~ $(MAIN)
