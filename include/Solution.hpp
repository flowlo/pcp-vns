#ifndef _SOLUTION_HPP_
#define _SOLUTION_HPP_
#include "pcp.hpp"
#include <memory>
#include <cstdint>
#include <algorithm>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/shared_array.hpp>

namespace pcp {
	//! Predicate structure to determine the visibilty of an edge
	/*!
		Predicate structure used by boost's filtered_graph class to determine
		the visibilty of a specific edge in the filtered_graph. An edge is 
		visible if both ends of the edge are visibile
		\sa vertex_visible, FilterGraph
	*/
	struct edge_visible {
		edge_visible() { }
		//! Constructor, to easily store the required graph and bool array
		edge_visible(boost::shared_array<bool> mapped_vertices, std::shared_ptr<Graph> graph) {
			this->mapped_vertices = mapped_vertices;
			this->graph = graph;
		}
		//! Main function which decides if an edge is visible
		/*!
			To determine that an edge is visible, the vertices on both ends have to 
			be visible
			\param edge The edge which needs to be checked
			\return True if edge is visible, false if not
		*/
		template <typename Ed> bool operator()(const Ed& edge) const {
			return mapped_vertices[source(edge, *graph)] && mapped_vertices[target(edge, *graph)];
		}
		//! Bool array which holds true for every visibe vertices
		boost::shared_array<bool> mapped_vertices;
		//! Pointer to the underlying graph, needed to fetch source and target of an edge
		std::shared_ptr<Graph> graph;
	};
	
	//! Predicate structure to determine the visibily of a vertex
	/*!
		Predicate structure used by boost's filtered_graph class to determine
		the visibility of a specific vertex in the filtered_graph. A vertex is 
		visible if the corresponding entry in the underlying bool array is true.
		\sa edge_visible, FilterGraph
	*/
	struct vertex_visible {
		vertex_visible() { }
		//! Constructor, to easily store the bool array
		vertex_visible(boost::shared_array<bool> mapped_vertices) {
			this->mapped_vertices = mapped_vertices;
		}
		//! Main function which determines if a vertex is visible
		/*!
			To determine if a vertex is visible, the corresponding entry in the 
			bool array is checked
			\param vertex The vertex which needs to be checked
			\return True if underlying entry in bool array is true, false otherwise
		*/
		template <typename Ve> bool operator()(const Ve& vertex) const {
			return mapped_vertices[vertex];
		}
		//! Bool array, where each entry represents a vertex in the graph, and its visibilty
		boost::shared_array<bool> mapped_vertices;
	};

	//! A selective representation of an underlying graph
	/*!
		A representation of an underlying full graph. Vertices are only visible,
		if their corresponding entry in the bool array is set to true. 
		\sa edge_visible, vertex_visible
	*/
	typedef boost::filtered_graph<Graph, edge_visible, vertex_visible> FilterGraph;

	//! Data type used to identify a single vertex in the full graph
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	//! Data type used to identidy a single vertex in the filtered graph
	typedef boost::graph_traits<FilterGraph>::vertex_descriptor FVertex;

	//! Data type used to identify a single edge in the full graph
	typedef boost::graph_traits<Graph>::edge_descriptor Edge;
	//! Data type used to identify a single edge in the filtered graph
	typedef boost::graph_traits<FilterGraph>::edge_descriptor FEdge;

	//! Iterator data type to iterate over all vertices of the full graph
	typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;
	//! Iterator data type to iterate over all vertices of the filtered graph
	typedef boost::graph_traits<FilterGraph>::vertex_iterator FVertexIter;

	//! Iterator data type to iterate over all edges of the full graph
	typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;
	//! Iterator data type to iterate over all edges of the filtered graph
	typedef boost::graph_traits<FilterGraph>::edge_iterator FEdgeIter;

	//! Iterator data type to iterate over the adjacent vertices of one vertex in the full graph
	typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
	//! Iterator data type to iterate over the adjacent vertices of one vertex in the filtered graph
	typedef boost::graph_traits<FilterGraph>::adjacency_iterator FAdjIter;

	//! Data type to represent a very compact representation of a solution
	typedef std::vector<std::pair<Vertex, color_t>> condensate;

