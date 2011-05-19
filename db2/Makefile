PROG=db

CXXFLAGS=-g -O2 -Wno-deprecated -Wall
LDFLAGS=

SRCS=db.cc
OBJS=$(SRCS:.cc=.o)

all: $(PROG)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) -lpthread

clean:
	rm -f $(PROG) $(OBJS)

