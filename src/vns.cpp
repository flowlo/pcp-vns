/// This file contains the implementation of VNS
#include "../include/vns-priv.hpp"

using namespace std;
using namespace boost;

namespace pcp {
	Solution *bestSolution;
	
	bool checkValid(Solution* s, Solution* full);
	
	bool TERM_SIGINT = false;
	
	void sigint_catch(int signum) {
		TERM_SIGINT = signum == SIGINT;
	}
	
	/// Implementation of VNS, see vns.hpp
	Solution vnsRun(Solution best, Solution& orig, string units, int unsuccessfulShake, 
						 int shakeStart, int shakeIncrement, int maxTime, bool checkIntermediate, bool checkFinal) {

		return best;
	}
	
	/// validate solutions
	bool checkValid(Solution* s, Solution *full) {
		/*pair<VertexIter, VertexIter> vIter;
		int *parts = new int[s->numParts];
		int *colors = new int[s->numParts];
		pair<AdjIter, AdjIter> aIter;
		VertexPart_Map vParts = get(boost::vertex_index1_t(), *s->g);
		bool valid = true;
		list<Vertex> allOrigIds;
	
		/// Initialize parts and colors
		for (int i = 0; i < s->numParts; i++) {
			parts[i] = 0;
			colors[i] = 0;
		}
	
		/// Check all vertices
		for (vIter = vertices(*s->g); vIter.first != vIter.second; vIter.first++) {
			/// Mark partition and color as used
			parts[vParts[*vIter.first]] = 1;
			colors[s->partition[vParts[*vIter.first]]] = 1;
			
			allOrigIds.push_back(s->getOriginalId(*vIter.first));
			
			/// Check color conflicts
			for (aIter = adjacent_vertices(*vIter.first, *s->g); 
				  aIter.first != aIter.second; aIter.first++) {
			
				if (s->partition[vParts[*aIter.first]] == 
					 s->partition[vParts[*vIter.first]]) {
				
					valid = false;
					cerr<<"Solution is invalid"<<endl;
					cerr<<"Adjacent vertices "<<*aIter.first<<" and "<<*vIter.first;
					cerr<<" have same color "<<s->partition[vParts[*vIter.first]];
					cerr<<endl;
				}
			}
		}
		
		/// Check colorsUsed
		int count = 0;
		for (int i = 0; i < s->numParts; i++) {
			if (colors[i] == 1) {
				count++;
			}
		}
		if (count != s->colorsUsed) {
			valid = false;
			cerr<<"Solution is invalid"<<endl;
			cerr<<"Wrong colorsUsed stored: stored: "<<s->colorsUsed;
			cerr<<", computed: "<<count<<endl;
		}
		
		/// Check partitions and representatives
		for (int i = 0; i < s->numParts; i++) {
			if (parts[i] == 0) {
				valid = false;
				cerr<<"Solution is invalid"<<endl;
				cerr<<"partition "<<i<<" seems to be missing"<<endl;
			}
			if (s->representatives[vParts[i]] != i) {
				valid = false;
				cerr<<"Solution is invalid"<<endl;
				cerr<<"Node "<<i<<" is not representative of partition "<<vParts[i];
				cerr<<endl;
			}
		}
		
		/// Compares the solution to the full graph, to report missing edges, 
		/// wrong adjacency and other bad stuff
		list<Vertex> origAdj;
		list<Vertex> curAdj;
		
		for (vIter = vertices(*s->g); vIter.first != vIter.second; 
			  vIter.first++) {
			
			origAdj.clear();
			curAdj.clear();
			
			Vertex toCompare = *vIter.first;
			Vertex origComp = s->getOriginalId(toCompare);
			
			/// Check if the partitions match
			if (s->getPartition(toCompare) != full->getPartition(origComp)) {
				cerr<<"Vertex "<<toCompare<<" has other partition than the ";
				cerr<<"original vertex "<<origComp<<endl;
			}
			
			/// Fill in all adjacencies in the original graph
			for (aIter = adjacent_vertices(origComp, *full->g); 
				  aIter.first != aIter.second; aIter.first++) {
				
				origAdj.push_back(*aIter.first);
			}
			
			/// Fill in all current adjacencies
			for (aIter = adjacent_vertices(toCompare, *s->g); 
				  aIter.first != aIter.second; aIter.first++) {
				
				curAdj.push_back(*aIter.first);
			}
			
			/// Check if all current adjacencies are part of the original graph
			list<Vertex>::iterator lIter, found;
			for (lIter = curAdj.begin(); lIter != curAdj.end(); lIter++) {
				found = find(origAdj.begin(), origAdj.end(), 
								 s->getOriginalId(*lIter));
								 
				if (found == origAdj.end()) {
					cerr<<"Edge ("<<*lIter<<"|"<<toCompare<<") is not in the";
					cerr<<" original graph"<<endl;
					valid = false;
				}
				else {
					origAdj.erase(found);
				}
			}
			
			/// Check if there are missing adjacencies
			for (lIter = origAdj.begin(); lIter != origAdj.end(); lIter++) {
			
				found = find(allOrigIds.begin(), allOrigIds.end(), *lIter);
				if (found != allOrigIds.end()) {
					cerr<<"Vertex "<<toCompare<<" seems to be an missing edge to ";
					cerr<<"original vertex "<<*found<<endl;
					valid = false;
				}
			}
		}
		delete[] parts;
		delete[] colors;
			
		return valid;*/
		return false;
	}
}

