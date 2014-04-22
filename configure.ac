AC_INIT([pcp-vns], [0.1], [moritz9422@gmail.com])
AC_PREREQ([2.62])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES([yes])
AM_PATH_CPPUNIT()

AC_LANG_PUSH([C++])
: ${CXXFLAGS=""} 
# Checks for programs.
AC_PROG_CXX([clang gcc g++])

# Checks for libraries.
AC_CHECK_LIB([boost_program_options], [main], [], [AC_MSG_ERROR[boost program options not found]])

# Checks for header files.
AC_CHECK_HEADERS([boost/graph/adjacency_list.hpp], [], 
	[AC_MSG_ERROR(You need the Boost Graph libraries.)])
AC_CHECK_HEADERS([boost/graph/filtered_graph.hpp], [], 
	[AC_MSG_ERROR(You need the Boost Graph libraries.)])
AC_CHECK_HEADERS([boost/shared_array.hpp], [], 
	[AC_MSG_ERROR(You need the Boost Shared Array libraries.)])

# Checks for typedefs, structures, and compiler characteristics.

# Travis does not like this macro
# AC_CHECK_HEADER_STDBOOL

AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="$CXXFLAGS -std=c++11"])
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
AX_CHECK_COMPILE_FLAG([-pedantic], [CXXFLAGS="$CXXFLAGS -pedantic"])

AC_LANG_POP([C++])

AC_OUTPUT(Makefile)
