# Makefile
# 
# Victor Jiao
# 
# the Makefile for the project.

COMPILE=clang++ 
# FLAGS=-g -fno-omit-frame-pointer -fsanitize=address -std=gnu++11 -Winline -Wall -O1
FLAGS=-std=gnu++11 -Winline -Wall -O3
LINK=-I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf

# clang-format --style=LLVM --sort-includes src/Utils.hpp 

# File names
EXEC = athena
SOURCES = $(shell find src/ -type f -name '*.cpp')
FILES = $(shell find src/ -type f -name '*pp')
FORMATTED_SOURCES = $(addsuffix .clean, $(FILES))
OBJECTS = $(addprefix build/, $(notdir $(SOURCES:.cpp=.o)))
      
# Targets
default: build $(EXEC)

build:
	mkdir build

format: $(FORMATTED_SOURCES)

src/%pp.clean: src/%pp
	clang-format --style=Google $< > $@
	diff $@ $<; if [ $$? -eq 1 ]; then mv $@ $<; fi;
	rm $@

# Main target
$(EXEC): $(OBJECTS)
	$(COMPILE) $(OBJECTS) $(LINK) $(FLAGS) -o $(EXEC)
 
# To obtain object files
# this one doesnt correctly recompile with impl
build/%.o: src/*/%.cpp src/*/%.hpp src/*/%-impl.hpp
	$(COMPILE) -c $(FLAGS) $< -o $@

build/%.o: src/*/%.cpp src/*/%.hpp
	$(COMPILE) -c $(FLAGS) $< -o $@

 
build/%.o: src/%.cpp src/%.hpp
	$(COMPILE) -c $(FLAGS) $< -o $@

build/%.o: src/%.cpp 
	$(COMPILE) -c $(FLAGS) $< -o $@
 
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

