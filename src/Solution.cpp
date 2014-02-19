#include "../include/Solution.hpp"

using namespace std;

namespace pcp {
	Solution :: Solution(uint32_t num_vertices, uint32_t num_partition) {
		shared_ptr<Graph> full(new Graph(num_vertices));
		boost::shared_array<bool> mapped(new bool[num_vertices]);
		
		// Construct the predicates for the filtered graph based on the bool array
		edge_visible edges(mapped, full);
		vertex_visible vertices(mapped);
		shared_ptr<FilterGraph> fg(new FilterGraph(*full, edges, vertices));
		this->fg = fg;
		this->mapped_vertices = mapped;
		
		boost::shared_array<vector<Vertex>> vparts(new vector<Vertex>[num_partition]);
		this->part_vertices = vparts;
		
		this->coloring = new color_t[num_partition];
		
		this->g = full;
		this->num_parts = num_partition;
		this->num_vertices = num_vertices;
		this->num_edges = 0;
		this->colors_used = -1;
		
		fill_n(mapped.get(), num_vertices, true);
		fill_n(coloring, num_partition, -1);
	}
	
	Solution::Solution() {
		this->num_edges = this->num_vertices = this->num_parts = 0;
		this->colors_used = -1;
	}

	Solution :: Solution(const Solution& cp) {
		// Copy all const values
		this->g = cp.g;
		this->fg = cp.fg;
		this->num_parts = cp.num_parts;
		this->num_edges = cp.num_edges;
		this->num_vertices = cp.num_vertices;
		this->colors_used = cp.colors_used;
		this->part_vertices = cp.part_vertices;
		
		// Add new coloring array, and copy the old values
		this->coloring = new color_t[this->num_parts];
		copy(cp.coloring, cp.coloring + num_parts, this->coloring);
		
		// Use old mapping array, add one manually later
		boost::shared_array<bool> mapped(cp.mapped_vertices);
		this->mapped_vertices = mapped;
	}
	
	Solution :: ~Solution() {
		// Deallocate color array, smart pointers do the rest
		delete[] coloring;
	}

	Solution& Solution :: operator=(const Solution& rhs) {
		// Check for weird assignments
		if (this == &rhs)
			return *this;
		
		if (this->num_parts != rhs.num_parts) {
			delete[] this->coloring;
			this->coloring = new color_t[rhs.num_parts];
		}
		
		// Copy const values
		this->g = rhs.g;
		this->fg = rhs.fg;
		this->num_parts = rhs.num_parts;
		this->num_edges = rhs.num_edges;
		this->num_vertices = rhs.num_vertices;
		this->colors_used = rhs.colors_used;
		this->part_vertices = rhs.part_vertices;
		
		// copy coloring values from rhs
		copy(rhs.coloring, rhs.coloring + num_parts, this->coloring);
		
		// Use old mapping array, add one manually later
		boost::shared_array<bool> mapped(rhs.mapped_vertices);
		this->mapped_vertices = mapped;
		return *this;
	}
	
	// Straightforward access to the property map, return partitionID
	partition_t Solution :: getPartition(const Vertex v) {
		return get(boost::vertex_index1_t(), *(this->g), v);
	}
	
	// Straightforward access to color of the vertex
	color_t Solution :: getColor(const Vertex& v) { 
		return coloring[getPartition(v)];
	}
	
	// Step through all adjacent vertices !IN THE FILTERED GRAPH! and count the 
	// number of colored vertices
	uint32_t Solution :: getColorDegree(const Vertex& v) {
		uint32_t colored = 0;
		FAdjIter i, end;
		
		for (tie(i, end) = adjacent_vertices(v, *this->fg); i != end; i++) 
			if (this->isColored(*i))
				colored++;
		
		return colored;
	}
	
	// Step through all adjacent vertices !IN THE FILTERED GRAPH! and check for
	// the min color possible
	color_t Solution :: minPossibleColor(const Vertex& v) {
		bool* colors = new bool[this->num_parts];
		fill_n(colors, this->num_parts, false);
		FAdjIter i, end;

		color_t c;
		// Mark all colors which are used by neighbors of node
		for (tie(i, end) = adjacent_vertices(v, *this->fg); i != end; i++)
			if ((c = getColor(*i)) >= 0)
				colors[c] = true;

		// find the first unused color
		for (uint32_t i = 0; i < this->num_parts; i++) {
			if (!colors[i]) {
					delete[] colors;
					return i;
			}
		}

		// This cant happen, but maybe it will...
		delete[] colors;
		return -1;
	}
	
