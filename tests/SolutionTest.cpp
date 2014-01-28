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
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol.getFullGraph()), 
				(boost::graph_traits<pcp::Graph>::vertices_size_type)100);
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol.getCurrentSolution()), 
				(boost::graph_traits<pcp::FilterGraph>::vertices_size_type)100);
}

void SolutionTest :: testCopyConstructor() {
	pcp::Solution sol1(100, 20);
	pcp::Solution cp(sol1);	
	
	CPPUNIT_ASSERT_EQUAL(sol1.getNumVertices(), cp.getNumVertices());
	CPPUNIT_ASSERT_EQUAL(sol1.getNumPartition(), cp.getNumPartition());
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol1.getFullGraph()), 
								boost::num_vertices(cp.getFullGraph()));
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol1.getCurrentSolution()), 
								boost::num_vertices(cp.getCurrentSolution()));
	CPPUNIT_ASSERT_EQUAL(&sol1.getFullGraph(), &cp.getFullGraph());
}

void SolutionTest :: testAssignOperator() {
	pcp::Solution sol1(100, 20);
	pcp::Solution cp(20, 3);	
	cp = sol1;
	
	CPPUNIT_ASSERT_EQUAL(sol1.getNumVertices(), cp.getNumVertices());
	CPPUNIT_ASSERT_EQUAL(sol1.getNumPartition(), cp.getNumPartition());
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol1.getFullGraph()), 
								boost::num_vertices(cp.getFullGraph()));
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol1.getCurrentSolution()), 
								boost::num_vertices(cp.getCurrentSolution()));
	CPPUNIT_ASSERT_EQUAL(&sol1.getFullGraph(), &cp.getFullGraph());
	
}

void SolutionTest :: testSetPartition() {
	pcp::Solution sol(20, 3);
	pcp::VertexIter i, end;
	pcp::partition_t p = 0;
	for (boost::tie(i, end) = vertices(sol.getFullGraph()); i != end; i++) {
		if (p % 3 == 0)
			p = 0;
		sol.setPartition(*i, p);
		CPPUNIT_ASSERT_EQUAL(sol.getPartition(*i), p);
	}
}

void SolutionTest :: testAddEdge() {
	pcp::Solution sol(20, 3);
	pcp::VertexIter i, end;
	
	const pcp::Graph& g = sol.getFullGraph();
	for (boost::tie(i, end) = boost::vertices(g); i != end-1;) {
		i++;
		sol.addEdge(*i, *(i-1));
	}
	CPPUNIT_ASSERT_EQUAL((boost::graph_traits<pcp::Graph>::edges_size_type)19,
		boost::num_edges(g));
		
	pcp::EdgeIter e, eend;
	for (boost::tie(e, eend) = boost::edges(g); e != eend; e++) {
		CPPUNIT_ASSERT(source(*e, g) - target(*e, g) == -1 ||
			source(*e, g) - target(*e, g) == 1);
	}
}
