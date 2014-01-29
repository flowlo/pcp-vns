#ifndef _SOLUTION_TEST_HPP_
#define _SOLUTION_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>

class SolutionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( SolutionTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testCopyConstructor );
  CPPUNIT_TEST( testAssignOperator );
  CPPUNIT_TEST( testSetPartition );
  CPPUNIT_TEST( testAddEdge );
  CPPUNIT_TEST( testToggleVertex );
  CPPUNIT_TEST( testReplaceVertex );
  CPPUNIT_TEST( testDetach );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testConstructor();
  void testCopyConstructor();
  void testAssignOperator();
  void testSetPartition();
  void testAddEdge();
  void testToggleVertex();
  void testReplaceVertex();
  void testDetach();
};

#endif