	// Return a vector with all nodes of a partition
	const std::vector<Vertex>& Solution :: getPartitionNodes(partition_t t) {
	   return part_vertices[t];
	}
	
	// Basically a combination of minPossibleColor() and getColorDegree() to 
	// speed things up
	pair<uint32_t, color_t> Solution :: getColorDegreeAndMinColor
																			(const Vertex& v) {
		bool* colors = new bool[this->num_parts];
		color_t c;
		uint32_t colored = 0;
		pair<uint32_t, color_t> ret;
		fill_n(colors, this->num_parts, false);
		FAdjIter i, end;

		// Mark all colors which are used by neighbors of node
		for (tie(i, end) = adjacent_vertices(v, *this->fg); i != end; i++) {
			if ((c = getColor(*i)) >= 0) {
				colors[c] = true;
				colored++;
			}
		}

		// find the first unused color
		for (color_t i = 0; i < this->num_parts; i++) {
			if (!colors[i]) {
					delete[] colors;
					ret.first = colored;
					ret.second = i;
					return ret;
			}
		}
		
		// This cant happen, but maybe it will...
		ret.first = 0;
		ret.second = -1;
		delete[] colors;
		return ret;
	}

	// Check wheter a vertex is colored or not
	bool Solution :: isColored(const Vertex& v) {
		return getColor(v) > -1;
	}
	
	bool Solution :: isVisible(const Vertex& v) {
		return this->mapped_vertices[v];
	}

	// Set a specific vertex to a specific color
	void Solution :: setVertexColor(const Vertex& v, color_t color) {
		this->coloring[getPartition(v)] = color;
	}

	// Set an previously unset vertices to a specific partition and 
	// push it in the correspondig vector for easy access
	void Solution :: setPartition(const Vertex v, partition_t part) {
		put(boost::vertex_index1_t(), *(this->g), v, part);
		this->part_vertices[part].push_back(v);
	}
	
	// Add an edge to the graph, visibility depends on the vertices
	void Solution :: addEdge(const Vertex& v1, const Vertex& v2) {
		add_edge(v1, v2, *this->g);
		this->num_edges++;
	}

	// Toggle the visibility of a vertex in the filtered graph
	void Solution :: toggleVertex(const Vertex& v) {
		this->detach();
		this->mapped_vertices[v] = !this->mapped_vertices[v];
	}

	// Replace on vertex with the other, done through toggeling visibilty	
	void Solution :: replaceVertex(const Vertex& toRep, const Vertex& nv) {
		this->toggleVertex(toRep);
		this->toggleVertex(nv);
	}
	
	// Detach the current maping from the others
	void Solution :: detach() {
		if (this->mapped_vertices.use_count() > 1) {
			boost::shared_array<bool> mapped(new bool[this->num_vertices]);
			copy(this->mapped_vertices.get(), 
				  this->mapped_vertices.get() + this->num_vertices,
				  mapped.get());
			this->mapped_vertices = mapped;
			
			// Renew the filtered graph to use the new mapping
			edge_visible edges(mapped, this->g);
			vertex_visible vertices(mapped);
			shared_ptr<FilterGraph> fg(new FilterGraph(*this->g, edges, vertices));
			this->fg = fg;
		}
	}
	
	// Return reference to the filtered graph
	FilterGraph& Solution :: getCurrentSolution() {
		return *this->fg;
	}
	
	// Return reference to the full graph
	Graph& Solution :: getFullGraph() {
		return *this->g;
	}
	
	std::int32_t Solution :: getColorsUsed() {
		return this->colors_used;
	}
	
	std::uint32_t Solution :: getNumPartition() {
		return this->num_parts;
	}
	
	std::uint32_t Solution :: getNumVertices() {
		return this->num_vertices;
	}
	
	std::uint32_t Solution :: getNumEdges() {
		return this->num_edges;
	}
}
