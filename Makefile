CC=g++
LD=g++
CFLAGS=-Wall -std=c++11
SRC=src
BIN=../bin
INCLUDES=
OBJECTS=main.o parser.o oneStepCd.o vns.o

all: pcp

pcp: $(OBJECTS)
	$(LD) $(OBJECTS) -o pcp
	cp pcp $(BIN)/pcp
	
clean: 
	rm *.o
