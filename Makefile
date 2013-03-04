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
OBJECTS=main.o oneStepCd.o pilot.o Solution.o vns.o StoredSolution.o unit.a
HEADERS=include/vns.hpp$(PCH) include/oneStepCd.hpp$(PCH) include/pilot.hpp$(PCH) include/main.hpp$(PCH) include/Solution.hpp$(PCH)
PROG=pcp
BRANCH=`git rev-parse --abbrev-ref HEAD`

$(PROG): $(OBJECTS)
	@echo $(LD)": Linking executable "$(PROG)
	@$(LD) $(OBJECTS) $(LDFLAGS) -o $(PROG)
	
main.o: main.cpp $(HEADERS)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include include/main.hpp -o main.o main.cpp

oneStepCd.o: oneStepCd.cpp include/oneStepCd.hpp$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include include/oneStepCd.hpp -o oneStepCd.o oneStepCd.cpp
	
pilot.o: pilot.cpp include/pilot.hpp$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include include/pilot.hpp -o pilot.o pilot.cpp

Solution.o: Solution.cpp include/Solution.hpp$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include include/Solution.hpp -o Solution.o Solution.cpp

vns.o: vns.cpp include/vns-priv.hpp$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include include/vns-priv.hpp -o vns.o vns.cpp
	
StoredSolution.o: StoredSolution.cpp include/StoredSolution.hpp$(PCH)
	@echo $(CXX)": Compiling "$@
	@$(CXX) $(CFLAGS) -include include/StoredSolution.hpp -o StoredSolution.o StoredSolution.cpp

unit.a: temp
	@cd units; $(MAKE) $(MFLAGS) unit.a

include/vns-priv.hpp$(PCH): include/vns-priv.hpp include/pcp.hpp include/vns.hpp include/StoredSolution.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header include/vns-priv.hpp -o include/vns-priv.hpp$(PCH)

include/StoredSolution.hpp$(PCH): include/StoredSolution.hpp include/pcp.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header include/StoredSolution.hpp -o include/StoredSolution.hpp$(PCH)

include/vns.hpp$(PCH): include/vns.hpp include/pcp.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header include/vns.hpp -o include/vns.hpp$(PCH)

include/main.hpp$(PCH): include/main.hpp include/pcp.hpp include/oneStepCd.hpp include/vns.hpp 
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header include/main.hpp -o include/main.hpp$(PCH)

include/oneStepCd.hpp$(PCH): include/oneStepCd.hpp include/pcp.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header include/oneStepCd.hpp -o include/oneStepCd.hpp$(PCH)

include/pilot.hpp$(PCH): include/pilot.hpp include/pcp.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header include/pilot.hpp -o include/pilot.hpp$(PCH)

include/Solution.hpp$(PCH): include/Solution.hpp include/pcp.hpp
	@echo $(CXX)": Precompiling "$@
	@$(CXX) $(CFLAGS) -x c++-header include/Solution.hpp -o include/Solution.hpp$(PCH)

clean:
	@echo "rm: Cleaning directory ."
	@rm -f $(PROG) $(OBJECTS)
	@cd units; $(MAKE) clean

clean-header:
	@echo "rm: Cleaning precompiled headers"
	@rm -f include/*.pch include/*.gch

.PHONY: temp

clang:
	@$(MAKE) CXX=clang++ $(PROG)

gcc:
	@$(MAKE) CXX=g++ $(PROG)

opt:
	@$(MAKE) CFLAGS="$(CFLAGS) -O3" $(PROG)
	
ubigraph:
	@$(MAKE) LDFLAGS="$(LDFLAGS) -lubigraphclient -lxmlrpc -lxmlrpc_client -lxmlrpc_util" CFLAGS="$(CFLAGS) -Dubigraph" $(PROG)

static:
	@$(MAKE) CFLAGS="$(CFLAGS) -O3" LDFLAGS="$(LDFLAGS) -static" $(PROG)
	
behemoth: clean
	git checkout behemoth
	@$(MAKE) static
	git checkout $(BRANCH)
	@./behemoth.sh
