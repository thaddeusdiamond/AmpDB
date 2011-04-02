# Thaddeus Diamond					    4/1/2011
# Makefile for "Loadgen"				DDBMS

# Standard compiler options
CFLAGS =  -g3 -std=c99 -pedantic -Wall

all:    loadgen.o

# Create Loadgen from loadgen.o
Loadgen:  loadgen.o
	${CXX} ${CFLAGS} -lm $^ -o $@

# Rule to create loadgen.o (link library header file)
loadgen.o: loadgen.cc loadgen.h


# Clean up
clean:
	/bin/rm -f *.o Loadgen
