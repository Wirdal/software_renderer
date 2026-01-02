CC = clang++
CFLAGS = -g
STD = c++20
STDLIB = libc++
MYINCLUDES = src/
SDL3INCLUDE = /usr/lib/sdl3/include
SDL3LIB = /usr/lib/sdl3/build/
SDLSTATICNAME = libSDL3.a
WARNOPTIONS = -Wall
# How to turn a .cpp file into a .o, essentially just compile it
%.o: %.cpp
	$(CC) $(WARNOPTIONS) -stdlib=$(STDLIB) -std=$(STD) -I$(SDL3INCLUDE) -I$(MYINCLUDES) $(CFLAGS) -c $< -o $@

# How to build main
MAINSOURCE = main.cpp
MAINOBJECTS = $(MAINSOURCE:.cpp=.o)
MAINEXE = main.exe
main: $(MAINOBJECTS)
	$(CC) $(WARNOPTIONS) -stdlib=$(STDLIB) -L$(SDL3LIB) $(MAINOBJECTS) -o $(MAINEXE) -l:$(SDLSTATICNAME)

# How to build tests
TESTSOURCE = $(wildcard tests/*.cpp)
TESTOBJECTS = $(TESTSOURCE:.cpp=.o)
TESTEXE = test.exe
test: $(TESTOBJECTS)
	$(CC) $(WARNOPTIONS) -stdlib=$(STDLIB) -L$(SDL3LIB) $(TESTOBJECTS) -o tests/$(TESTEXE) -l:$(SDLSTATICNAME) 

all : main test

clean:
	rm -f $(wildcard *.exe) $(wildcard *.o) $(wildcard tests/*.exe) $(wildcard tests/*.o)