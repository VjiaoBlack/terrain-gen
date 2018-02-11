# Makefile
# 
# Victor Jiao
# 
# the Makefile for the project.

COMPILE=g++ 
FLAGS=-std=gnu++11 -Winline -Wall -O3
LINK=-I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image

# File names
EXEC = athena
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(addprefix build/, $(notdir $(SOURCES:.cpp=.o)))

# Main target
$(EXEC): $(OBJECTS)
	$(COMPILE) $(OBJECTS) $(LINK) $(FLAGS) -o $(EXEC)
 
# To obtain object files
# this one doesnt correctly recompile with impl
build/%.o: src/%.cpp src/%.hpp src/%-impl.hpp
	$(COMPILE) -c $(FLAGS) $< -o $@

build/%.o: src/%.cpp src/%.hpp
	$(COMPILE) -c $(FLAGS) $< -o $@

build/%.o: src/%.cpp 
	$(COMPILE) -c $(FLAGS) $< -o $@
 
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

