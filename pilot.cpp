/// This file contains the implementation of the onestep color-degree 
/// constructiion heuristic, see oneStepCd.hpp

#include "include/pilot.hpp"

using namespace boost;
using namespace std;

namespace pcp {
	
	Solution *pilot(Solution& s) {
		/// Copy the old solution
		Solution *clean = new Solution(&s);
		clean->requestDeepCopy();
	
		/// Remove all edges between nodes of the same partition, as they are of
		/// no use
		removePartEdges(*clean);
		
		/// Set the partitions color to "not set"
		fill(clean->partition, clean->partition + clean->numParts, -1);
	
		Solution *sol = pilot(*clean, 0);
		
		cout << "Finished internal PILOT." << endl;
		
		/// Fill in the representatives
		pair<VertexIter, VertexIter> vp;
		for (vp = vertices(*sol->g); vp.first != vp.second; vp.first++)
			sol->representatives[sol->getPartition(*vp.first)] = *vp.first;
		
		cout << "Filled in representatives." << endl;
		
		return sol;
	}

	Solution *pilot(Solution& s, int fixed) {
		// cout << "Pilot level " << fixed << " called (of " << s.numParts << ")" << endl;

		if (fixed == s.numParts) {
			cout << "Pilot reached depth " << fixed << ", returning." << endl;

			int colors = -1;
			for (int i = 0; i < s.numParts; i++)
				if (s.partition[i] > colors)
					colors = s.partition[i];

			s.colorsUsed = colors + 1;
			
			return &s;
		}

		/// Initialize the heuristics' parameters
		int minDegree[s.numParts];
		int maxDegree = -1, cd = 0;
		VertexIter i, end;
		
		/// List to store the candidates for recursion
		vector<Vertex> targets;
	
		/// Reset the minimal color degree to maximum each iteration
		fill(minDegree, minDegree + s.numParts, s.numParts + 1);

		/// For each vertex in the graph
	 	for (tie(i, end)= vertices(*s.g); i != end; i++) {
	 		/// Compute the color degree for the vertex
			cd = s.getColorDegree(*i);
			
			/// If the color degree of the selected vertex is less than that of
			/// previous vertices in the same partition, and the partition is 
			/// uncolored
			if (cd < minDegree[s.getPartition(*i)] && !s.isPartitionColored(*i)) {
				/// Set the minimal color degree for the vertex' partition to cd
				minDegree[s.getPartition(*i)] = cd;
				
				/// If the vertex' color degree is more then the current maximum
				/// degree, select the current node as the new target
				if (cd > maxDegree) {
					targets.clear();
						
					maxDegree = cd;
					targets.push_back(*i);
				}
				else if (cd == maxDegree) {
					targets.push_back(*i);
				}
			}
		}
		
		int minColors = s.numParts;
		Solution *best, *sol;
		
		for (vector<Vertex>::iterator i = targets.begin(); i != targets.end(); i++) {
			sol = new Solution(&s);
			sol->requestDeepCopy();

			sol->setPartitionColor(*i, sol->minPossibleColor(*i));
			removeOthers(*i, *sol);
						
			sol = pilot(*sol, fixed + 1);
			
			cout << "Got " << sol->colorsUsed << endl;
			
			if (sol->colorsUsed < minColors) {
				cout << "New minimum: " << sol->colorsUsed << endl;
				best = new Solution(sol);
				best->requestDeepCopy();
			}
		}

		return &s;
	}
}
