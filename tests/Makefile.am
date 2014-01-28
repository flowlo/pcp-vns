AM_CXXFLAGS = -Wall -Werror -std=c++11 -O2 -g
AM_LDFLAGS = 

TESTS = TestSolution

check_PROGRAMS = TestSolution

TestSolution_SOURCES = SolutionTest.cpp SolutionTest.hpp SolutionMain.cpp \
	$(top_builddir)/src/Solution.cpp $(top_builddir)/include/Solution.hpp
	
TestSolution_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestSolution_LDADD = $(CPPUNIT_LIBS)
