

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CXX=g++
CXXFLAGS=-std=c++2a -g -pthread -g
LDFLAGS=-pthread -lprotobuf -lgdbm
endif

all: bin/a1
OBJS=main.o network.o server.o client.o second_client.o kv_service.o client_stub.o kv.pb.o
bin/a1: $(OBJS)
	$(CXX) -g -o bin/a1 $(OBJS) $(LDFLAGS)

main.o:  network.hpp server.hpp kv_service.hpp client.hpp second_client.hpp

client.o: client.hpp client_stub.hpp network.hpp kv.pb.h
second_client.o: second_client.hpp client_stub.hpp network.hpp kv.pb.h

kv_service.o: kv_service.hpp network.hpp
server.o: server.hpp kv_service.hpp network.hpp

network.o: network.hpp

clean:
	rm bin/* *.o 

