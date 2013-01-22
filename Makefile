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
OBJECTS=main.o oneStepCd.o Solution.o vns.o unit.a
HEADERS=header/vns.hpp$(PCH) header/oneStepCd.hpp$(PCH) header/main.hpp$(PCH) header/Solution.hpp$(PCH)
PROG=pcp

$(PROG): $(OBJECTS)
	@echo $(LD)": Linking executable "$(PROG)
	@$(LD) $(OBJECTS) $(LDFLAGS) -o $(PROG)
	
main.o: main.cpp $(HEADERS)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include header/main.hpp -o main.o main.cpp

oneStepCd.o: oneStepCd.cpp header/oneStepCd.hpp$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include header/oneStepCd.hpp -o oneStepCd.o oneStepCd.cpp
	
Solution.o: Solution.cpp header/Solution.hpp$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include header/Solution.hpp -o Solution.o Solution.cpp

vns.o: vns.cpp header/vns-priv.hpp$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include header/vns-priv.hpp -o vns.o vns.cpp
	
unit.a: temp
	@cd units; $(MAKE) $(MFLAGS) unit.a

header/vns-priv.hpp$(PCH): header/vns-priv.hpp header/pcp.hpp header/vns.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header header/vns-priv.hpp -o header/vns-priv.hpp$(PCH)

header/vns.hpp$(PCH): header/vns.hpp header/pcp.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header header/vns.hpp -o header/vns.hpp$(PCH)

header/main.hpp$(PCH): header/main.hpp header/pcp.hpp header/oneStepCd.hpp header/vns.hpp 
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header header/main.hpp -o header/main.hpp$(PCH)

header/oneStepCd.hpp$(PCH): header/oneStepCd.hpp header/pcp.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header header/oneStepCd.hpp -o header/oneStepCd.hpp$(PCH)

header/Solution.hpp$(PCH): header/Solution.hpp header/pcp.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header header/Solution.hpp -o header/Solution.hpp$(PCH)

clean:
	@echo "rm: Cleaning directory ."
	@rm -f $(PROG) $(OBJECTS)
	@cd units; $(MAKE) clean

clean-header:
	@echo "rm: Cleaning precompiled headers"
	@rm -f header/*.pch header/*.gch

.PHONY: temp

clang:
	@$(MAKE) CXX=clang++ $(PROG)

gcc:
	@$(MAKE) CXX=g++ $(PROG)

opt:
	@$(MAKE) CXX=g++ CFLAGS="$(CFLAGS) -O3" $(PROG)
