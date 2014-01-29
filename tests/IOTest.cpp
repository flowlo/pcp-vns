#include "IOTest.hpp"
#include "../include/Solution.hpp"
#include "../include/IO.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <sstream>
#include <cstdint>

CPPUNIT_TEST_SUITE_REGISTRATION( IOTest );

using namespace pcp;
void IOTest :: setUp() { 
	this->pcp_in = "10 20 5\n4\n1\n3\n2\n0\n2\n3\n3\n1\n0\n7 9\n5 6\n6 3\n4 3\n5 8\n0 1\n3 6\n2 1\n5 9\n4 8\n1 3\n7 5\n3 1\n2 9\n4 2\n2 3\n9 2\n9 8\n4 5\n8 2\n";
}

void IOTest :: tearDown() { }

void IOTest :: testPcpInput() {

	std::stringstream s(this->pcp_in);
	Solution test = readPcpStream(s);
	
	CPPUNIT_ASSERT_EQUAL((int)test.getNumVertices(), (int)10);
	CPPUNIT_ASSERT_EQUAL((int)test.getNumPartition(), (int)5);
	CPPUNIT_ASSERT_EQUAL((int)test.getNumEdges(), (int)20);
	
	CPPUNIT_ASSERT_EQUAL(test.getPartition((Vertex)0), (partition_t)4);
	CPPUNIT_ASSERT_EQUAL(test.getPartition((Vertex)9), (partition_t)0);
	
	EdgeIter e, eend;
	bool t1 = false;
	bool t2 = false;
	bool t3 = false;
	Graph& g = test.getFullGraph();
	for (boost::tie(e, eend) = boost::edges(g); e != eend; e++) {
		if ((boost::source(*e, g) == (Vertex)7 && boost::target(*e, g) == (Vertex)9)
			|| (boost::source(*e, g) == (Vertex)9 && boost::target(*e, g) == (Vertex)7))
				t1 = true;
		if ((boost::source(*e, g) == (Vertex)6 && boost::target(*e, g) == (Vertex)3)
			|| (boost::source(*e, g) == (Vertex)3 && boost::target(*e, g) == (Vertex)6))
				t2 = true;
		if ((boost::source(*e, g) == (Vertex)1 && boost::target(*e, g) == (Vertex)0)
			|| (boost::source(*e, g) == (Vertex)0 && boost::target(*e, g) == (Vertex)1))
				t3 = true;
	}
	CPPUNIT_ASSERT(t1);
	CPPUNIT_ASSERT(t2);
	CPPUNIT_ASSERT(t3);
}
