PROG=cluster #client dummy-db 

CXXFLAGS=-g -O2 -Wall -Wno-deprecated
LDFLAGS=

CLIENT_SRCS=client.cc ../global.cc
CLIENT_OBJS=$(CLIENT_SRCS:.cc=.o) ../config.o

DB_SRCS=dummy-db.cc
DB_OBJS=$(DB_SRCS:.cc=.o) ../config.o ../remote.o

CLUSTER_SRCS=cluster.cc
CLUSTER_OBJS=$(CLUSTER_SRCS:.cc=.o) ../config.o

all: $(PROG)

../%.o: ../%.cc
	$(MAKE) -C ..

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

client: $(CLIENT_OBJS) ../loadgen/loadgen.o
	$(CXX) -o $@ $(CLIENT_OBJS) $(LDFLAGS) -lrt

dummy-db: $(DB_OBJS)
	$(CXX) -o $@ $(DB_OBJS) $(LDFLAGS)

cluster: $(CLUSTER_OBJS)
	$(CXX) -o $@ $(CLUSTER_OBJS) $(LDFLAGS)

clean:
	rm -f $(PROG) $(CLIENT_OBJS) $(DB_OBJS) $(CLUSTER_OBJS)
