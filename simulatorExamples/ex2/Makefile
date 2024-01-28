

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

all: bin/ex2 
OBJS=main.o network.o dumpHex.o e2server.o e2service.o e2client.o 
bin/ex2:  $(OBJS)
	$(CXX) -g -o bin/ex2 $(OBJS) $(LDFLAGS)
	

main.o:  network.hpp e2server.hpp e2service.hpp e2client.hpp

e2client.o: e2client.hpp network.hpp

e2service.o: e2service.hpp network.hpp
e2server.o: e2server.hpp e2service.hpp network.hpp

network.o: network.hpp
dumpHex.o: dumpHex.hpp


clean:
	rm bin/* *.o 
