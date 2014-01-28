#include "SolutionTest.hpp"
#include "../include/Solution.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <cstdint>

CPPUNIT_TEST_SUITE_REGISTRATION( SolutionTest );

void SolutionTest :: setUp() { }

void SolutionTest :: tearDown() { } 

void SolutionTest :: testConstructor() {

	pcp::Solution sol(100 , 5);
	
	CPPUNIT_ASSERT_EQUAL(sol.getNumVertices(), (std::uint32_t)100);
	CPPUNIT_ASSERT_EQUAL(sol.getNumPartition(), (std::uint32_t)5);
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol.getFullGraph()), (long unsigned int)100);
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol.getCurrentSolution()), (long unsigned int)100);
}
