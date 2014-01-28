#ifndef _SOLUTION_TEST_HPP_
#define _SOLUTION_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>

class SolutionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( SolutionTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testConstructor();
};

#endif
