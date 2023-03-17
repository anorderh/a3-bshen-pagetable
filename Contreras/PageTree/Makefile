CC=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb
CCFLAGS=-std=c++11 -Wall -g3 -c


# object files
OBJS = main.o vaddr_tracereader.o

# Program name
PROGRAM = PageTree

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -pthread -o $(PROGRAM) $(OBJS)

main.o : main.cpp
	$(CC) $(CCFLAGS) main.cpp

vaddr_tracereader.o: vaddr_tracereader.h vaddr_tracereader.c
	gcc -c vaddr_tracereader.c

# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
# As we use gnuemacs which leaves auto save files termintating
# with ~, we will delete those as well.
clean :
	rm -f *.o *~ $(PROGRAM)