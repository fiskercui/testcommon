#c compiler
#c++ compiler
CC = gcc
CXX = g++
#c linker
#c++ linker
CLINKER = gcc
CXXLINKER = g++
#compile define macro
DEFINES +=
#c flag
#c++ flag
CFLAGS += -g -pg
CXXFLAGS += -g -pg -rdynamic
LINKFLAGS += -pg

LINK = $(CXXLINKER)

#include path
INCPATH +=
#libs
LIBS +=

#outdir
OUTPUT=output/
RUN=run/

#objects
OBJECTS = test.o error.o select.o epoll.o
#targets
TARGETS = testproject

DEL = rm -rf
first:all
.SUFFIXES: .o .c .cpp .cxx .C
.c.o:
	$(CC) -c $(INCPATH) $(CFLAGS) -o "$@" "$<"
.cpp.o:
	$(CXX) -c $(INCPATH) $(CXXFLAGS) -o "$@" "$<"
.cxx.o:
	$(CXX) -c $(INCPATH) $(CXXFLAGS) -o "$@" "$<"
.C.o:
	$(CXX) -c $(INCPATH) $(CXXFLAGS) -o "$@" "$<"
all:$(TARGETS)
$(TARGETS):$(OBJECTS)
	$(LINK) $(LINKFLAGS) -o $(TARGETS) $(OBJECTS) $(LIBS)
clean:
	$(DEL) $(OBJECTS)	
