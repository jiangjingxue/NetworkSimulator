CXX=g++
CXXFLAGS=-pthread -std=c++17
bin/ex1: main.o ex1.pb.o
	g++ -o bin/ex1 main.o ex1.pb.o -lprotobuf -pthread
ex1.pb.h ex1.pb.cc: ex1.proto
	protoc --cpp_out=. ex1.proto


main.o: main.cpp ex1.pb.h
ex1.pb.o: ex1.pb.cc ex1.pb.h

clean:
	rm *.o bin/ex1
