PROG=lm

CXXFLAGS=-g -O2 -Wno-deprecated -Wall
LDFLAGS=

SRCS=lm.cc ../remote.cc ../config.cc ../nonblock.cc ../loadgen/loadgen.cc
OBJS=$(SRCS:.cc=.o)

all: $(PROG)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) -lpthread

clean:
	rm -f $(PROG) $(OBJS)

