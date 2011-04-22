# Thaddeus Diamond					    03/30/2011
# Makefile for "TPC-C"					DDBMS

# Standard compiler options
PROG = TPC-C
CFLAGS =  -g3 -Wno-deprecated -lpthread -lm
SRCS = tpc_c.cc new_order.cc payment.cc ../loadgen/loadgen.cc ../remote.cc ../config.cc ../freeport_config.cc
OBJS = $(SRCS:.cc=.o)

all: $(PROG)

# Create objects from .cc files
%.o: %.cc
	$(CXX) $(CFLAGS) -o $@ -c $<

# Create TPC-C from tpc_c.o and new_order.o
$(PROG): $(OBJS)
	$(CXX) $(CFLAGS) -o $(PROG) $(OBJS)

# Clean up
clean:
	rm -f $(PROG) $(OBJS)
