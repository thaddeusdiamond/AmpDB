# Thaddeus Diamond					    03/30/2011
# Makefile for "TPC-C"					DDBMS

# Standard compiler options
CFLAGS =  -g3

all:    TPC-C

# Create TPC-C from tpc_c.o and new_order.o
TPC-C:  tpc_c.o new_order.o payment.o ../loadgen/loadgen.o
	${CXX} ${CFLAGS} -lm $^ -o $@

# Rule to create tpc_c.o (link library header file)
tpc_c.o: tpc_c.cc tpc_c.h
	${CXX} -c ${CFLAGS} tpc_c.cc

# Rule to create new_order.o (link library header file)
new_order.o: new_order.cc tpc_c.h
	${CXX} -c ${CFLAGS} new_order.cc

# Rule to create payment.o (link library header file)
payment.o: payment.cc tpc_c.h
	${CXX} -c ${CFLAGS} payment.cc

# Clean up
clean:
	/bin/rm -f *.o TPC-C
