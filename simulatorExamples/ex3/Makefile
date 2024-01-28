

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

all: bin/ex3 
OBJS=main.o network.o dumpHex.o e3server.o e3service.o e3client.o 
bin/ex3:  $(OBJS)
	$(CXX) -g -o bin/ex3 $(OBJS) $(LDFLAGS)
	

main.o:  network.hpp e3server.hpp e3service.hpp e3client.hpp

e3client.o: e3client.hpp network.hpp

e3service.o: e3service.hpp network.hpp
e3server.o: e3server.hpp e3service.hpp network.hpp

network.o: network.hpp
dumpHex.o: dumpHex.hpp


clean:
	rm bin/* *.o 
