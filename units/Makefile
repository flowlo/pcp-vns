CXX?=g++
LD=$(CXX)
PCH=.ch
ifeq ($(CXX),clang++)
	PCH=.pch
else ifeq ($(CXX),g++)
	PCH=.gch
endif
CFLAGS=-Wall -std=c++11 -g -c
ifeq ($(TRAVIS),true)
        CFLAGS=-Wall -std=c++0x -g -c
endif
LDFLAGS=-lboost_program_options
OBJECTS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
HEADER=../header/vns-priv.hpp

unit.a: $(OBJECTS)
	@echo "ar: Packing objectfiles into "$@
	@ar rs unit.a $(OBJECTS)
	@cp unit.a ../unit.a

%.o: %.cpp $(HEADER)$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include $(HEADER) $< -o $@
	
$(HEADER)$(PCH): $(HEADER) ../header/pcp.hpp
	@$(CXX) $(CFLAGS) -x c++-header ../header/vns-priv.hpp -o ../header/vns-priv.hpp$(PCH)	

clean:
	@echo "rm: Cleaning directory ./units/"
	@rm -f unit.o $(OBJECTS)

clang:
	@$(MAKE) CXX=clang++ unit.a

gcc:
	@$(MAKE) CXX=g++ unit.a
