#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([pcp-vns], [0.1], [moritz9422@gmail.com])
AC_PREREQ([2.69])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES([yes])
AM_PATH_CPPUNIT()


AC_LANG_PUSH([C++])
# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([boost/graph/adjacency_list.hpp], [], 
	[AC_MSG_ERROR(You need the Boost Graph libraries.)])
AC_CHECK_HEADERS([boost/graph/filtered_graph.hpp], [], 
	[AC_MSG_ERROR(You need the Boost Graph libraries.)])
AC_CHECK_HEADERS([boost/shared_array.hpp], [], 
	[AC_MSG_ERROR(You need the Boost Shared Array libraries.)])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

AX_CHECK_COMPILE_FLAG([-std=c++11], [
                        CXXFLAGS="$CXXFLAGS -std=c++11"])

AC_LANG_POP([C++])

# Checks for library functions.

AC_OUTPUT(Makefile src/Makefile tests/Makefile)
