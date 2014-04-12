// This file contains the basic definitions and classes necessary to solve the
// PCP, as well as some useful shortcuts and helper definitions 

#ifndef PCP_H
#define PCP_H
#include <boost/assign/list_of.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/tuple/tuple.hpp>

namespace pcp {
	//! Data type used to identify a partition
	typedef std::uint64_t partition_t;
	
	//! Data type used to identify the color of a partition
	typedef std::int64_t color_t;
	
	//! Extra bit of data stored with every vertex in a graph
	/*!
		Every vertex in the graph stores an partition id to mark the partition
		to which it belongs.
		\sa partition_t
	*/
	typedef boost::property<boost::vertex_index1_t, partition_t> VertexProperty;

	//! Data structure to store an undireceted graph
	/*!
		Instance of boost's adjacency_list. The underlying data structures are a
		vector to store the vertices and another vector to store all edges. 
		Additionally, every vertex stores a partition id
		\sa VertexProperty
	*/
  	typedef boost::adjacency_list<boost::vecS, boost::vecS,
                boost::undirectedS, VertexProperty> Graph;

	//! A collectin of strings, representing rgb code
	const std::vector<std::string> hexColors = boost::assign::list_of("#ff0000")("#00ff00")("#0000ff")("#ffff00")("#00ffff")("#ff00ff")("#ffffff")("#ff8800")("#88ff00")("#0088ff")("#ff0088")("#8800ff")("#00ff88");
}

//! Global variable which indicates the level of verbosity
extern int DEBUG_LEVEL;

#endif
