CC = clang++
CFLAGS = -g
STD = c++20
STDLIB = libc++
MYINCLUDES = src/
SDL3INCLUDE = /usr/lib/sdl3/include
SDL3LIB = /usr/lib/sdl3/build/
SDLSTATICNAME = libSDL3.a

# How to turn a .cpp file into a .o, essentially just compile it
%.o: %.cpp
	$(CC) -stdlib=$(STDLIB) -std=$(STD) -I$(SDL3INCLUDE) -I$(MYINCLUDES) $(CFLAGS) -c $< -o $@

# How to build main
MAINSOURCE = main.cpp
MAINOBJECTS = $(MAINSOURCE:.cpp=.o)
MAINEXE = main.exe
main: $(MAINOBJECTS)
	$(CC) -stdlib=$(STDLIB) -L$(SDL3LIB) $(MAINOBJECTS) -o $(MAINEXE) -l:$(SDLSTATICNAME)

# How to build tests
# Currently only one test file
TESTSOURCE = tests/evector_tests.cpp
TESTOBJECTS = $(TESTSOURCE:.cpp=.o)
TESTEXE = test.exe
test: $(TESTOBJECTS)
	$(CC) -stdlib=$(STDLIB) -L$(SDL3LIB) $(TESTOBJECTS) -o tests/$(TESTEXE) -l:$(SDLSTATICNAME) 

all : main test

clean:
	rm -f $(wildcard *.exe) $(wildcard *.o) $(wildcard tests/*.exe) $(wildcard tests/*.o)