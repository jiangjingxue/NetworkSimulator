

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CXX=g++
CXXFLAGS=-std=c++2a -g -pthread -g
LDFLAGS=-pthread
endif
ifeq ($(UNAME_S),Darwin)
CXXFLAGS=-std=c++20 -g -pthread -g
CXX=c++
LDFLAGS=-pthread
endif

all: bin/ex1 
OBJS=main.o network.o dumpHex.o e1server.o e1service.o e1client.o 
bin/ex1:  $(OBJS)
	$(CXX) -g -o bin/ex1 $(OBJS) $(LDFLAGS)
	

main.o:  network.hpp e1server.hpp e1service.hpp e1client.hpp

e1client.o: e1client.hpp network.hpp

e1service.o: e1service.hpp network.hpp
e1server.o: e1server.hpp e1service.hpp network.hpp

network.o: network.hpp
dumpHex.o: dumpHex.hpp


clean:
	rm bin/* *.o 
