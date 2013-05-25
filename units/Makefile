CXX?=g++
LD=$(CXX)
PCH=.ch
ifeq ($(CXX),clang++)
	PCH=.pch
else ifeq ($(CXX),g++)
	PCH=.gch
endif
CFLAGS=-Wall -std=c++0x -g -c
LDFLAGS=-lboost_program_options
OBJECTS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
HEADER=../include/vns-priv.hpp

unit.a: $(OBJECTS)
	@echo "ar: Packing objectfiles into "$@
	@ar rs unit.a $(OBJECTS)
	@cp unit.a ../unit.a

%.o: %.cpp $(HEADER)$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include $(HEADER) $< -o $@
	
$(HEADER)$(PCH): $(HEADER) ../include/pcp.hpp
	@$(CXX) $(CFLAGS) -x c++-header ../include/vns-priv.hpp -o ../include/vns-priv.hpp$(PCH)	

clean:
	@echo "rm: Cleaning directory ./units/"
	@rm -f unit.a $(OBJECTS)

clang:
	@$(MAKE) CXX=clang++ unit.a

gcc:
	@$(MAKE) CXX=g++ unit.a
