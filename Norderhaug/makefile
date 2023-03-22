# Anthony Norderhaug, Anthony Contreras
# CS 480 - Spring 2023
# RedID: 823899304, 824089247
#
# Makefile for compiling and linking modules.

PROGRAM = mmuwithtlb
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g3 -c

# Dependencies- C++ and C
DEPS_CPP = AddressConstraints.h ArgParser.h BitManipulation.h\
 			Level.h Map.h PageTable.h PageTableStats.h TLB.h
DEPS_C = print_helpers.h vaddr_tracereader.h
# Target object files
OBJS = main.o ArgParser.o TLB.o PageTable.o Level.o Map.o\
 		PageTableStats.o BitManipulation.o print_helpers.o vaddr_tracereader.o

%.o: %.cpp $(DEPS_CPP)
	$(CXX) $(CXXFLAGS) -o $<

%.o: %.c $(DEPS_C)
	gcc -std=c99 -c $<

$(PROGRAM) : $(OBJS)
	$(CXX) -o $(PROGRAM) $(OBJS)

clean :
	rm -f *.o $(PROGRAM)