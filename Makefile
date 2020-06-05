CXX=g++
CXXFLAGS=-std=c++14 -O3 -Wall -Werror
CXXINCLUDE=-I /usr/include -I.

.DEFAULT_GOAL=bagman.elf 

DEPS=easyws/easywsclient.hpp

OBJS=$(DEPS:%.hpp=%.o)

.SECONDARY: $(OBJS)

%.o: %.cpp $(DEPS)
	@$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS) $(LDLIBS)

%.elf: %.cc $(OBJS) 
	@$(CXX) $(CXXFLAGS) $(CXXINCLUDE) -o $@ $< $(OBJS) $(LDFLAGS) $(LDLIBS)

all: bagman.elf

clean:
	@rm -f *.elf $(OBJS)