	//! Class used to represent a single solution to the pc problem
	/*!
		A class representing a single instance of the pc problem. It maintains
		all relevant data to solve the problem. Any data that may be shared
		with other solutions, to the same pc problem, will be shared as much
		as feasible. 
		
		The main components are a pointer to problem graph, another pointer
		to a filtered graph, representing the currently selected nodes of 
		a solution, and a simple array representing the coloring of a 
		solution.
		
		There are many methods to manipulate a solution. If necessary relevant
		data structures shared with other solutions will be copied. 
		
		\sa Graph, FilterGraph
	*/
	class Solution {
		public:
			//! Constructor to easily build up an instance of the pcp
			/*!
				Constructor mainly used to generate the very first representation
				of the pcp. Edges as well as partition ids may be added later.
				\param num_vertices The number of vertices of the full instance
				\param num_partition The number of partitions of the full instance
				\sa setPartition(), addEdge(), Solution(const Solution& copy)), Solution()
			*/
			Solution(std::uint32_t num_vertices, std::uint32_t num_partition);
			//! Copy constructor for solution class
			/*!
				Implementation of a copy constructor for solution class. All 
				data that may be shared with other solution objects will only 
				be copied on demand. 
				\param copy The instance which should be copied
				\sa Solution(num_vertices, num_partition), Solution()
			*/
			Solution(const Solution& copy);
			//! Generate an empty solution
			/*!
				Constructor used to generate an empty instance of the pcp. Mainly
				used to indicate a failure state and/or undesired outcome of an
				operation
				\sa Solution(const Solution& copy)), Solution(std::uint32_t num_vertices, std::uint32_t num_partition)
			*/
			Solution();
			
			//! Destructor for solution objects
			/*!
				Cleans up all allocated memory of this instance. If other instances
				still reference some shared data, that data will not be freed.
			*/
			~Solution();
			
			//! Implementation of the assignment operator
			/*!
				An implementation of the assignment operator optimized to copy
				only what is necessary. If needed, data structures can later be
				copied.
				\sa detach()
			*/
			Solution& operator=(const Solution& rhs);
			
			//! Check solution for equality
			/*!
				Checks wheter to solution objects represent the same solution or not.
				To achive that, the coloring and the selected nodes are compared
				\sa operator!=(const Solution& rhs))
			*/
			bool operator==(const Solution& rhs);
			//! Check solution for inequality
			/*!
				Checks wheter to solution object represent a different solution or not.
				\sa operator=(const Solution& rhs))
			*/
			bool operator!=(const Solution& rhs);
			
			//! Check solution for less colors used
			bool operator<(const Solution& rhs);
			//! Check solution for less or equal colors used
			bool operator<=(const Solution& rhs);
			//! Check solution for more colors used
			bool operator>(const Solution& rhs);
			//! Check solution for more or equal colors used
			bool operator>=(const Solution& rhs);

			//! Get the color degree of a vertex
			/*!
				Returns the number of colored neighbors of a vertex
				\param v The vertex of which the color degree should be calculated
				\return The color degree of v
				\sa getColorDegreeAndMinColor(const Vertex& v)
			*/
			std::uint32_t getColorDegree(const Vertex& v);
			//! Get the minimal possible color of a vertex
			/*!
				Calculate the minimal color of a given vertex, which does not 
				introduce new conflicts with neighbors with the same color.
				\param v The vertex of which the minimal color should be calculated
				\return The minimal conflict-free color of v
				\sa getColorDegreeAndMinColor(const Vertex& v)
			*/
			color_t minPossibleColor(const Vertex& v);
			//! Get the color degree and the minimal possible color
			/*!
				Calculate the minimal possible color as well as the color degree of
				a given vertex
				\param v The vertex of which the minimal color and color degree 
				should be calculated
				\return A pair containing the color degree as well as the minimal 
				possible color
				\sa getColorDegree(const Vertex& v), minPossibleColor(const Vertex& v)
			*/
			std::pair<std::uint32_t, color_t> 
						getColorDegreeAndMinColor(const Vertex& v);
			condensate condense();
			
			//! Checks a solution for validity
			/*!
				Checks wheter this instance is a valid solution of the pcp. It
				checks for misplaced partitions, conflicting coloring as well
				as inconcistency of the coloring scheme.
				\return A pair representing a bool value of validity as well as
				a string containing the reasons why a given solution is invalid.
			*/
			std::pair<bool,std::string> isValid();
			
