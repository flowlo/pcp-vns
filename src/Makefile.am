AM_CXXFLAGS = -Wall -Werror -std=c++11 -O2 -g
AM_LDFLAGS = -lboost_program_options

bin_PROGRAMS = pcp-vns

pcp_vns_SOURCES = main.cpp Solution.cpp IO.cpp oneStepCd.cpp vns.cpp $(top_builddir)/include/Solution.hpp \
	$(top_builddir)/include/vns-cpp.hpp $(top_builddir)/include/
