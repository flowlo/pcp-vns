CC=clang++
LD=clang++
CFLAGS=-Wall -std=c++11 -g -c
LDFLAGS=-Wall -g
OBJECTS=main.o parser.o oneStepCd.o vns.o
HEADERS=header/vns.hpp.pch header/pcp.hpp.pch header/oneStepCd.hpp.pch header/parser.hpp.pch header/main.hpp.pch
PROG=pcp

all: $(PROG)

pcp: $(OBJECTS) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PROG)
	
main.o: main.cpp $(HEADERS)
	$(CC) $(CFLAGS) -include header/main.hpp -o main.o main.cpp
	
parser.o: parser.cpp header/parser.hpp.pch
	$(CC) $(CFLAGS) -include header/parser.hpp -o parser.o parser.cpp

oneStepCd.o: oneStepCd.cpp header/oneStepCd.hpp.pch
	$(CC) $(CFLAGS) -include header/oneStepCd.hpp -o oneStepCd.o oneStepCd.cpp

vns.o: vns.cpp header/vns.hpp.pch
	$(CC) $(CFLAGS) -include header/vns.hpp -o vns.o vns.cpp

header/vns.hpp.pch: header/vns.hpp header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/vns.hpp -o header/vns.hpp.pch

header/pcp.hpp.pch: header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/pcp.hpp -o header/pcp.hpp.pch

header/main.hpp.pch: header/main.hpp header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/main.hpp -o header/main.hpp.pch
	
header/parser.hpp.pch: header/parser.hpp header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/parser.hpp -o header/parser.hpp.pch

header/oneStepCd.hpp.pch: header/oneStepCd.hpp header/pcp.hpp
	$(CC) $(CFLAGS) -x c++-header header/oneStepCd.hpp -o header/oneStepCd.hpp.pch

clean: 
	rm $(PROG) $(OBJECTS)
