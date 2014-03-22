AM_CXXFLAGS = -Wall -Werror -std=c++11 -g
AM_LDFLAGS = -lboost_program_options

bin_PROGRAMS = pcp-vns

# TODO proper automake subdirectory configuration

pcp_vns_SOURCES = main.cpp \
	Solution.cpp \
	IO.cpp \
	onestepCD.cpp \
	vns.cpp \
	StoredSolution.cpp \
	units/base.cpp \
	units/dsatur.cpp \
	units/changeAll.cpp \
	units/changeNode.cpp \
	units/changeColor.cpp
