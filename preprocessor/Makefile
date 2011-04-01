PROG=preproc

CXXFLAGS=-g -O2 -Wall -Wno-deprecated
LDFLAGS=-lrt # for clock_gettime()

SRCS=main.cc config.cc server.cc conflict_resolver.cc scheduler.cc
OBJS=$(SRCS:.cc=.o) ../config.o ../remote.o

all: $(PROG)

../%.o: ../%.cc
	$(MAKE) -C ..

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROG): $(OBJS)
	$(CXX) -o $(PROG) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(PROG) $(OBJS)
