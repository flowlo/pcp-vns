/// This file contains the implementation of the onestep color-degree 
/// constructiion heuristic, see oneStepCd.hpp

#include "header/oneStepCd.hpp"

using namespace boost;
using namespace std;

namespace pcp {
	
	/// Implementation of onestepCD from oneStepCd.hpp
	Solution *onestepCD(Solution& s) {
	
		/// Number of colors used for the onestepCd solution
		int numColors = 0;
		
		/// Copy the old solution
		Solution *sol = new Solution(&s);

		/// Remove all edges between nodes of the same partition, as they are of
		/// no use
		removePartEdges(*sol);

		/// Initialize the heuristics' parameters
		int minDegree[s.numParts];
		int maxDegree, target = -1, cd = 0;
		std::pair<VertexIter, VertexIter> vp;
		
		/// Set the partitions color to "not set"
		for (int i = 0; i < sol->numParts; i++)
			sol->partition[i] = -1;
	
		/// Get the property map which stores the partition ID for all vertices
		VertexPart_Map vParts = get(vertex_index1_t(), *sol->g);
	
		/// Repeat until there are no uncolored partitions
		for (int j = 0; j < s.numParts; j++) {	
			
			/// Reset the minimal color degree to maximum each iteration
			for (int i = 0; i < s.numParts; i++)
				minDegree[i] = s.numParts + 1;
			
			/// Reset the maximum degree
			maxDegree = -1;

			/// For each vertex in the graph
		 	for (vp = vertices(*sol->g); vp.first != vp.second; ++vp.first) {
		 		
		 		/// Compute the color degree for the vertex
				cd = colorDegree(*vp.first, *sol);
				
				/// If the color degree of the selected vertex is less than that of
				/// previous vertices in the same partition, and the partition is 
				/// uncolored
				if (cd < minDegree[get(vParts, *vp.first)] && 
					 sol->partition[get(vParts, *vp.first)] == -1) {
				
					/// Set the minimal color degree for the vertex' partition to cd
					minDegree[get(vParts, *vp.first)] = cd;
					
					/// If the vertex' color degree is more then the current maximum
					/// degree, select the current node as the new target
					if (cd > maxDegree) {
						maxDegree = cd;
						target = (*vp.first);
					}
				}
			}
			
			/// Compute the minimal possible color of the target vertex, set the 
			/// corresponding partition to that color and remove all other vertices
			/// in the partition
			int color = minPossibleColor(target, *sol);
			sol->partition[get(vParts, target)] = color;
			if (numColors < color)
				numColors = color;
				
			removeOthers(target, *sol);
		}
		
		/// Fill in the representatives
		for (vp = vertices(*sol->g); vp.first != vp.second; vp.first++) {
			Vertex node = *vp.first;
			sol->representatives[vParts[node]] = node;
		}
		
		sol->colorsUsed = numColors + 1;
		return sol;
	}

	int colorDegree(Vertex node, Solution& s) {
		typename boost::graph_traits<Graph>::adjacency_iterator adj_i, adj_end;
		
		VertexPart_Map vParts = get(vertex_index1_t(), *s.g);
		
		int colored = 0;

		for (tie(adj_i, adj_end) = adjacent_vertices(node, *s.g); adj_i != adj_end; adj_i++)
			if (s.partition[get(vParts, *adj_i)] != -1)
				colored++;

		//cout<<"Color degree of vertex "<<node<<" is "<<colored<<endl;

		return colored;
	}

	int minPossibleColor(Vertex node, Solution& s) {
		VertexPart_Map vParts = get(vertex_index1_t(), *s.g);
		
		int colors[s.numParts];
	
		for (int i = 0; i < s.numParts; i++)
			colors[i] = 0;
	
		typename boost::graph_traits<Graph>::adjacency_iterator iter, last;
	
		for (tie(iter, last) = adjacent_vertices(node, *s.g); iter != last; ++iter)
			if (s.partition[get(vParts, *iter)] != -1)
				colors[s.partition[get(vParts, *iter)]] = 1;
	
		for (int i = 0; i < s.numParts; i++)
			if (colors[i] == 0)
				return i;

		return -1;
	}

	void removeOthers(Vertex node, Solution& s) {
		VertexPart_Map vParts = get(vertex_index1_t(), *s.g);
		
		std::pair<VertexIter, VertexIter> vp;
		
		for (vp = vertices(*s.g); vp.first != vp.second; ++vp.first) {
	 		if (node != *vp.first && 
	 			 get(vParts, *vp.first) == get(vParts, node)) {
	 			
	 			//cout<<"Deleteing vertex "<<*vp.first<<endl;
	 			clear_vertex(*vp.first, *s.g);
	 			remove_vertex(*vp.first, *s.g);

	 			// TODO clean this shit
	 			if (vp.first-- == vp.second - 1)
	 				break;
	 		}
	 	}
	}

	void removePartEdges(Solution& s) {
		VertexPart_Map vParts = get(vertex_index1_t(), *s.g);

	
		graph_traits<Graph>::edge_iterator iter, last;
		for (tie(iter, last) = edges(*s.g); iter != last; ++iter) {
			if (get(vParts, source(*iter, *s.g)) == 
				 get(vParts, target(*iter, *s.g))) {
				remove_edge(*iter--, *s.g);
			}
		}
	}
}
