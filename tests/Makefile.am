AM_CXXFLAGS = -Wall -Werror -std=c++11 -O2 -g
AM_LDFLAGS = 

TESTS = TestSuite

check_PROGRAMS = TestSuite

TestSuite_SOURCES = SolutionTest.cpp SolutionTest.hpp TestSuite.cpp \
	$(top_builddir)/src/Solution.cpp $(top_builddir)/include/Solution.hpp \
	$(top_builddir)/src/IO.cpp $(top_builddir)/include/IO.hpp
	
TestSuite_CXXFLAGS = $(CPPUNIT_CFLAGS)
TestSuite_LDADD = $(CPPUNIT_LIBS)
