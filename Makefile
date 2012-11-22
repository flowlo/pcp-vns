CC=g++
LD=g++
CFLAGS=-Wall -std=c++11 -g -c
LDFLAGS=-Wall -g
OBJECTS=main.o parser.o oneStepCd.o vns.o
HEADERS=header/vns.hpp header/pcp.hpp header/oneStepCd.hpp header/parser.hpp
PROG=pcp

all: $(PROG)

pcp: $(OBJECTS) $(HEADERS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PROG)
	
main.o:
	$(CC) $(CFLAGS) -o main.o main.cpp
	
parser.o:
	$(CC) $(CFLAGS) -o parser.o parser.cpp

oneStepCd.o:
	$(CC) $(CFLAGS) -o oneStepCd.o oneStepCd.cpp

vns.o:
	$(CC) $(CFLAGS) -o vns.o vns.cpp

clean: 
	rm $(PROG) $(OBJECTS)
