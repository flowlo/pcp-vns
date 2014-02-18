#ifndef _SOLUTION_HPP_
#define _SOLUTION_HPP_

#include <cstdint>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/shared_array.hpp>
#include <memory>

namespace pcp {
	typedef std::uint64_t vertex_t;
	typedef std::uint64_t edge_t;
	typedef std::uint64_t partition_t;
	typedef std::int64_t color_t;
	
	// Stores the partition info for each vertex
	typedef boost::property<boost::vertex_index1_t, partition_t> VertexProperty;
	
	/* TODO: Is this really necessary?
	// Properties that are stored for each edge in the graph
  	typedef boost::property<boost::edge_index_t, int> EdgeProperty; */
  	
  	// The unfiltered full graph type
  	typedef boost::adjacency_list<boost::vecS, boost::vecS,
                boost::undirectedS, VertexProperty> Graph;
	
	// Predicate to select visible edges
	struct edge_visible {
		edge_visible() { }
		edge_visible(boost::shared_array<bool> m_v,
						 std::shared_ptr<Graph> graph) {
			mapped_vertices = m_v;
			g = graph;
		}
		template <typename Ed> bool operator()(const Ed& e) const {
			return mapped_vertices[source(e, *g)] && mapped_vertices[target(e, *g)];
		}
	  	boost::shared_array<bool> mapped_vertices;
	  	std::shared_ptr<Graph> g;
	};
	
	// Predicate to select visible vertices
	struct vertex_visible {
		vertex_visible() { }
		vertex_visible(boost::shared_array<bool> m_v) {
			mapped_vertices = m_v;
		}
		template <typename Ve> bool operator()(const Ve& v) const {
			return mapped_vertices[v];
		}
	  	boost::shared_array<bool> mapped_vertices;
	};
   
   // The filtered graph, only showing visible edges and vertices
   typedef boost::filtered_graph<Graph, edge_visible, 
   										vertex_visible> FilterGraph;
   										
 	// Represents a vertex
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	typedef boost::graph_traits<FilterGraph>::vertex_descriptor FVertex;

	// Represents an edge
	typedef boost::graph_traits<Graph>::edge_descriptor Edge;
	typedef boost::graph_traits<FilterGraph>::edge_descriptor FEdge;

	// Standard vertex_iterator
	typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;
	typedef boost::graph_traits<FilterGraph>::vertex_iterator FVertexIter;

	// Standard vertex_iterator
	typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;
	typedef boost::graph_traits<FilterGraph>::edge_iterator FEdgeIter;

	// Iterator for adjacent vertices
	typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
	typedef boost::graph_traits<FilterGraph>::adjacency_iterator FAdjIter;

	// Stores one Solution, using a filtered graph
	// Provides methods to manipulating the stored Solution
	class Solution {
		public:
			Solution(std::uint32_t num_vertices, std::uint32_t num_partition);
			Solution(const Solution& copy);
			~Solution();
		
			Solution& operator=(const Solution& rhs);
			
			std::int32_t getColorsUsed();
			std::uint32_t getNumPartition();
			std::uint32_t getNumVertices();
			std::uint32_t getNumEdges();
			partition_t getPartition(const Vertex v);
			color_t getColor(const Vertex& v);
			std::uint32_t getColorDegree(const Vertex& v);
			color_t minPossibleColor(const Vertex& v);
			const std::vector<Vertex>& getPartitionNodes(partition_t t);
			std::pair<std::uint32_t, color_t> 
						getColorDegreeAndMinColor(const Vertex& v);
			FilterGraph& getCurrentSolution();
			Graph& getFullGraph();
			
			bool isColored(const Vertex& v);
			bool isVisible(const Vertex& v);
			
			void setVertexColor(const Vertex& v, color_t color);
			
			void setPartition(const Vertex v, partition_t part);
			void addEdge(const Vertex& v1, const Vertex& v2);
			
			void toggleVertex(const Vertex& v);
			void replaceVertex(const Vertex& toRep, const Vertex& nv);
			void detach();
		private:
			std::shared_ptr<Graph> g;
			std::shared_ptr<FilterGraph> fg;
			boost::shared_array<bool> mapped_vertices;
			boost::shared_array<std::vector<Vertex>> part_vertices;
			color_t* coloring;
			
			std::int32_t colors_used;
			std::uint32_t num_parts;
			std::uint32_t num_vertices;
			std::uint32_t num_edges;
	};
}

#endif
