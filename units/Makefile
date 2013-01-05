CC=g++
PCH=
CFLAGS=-Wall -std=c++11 -g -c
LDFLAGS=
HEADER=../header/vns-priv.hpp
OBJECTS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

all:
	@$(MAKE) PCH=.gch unit.a
	
clang:
	@$(MAKE) PCH=.pch CC=clang++ LD=clang++ unit.a

unit.a: $(OBJECTS)
	@echo "ar: Packing objectfiles into "$@
	@ar rs unit.a $(OBJECTS)
	@cp unit.a ../unit.a

%.o: %.cpp $(HEADER)$(PCH)
	@echo $(CC)": Compiling "$@
	@$(CC) $(CFLAGS) -include $(HEADER) $< -o $@
	
$(HEADER)$(PCH): $(HEADER) ../header/pcp.hpp
	@$(CC) $(CFLAGS) -x c++-header ../header/vns-priv.hpp -o ../header/vns-priv.hpp$(PCH)	

clean:
	@echo "rm: Cleaning directory ./units/"
	@rm -f unit.o $(OBJECTS)