			//! Checks wheter a given vertex is colored or not
			inline bool isVertexColored(const Vertex& v) {
				return getVertexColor(v) > -1;
			};
			
			//! Checks wheter a given vertex is currently visible in the filtered graph
			inline bool isVisible(const Vertex& v) {
				return this->mapped_vertices[v];
			};
			
			//! Sets a given vertex to visible
			inline void setVisible(const Vertex& v, bool value) {
				this->mapped_vertices[v] = value;
			};
			
			//! Returns the partition id of a given Vertex
			inline partition_t getPartition(const Vertex v) {
				return get(boost::vertex_index1_t(), *(this->g), v);
			};

			//! Set a given vertex (and its partition) to a given color
			inline void setVertexColor(const Vertex& v, color_t color) {
				this->coloring[getPartition(v)] = color;
			};
			
			//! Set a given partition to a given color
			inline void setPartitionColor(partition_t part, color_t color) {
				this->coloring[part] = color;
			};
			
			//! Set the amount of colors used to color the graph
			inline void setColorsUsed(std::int32_t i) {
				this->colors_used = i;
			}

			//! Return reference to the filtered graph
			inline FilterGraph& getCurrentSolution() {
				return *this->fg;
			};

			//! Return reference to the full graph
			inline Graph& getFullGraph() {
				return *this->g;
			};
			
			//! Return the amount of colores used to color the graph
			inline std::int32_t getColorsUsed() {
				return this->colors_used;
			};

			//! Return the number of partitions
			inline std::uint32_t getNumPartition() {
				return this->num_parts;
			};

			//! Return the number of vertices
			inline std::uint32_t getNumVertices() {
				return this->num_vertices;
			};

			//! Return the number of edges
			inline std::uint32_t getNumEdges() {
				return this->num_edges;
			};
			
			//! Return the color of a given vertex
			inline color_t getVertexColor(const Vertex& v) {
				return coloring[getPartition(v)];
			};
			
			//! Return the color of a given partition
			inline color_t getPartitionColor(partition_t partition) {
				return coloring[partition];
			};

			//! Return a vector with all vertices of a given partition
			inline const std::vector<Vertex>& getPartitionNodes(partition_t t) {
				return part_vertices[t];
			};

			//! Add a given vertex to a given partition
			void setPartition(const Vertex v, partition_t part);
			//! Add a new edge between two given vertices
			void addEdge(const Vertex& v1, const Vertex& v2);

			//! Toggle the visibility of a given vertex
			/*!
				Toggles the visibility state of a given vertex.
				A call to this method does trigger a detech-call.
				\param v The vertex of which the visibility should be changed
				\sa detach()
			*/
			void toggleVertex(const Vertex& v);
			
			//! Replace one given vertex with another
			/*!
				Replace a vertex in the solution with another. No checks are made
				wheter this produces a valid state of a solution. This does trigger 
				a detech-call.
				\param toRep The vertex to replace
				\param nv The new vertex which replaces the old one
				\sa detach()
			*/
			void replaceVertex(const Vertex& toRep, const Vertex& nv);
			
			//! Detach the current state of visibility from all other solution
			/*!
				In order to maximize efficiency, different solution objects can 
				share the same bool array representing their chosen vertices.
				A call to detach cuts these ties to other solutions, so that
				vertex changes can be done without side effects
				\sa toggleVertex(const Vertex& v), replaceVertex(const Vertex& toRep, const Vertex& nv)
			*/
			void detach();
			
			//! Clear all colors and reset them
			inline void clearColors() {
				std::fill_n(this->coloring, this->num_parts, -1);
			};

		private:
			//! A shared pointer to a graph representing the whole instance
			std::shared_ptr<Graph> g;
			//! A shared pointer to a filtered graph, representing a specific selection of vertices
			std::shared_ptr<FilterGraph> fg;
			//! A shared bool array, representing a specific selection of vertices
			boost::shared_array<bool> mapped_vertices;
			//! A shared array of vector containing all nodes of a given partition
			boost::shared_array<std::vector<Vertex>> part_vertices;
			//! An array containing the specific coloring of a solution
			color_t* coloring;
			
			std::int32_t colors_used;
			std::uint32_t num_parts;
			std::uint32_t num_vertices;
			std::uint32_t num_edges;
	};
}

#endif
