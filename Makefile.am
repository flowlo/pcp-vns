AM_CXXFLAGS = -Wall -Werror -std=c++11
AM_LDFLAGS = -lboost_program_options -lstdc++ -lm

bin_PROGRAMS = pcp-vns
check_PROGRAMS = TestSuite
EXTRA_PROGRAMS = pcp-vns-asan pcp-vns-perform pcp-vns-static pcp-vns-perftools
TESTS = TestSuite

pcp_vns_CXXFLAGS = $(AM_CXXFLAGS) -g3
pcp_vns_SOURCES = \
	src/main.cpp \
	src/Solution.cpp \
	src/IO.cpp \
	src/onestepCD.cpp \
	src/vns.cpp \
	src/units/base.cpp \
	src/units/dsatur.cpp \
	src/units/changeAll.cpp \
	src/units/changeNode.cpp \
	src/units/changeColor.cpp

TestSuite_SOURCES = \
	tests/SolutionTest.cpp \
	tests/TestSuite.cpp \
	tests/IOTest.cpp \
	src/Solution.cpp \
	src/IO.cpp 

TestSuite_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestSuite_LDADD = $(CPPUNIT_LIBS)

# Debugging Memory with AddressSanitizer
#  * export ASAN_OPTIONS="detect_leaks=1:symbolize=1"
#  * export ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer)

pcp_vns_asan_SOURCES = $(pcp_vns_SOURCES)
pcp_vns_asan_CXXFLAGS = $(AM_CXXFLAGS) -g3 -fsanitize=address -fno-omit-frame-pointer

pcp_vns_perform_SOURCES = $(pcp_vns_SOURCES)
pcp_vns_perform_CXXFLAGS = $(AM_CXXFLAGS) -O3

pcp_vns_static_SOURCES = $(pcp_vns_SOURCES)
pcp_vns_static_CXXFLAGS = $(pcp_vns_perform_CXXFLAGS)
pcp_vns_static_LDFLAGS = -static $(AM_LDFLAGS)

pcp_vns_perftools_SOURCES = $(pcp_vns_SOURCES)
pcp_vns_perftools_CXXFLAGS = $(pcp_vns_perform_CXXFLAGS) -O2
pcp_vns_perftools_LDFLAGS = -lprofiler $(AM_LDFLAGS)

asan: pcp-vns-asan
perform: pcp-vns-perform
static: pcp-vns-static
perftools: pcp-vns-perftools
