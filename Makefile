CXXFLAGS=-g -O2 -Wall -Wno-deprecated
LDFLAGS=

SRCS=config.cc remote.cc global.cc
OBJS=$(SRCS:.cc=.o)

all: $(OBJS)
	$(MAKE) -C db
	$(MAKE) -C mediator
	$(MAKE) -C loadgen
	$(MAKE) -C preprocessor
	$(MAKE) -C utils

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)
	$(MAKE) -C db clean
	$(MAKE) -C mediator clean
	$(MAKE) -C loadgen clean
	$(MAKE) -C preprocessor clean
	$(MAKE) -C utils clean
