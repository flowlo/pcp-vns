#include "SolutionTest.hpp"
#include "../include/Solution.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <cstdint>

CPPUNIT_TEST_SUITE_REGISTRATION( SolutionTest );

void SolutionTest :: setUp() { }

void SolutionTest :: tearDown() { } 

// Tests the normal contructor for setting the right numbers and initializing
// the correct graph
void SolutionTest :: testConstructor() {

	pcp::Solution sol(100 , 5);
	
	CPPUNIT_ASSERT_EQUAL(sol.getNumVertices(), (std::uint32_t)100);
	CPPUNIT_ASSERT_EQUAL(sol.getNumPartition(), (std::uint32_t)5);
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol.getFullGraph()), 
				(boost::graph_traits<pcp::Graph>::vertices_size_type)100);
	CPPUNIT_ASSERT_EQUAL(boost::num_vertices(sol.getCurrentSolution()), 
				(boost::graph_traits<pcp::FilterGraph>::vertices_size_type)100);
}

// Tests wheter the copy constructor works as intended, with the same checks 
// as normal contructor
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

// Tests the assign operator (=) for correct graphs and numbers
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

// Tests the setPartition() and getPartition() methods
void SolutionTest :: testSetPartition() {
	pcp::Solution sol(20, 3);
	pcp::VertexIter i, end;
	pcp::partition_t p = 0;
	for (boost::tie(i, end) = boost::vertices(sol.getFullGraph()); i != end; 
			i++) {
		if (p % 3 == 0)
			p = 0;
		sol.setPartition(*i, p);
		CPPUNIT_ASSERT_EQUAL(sol.getPartition(*i), p);
	}
}

// Tests the addEdge() method
void SolutionTest :: testAddEdge() {
	pcp::Solution sol(20, 3);
	pcp::VertexIter i, end;
	
	pcp::Graph& g = sol.getFullGraph();
	for (boost::tie(i, end) = boost::vertices(g); i != end-1;) {
		i++;
		sol.addEdge(*i, *(i-1));
	}
	CPPUNIT_ASSERT_EQUAL(sol.getNumEdges(), (std::uint32_t)boost::num_edges(g));
		
	pcp::EdgeIter e, eend;
	for (boost::tie(e, eend) = boost::edges(g); e != eend; e++) {
		CPPUNIT_ASSERT(source(*e, g) - target(*e, g) == -1 ||
			source(*e, g) - target(*e, g) == 1);
	}
}

// Tests the toggleVertex() method
void SolutionTest :: testToggleVertex() {
	pcp::Solution sol(100, 1);
	pcp::VertexIter v, vend;
	pcp::FVertexIter fv, fvend;
	pcp::Graph& g = sol.getFullGraph();
	pcp::FilterGraph& fg = sol.getCurrentSolution();
	int a = 0;
	for (boost::tie(v, vend) = boost::vertices(g); v != vend; v++) {
		sol.toggleVertex(*v);
	}
	
	for (boost::tie(fv, fvend) = boost::vertices(fg); fv != fvend; fv++) {
		CPPUNIT_FAIL("Filtered graph contains vertex");
	}
	
	for (boost::tie(v, vend) = boost::vertices(g); v != vend; v++) {
		sol.toggleVertex(*v);
	}
	
	for (boost::tie(fv, fvend) = boost::vertices(fg); fv != fvend; fv++) {
		a++;
	}
	
	CPPUNIT_ASSERT_EQUAL(a, 100);
}

void SolutionTest :: testReplaceVertex() {
	pcp::Solution sol(100, 1);
	pcp::VertexIter v, vend;
	pcp::FVertexIter fv, fvend;
	pcp::Graph& g = sol.getFullGraph();
	pcp::FilterGraph& fg = sol.getCurrentSolution();
	
	for (boost::tie(v, vend) = boost::vertices(g); v != vend; v+=2) {
		sol.toggleVertex(*v);
	}
	boost::tie(v, vend) = boost::vertices(g);
	v++;
	for (; v+1 != vend; v+=2) {
		sol.replaceVertex(*v, *(v+1));
	}
	for (boost::tie(fv, fvend) = boost::vertices(fg); fv != fvend; fv++) {
		if ((*v % 2) == 0)
			CPPUNIT_FAIL("Filtered graph contains wrong vertex");
	}
}

void SolutionTest :: testDetach() {
	pcp::Solution sol(100, 1);
	pcp::Solution copy(sol);
	pcp::VertexIter v, vend;
	pcp::FVertexIter fv, fvend;
	pcp::Graph& g = sol.getFullGraph();
	pcp::FilterGraph& fg = copy.getCurrentSolution();
	int a = 0;
	CPPUNIT_ASSERT_EQUAL(&copy.getCurrentSolution(),  &sol.getCurrentSolution());
	for (boost::tie(v, vend) = boost::vertices(g); v != vend; v+=2) {
		sol.toggleVertex(*v);
	}
	
	CPPUNIT_ASSERT(&copy.getCurrentSolution() != &sol.getCurrentSolution());
	for (boost::tie(fv, fvend) = boost::vertices(fg); fv != fvend; fv++) {
		a++;
	}
	CPPUNIT_ASSERT_EQUAL(a, 100);
	
	copy = sol;
	a = 0;
	pcp::Graph& g2 = copy.getFullGraph();
	pcp::FilterGraph& fg2 = sol.getCurrentSolution();
	CPPUNIT_ASSERT_EQUAL(&copy.getCurrentSolution(),  &sol.getCurrentSolution());
	
	boost::tie(v, vend) = boost::vertices(g2);
	for (v++; v < vend; v+=2) {
		copy.toggleVertex(*v);
	}
	
	CPPUNIT_ASSERT(&copy.getCurrentSolution() != &sol.getCurrentSolution());
	for (boost::tie(fv, fvend) = boost::vertices(fg2); fv != fvend; fv++) {
		a++;
	}
	CPPUNIT_ASSERT_EQUAL(a, 50);
}
