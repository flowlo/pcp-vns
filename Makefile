CC=clang++
LD=clang++
PCH=pch
CFLAGS=-Wall -std=c++11 -g -c
LDFLAGS=-Wall -g
OBJECTS=main.o parser.o oneStepCd.o vns.o
HEADERS=header/vns.hpp.$(PCH) header/pcp.hpp.$(PCH) header/oneStepCd.hpp.$(PCH) header/parser.hpp.$(PCH) header/main.hpp.$(PCH)
PROG=pcp

all: $(PROG)

clang:
	$(MAKE) CC=clang++ LD=clang++ PCH=pch

gcc:
	$(MAKE) CC=g++ LD=g++ PCH=gch $(PROG)

$(PROG): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PROG)
	
main.o: main.cpp $(HEADERS)
	$(CC) $(CFLAGS) -include header/main.hpp -o main.o main.cpp
	
parser.o: parser.cpp header/parser.hpp.$(PCH)
	$(CC) $(CFLAGS) -include header/parser.hpp -o parser.o parser.cpp

oneStepCd.o: oneStepCd.cpp header/oneStepCd.hpp.$(PCH)
	$(CC) $(CFLAGS) -include header/oneStepCd.hpp -o oneStepCd.o oneStepCd.cpp

vns.o: vns.cpp header/vns.hpp.$(PCH)
	$(CC) $(CFLAGS) -include header/vns.hpp -o vns.o vns.cpp

header/vns.hpp.$(PCH): header/vns.hpp header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/vns.hpp -o header/vns.hpp.$(PCH)

header/pcp.hpp.$(PCH): header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/pcp.hpp -o header/pcp.hpp.$(PCH)

header/main.hpp.$(PCH): header/main.hpp header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/main.hpp -o header/main.hpp.$(PCH)
	
header/parser.hpp.$(PCH): header/parser.hpp header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/parser.hpp -o header/parser.hpp.$(PCH)

header/oneStepCd.hpp.$(PCH): header/oneStepCd.hpp header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/oneStepCd.hpp -o header/oneStepCd.hpp.$(PCH)

clean: 
	rm $(PROG) $(OBJECTS)
