# Makefile comments
PROGRAMS = problem1 problem2 problem3
CC = gcc
CFLAGS = -Wall -g
all: $(PROGRAMS)
clean:
	rm -f *.o
# C compilations

array2D.o: array2D.c array2D.h
	$(CC) $(CFLAGS) -c array2D.c
	
hashTableGraph.o: hashTableGraph.c hashTableGraph.h
	$(CC) $(CFLAGS) -c hashTableGraph.c
	
hashTable.o: hashTable.c hashTable.h
	$(CC) $(CFLAGS) -c hashTable.c

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c
	
graph.o: graph.c graph.h hashTableGraph.o hashTableGraph.h
	$(CC) $(CFLAGS) -c graph.c
	
problem1.o: problem1.c
	$(CC) $(CFLAGS) -c problem1.c
	
problem2.o: problem2.c tree.o tree.h
	$(CC) $(CFLAGS) -c problem2.c
	
problem3.o: problem3.c
	$(CC) $(CFLAGS) -c problem3.c

# Executable programs

###TODO: complete this Makefile by filling in the required .o files and gcc command; if you only wish to test a single problem/program rather than all of them then you can run commands such as "make problem1", "make problem2", "make problem3", etc. on the command terminal

problem1: problem1.o
	$(CC) $(CFLAGS) problem1.o -o problem1.out

problem2: problem2.o tree.o
	$(CC) $(CFLAGS) problem2.o tree.o -o problem2.out

problem3: problem3.o graph.o hashTableGraph.o
	$(CC) $(CFLAGS) problem3.o graph.o hashTableGraph.o -o problem3.out