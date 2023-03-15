# Anthony Norderhaug
# CS 480 - Spring 2023
# RedID: 823899304

CXX = g++
CXXFLAGS = -std=c++11 -Wall -g3 -c

all: main

main : main.o vaddr_tracereader.o
	$(CXX) -o main main.o vaddr_tracereader.o

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp

vaddr_tracereader.o : src/vaddr_tracereader.h src/vaddr_tracereader.c
	gcc -c src/vaddr_tracereader.c

clean :
	rm -f *.o $(PROGRAM)