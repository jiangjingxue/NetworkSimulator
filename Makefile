UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CXX=g++
CXXFLAGS=-std=c++2a -g -pthread -g
LDFLAGS=-pthread -lprotobuf -lgdbm
endif
ifeq ($(UNAME_S),Darwin)
CXXFLAGS=-std=c++20 -g -pthread -g
CXX=c++
LDFLAGS=-pthread -lprotobuf -lgdbm
endif

all: bin/a1 
OBJS=main.o network.o dumpHex.o rpcserver.o kvservice.o rpcclient.o kv.pb.o
bin/a1:  $(OBJS)
	$(CXX) -g -o bin/a1 $(OBJS) $(LDFLAGS)

kv.pb.h kv.pb.cc: kv.proto
	protoc --cpp_out=. kv.proto
	
main.o:  network.hpp rpcserver.hpp kvservice.hpp rpcclient.hpp kv.pb.h

rpcclient.o: rpcclient.hpp network.hpp

kvservice.o: kvservice.hpp network.hpp
rpcserver.o: rpcserver.hpp kvservice.hpp network.hpp

kv.pb.o: kv.pb.cc kv.pb.h
network.o: network.hpp
dumpHex.o: dumpHex.hpp


clean:
	rm bin/* *.o 
