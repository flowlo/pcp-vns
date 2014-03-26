AM_CXXFLAGS = -Wall -Werror -std=c++11 -g3

# Debugging Memory with AddressSanitizer
#  * Add CXXFLAGS -fsanitize=address -fno-omit-frame-pointer
#  * export ASAN_OPTIONS="detect_leaks=1:symbolize=1"
#  * export ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer)
#
# TODO: seperate targets for AddressSanitizer and enabled optimiziation

AM_LDFLAGS = -lboost_program_options -lstdc++ -lm

bin_PROGRAMS = pcp-vns

# TODO proper automake subdirectory configuration

pcp_vns_SOURCES = main.cpp \
	Solution.cpp \
	IO.cpp \
	onestepCD.cpp \
	vns.cpp \
	units/base.cpp \
	units/dsatur.cpp \
	units/changeAll.cpp \
	units/changeNode.cpp \
	units/changeColor.cpp
