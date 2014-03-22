#ifndef _SOLUTION_HPP_
#define _SOLUTION_HPP_
#include "pcp.hpp"
#include <memory>
#include <cstdint>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/shared_array.hpp>

namespace pcp {
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
	typedef boost::filtered_graph<Graph, edge_visible, vertex_visible> FilterGraph;

	// Represents a vertex
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	typedef boost::graph_traits<FilterGraph>::vertex_descriptor FVertex;

	// Represents an edge
	typedef boost::graph_traits<Graph>::edge_descriptor Edge;
	typedef boost::graph_traits<FilterGraph>::edge_descriptor FEdge;

	// Standard vertex_iterator
	typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;
	typedef boost::graph_traits<FilterGraph>::vertex_iterator FVertexIter;

	// Standard edge_iterator
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
			Solution();
			//Solution(std::istream& in); avoid this to shorten compilation time
			~Solution();

			Solution& operator=(const Solution& rhs);
			bool operator==(const Solution& rhs);
			bool operator!=(const Solution& rhs);
			bool operator<(const Solution& rhs);
			bool operator<=(const Solution& rhs);
			bool operator>(const Solution& rhs);
			bool operator>=(const Solution& rhs);
			uint32_t operator+(const Solution& rhs);
			uint32_t operator+(const Solution& rhs);

			std::uint32_t getColorDegree(const Vertex& v);
			color_t minPossibleColor(const Vertex& v);
			std::pair<std::uint32_t, color_t> 
						getColorDegreeAndMinColor(const Vertex& v);

			bool isValid();

			inline bool isColored(const Vertex& v) {
				return getColor(v) > -1;
			};

			inline bool isVisible(const Vertex& v) {
				return this->mapped_vertices[v];
			};

			inline void setVisible(const Vertex& v, bool value) {
				this->mapped_vertices[v] = value;
			};

			// Straightforward access to the property map, return partitionID
			inline partition_t getPartition(const Vertex v) {
				return get(boost::vertex_index1_t(), *(this->g), v);
			};

			// Set a specific vertex to a specific color
			inline void setVertexColor(const Vertex& v, color_t color) {
				this->coloring[getPartition(v)] = color;
			};

			// Set the colors used variable
			inline void setColorsUsed(std::int32_t i) {
				this->colors_used = i;
			}

			// Return reference to the filtered graph
			inline FilterGraph& getCurrentSolution() {
				return *this->fg;
			};

			// Return reference to the full graph
			inline Graph& getFullGraph() {
				return *this->g;
			};

			inline std::int32_t getColorsUsed() {
				return this->colors_used;
			};

			inline std::uint32_t getNumPartition() {
				return this->num_parts;
			};

			inline std::uint32_t getNumVertices() {
				return this->num_vertices;
			};

			inline std::uint32_t getNumEdges() {
				return this->num_edges;
			};

			// Straightforward access to color of the vertex
			inline color_t getColor(const Vertex& v) {
				return coloring[getPartition(v)];
			};
			
			inline color_t getPartitionColor(uint32_t partition) {
				return coloring[partition];
			};

			// Return a vector with all nodes of a partition
			inline const std::vector<Vertex>& getPartitionNodes(partition_t t) {
				return part_vertices[t];
			};

			void setPartition(const Vertex v, partition_t part);
			void addEdge(const Vertex& v1, const Vertex& v2);

			void toggleVertex(const Vertex& v);
			void replaceVertex(const Vertex& toRep, const Vertex& nv);
			void detach();

			inline void clearColors() {
				std::fill_n(this->coloring, this->num_parts, -1);
			};

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
