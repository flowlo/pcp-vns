CC=clang++
LD=clang++
CFLAGS=-Wall -std=c++11 -g -c
LDFLAGS=-Wall -g
OBJECTS=main.o parser.o oneStepCd.o vns.o
HEADERS=header/vns.hpp header/pcp.hpp header/oneStepCd.hpp header/parser.hpp
PROG=pcp

all: $(PROG)

pcp: $(OBJECTS) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PROG)
	
main.o: main.cpp $(HEADERS)
	$(CC) $(CFLAGS) -o main.o main.cpp
	
parser.o: parser.cpp header/parser.hpp
	$(CC) $(CFLAGS) -o parser.o parser.cpp

oneStepCd.o: oneStepCd.cpp header/oneStepCd.hpp
	$(CC) $(CFLAGS) -o oneStepCd.o oneStepCd.cpp

vns.o: vns.cpp header/vns.hpp
	$(CC) $(CFLAGS) -o vns.o vns.cpp

clean: 
	rm $(PROG) $(OBJECTS)
