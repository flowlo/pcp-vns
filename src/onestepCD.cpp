/// This file contains the implementation of the onestep color-degree
/// constructiion heuristic, see oneStepCd.hpp

#include "../include/onestepCD.hpp"

using namespace boost;
using namespace std;

namespace pcp {

   /// Implementation of onestepCD from oneStepCd.hpp
   Solution onestepCD(Solution& s) {

      /// Number of colors used for the onestepCd solution
      int numColors = 0;

      /// Copy the old solution
      Solution sol(s);
      
      /// Remove all edges between nodes of the same partition, as they are of
      /// no use
      removePartEdges(sol);

      /// Initialize the heuristics' parameters
      int minDegree[s.getNumPartition()];
      int maxDegree, target = -1, cd = 0;
      pair<VertexIter, VertexIter> vp;

      /// Set the partitions color to "not set"
      // fill(sol->partition, sol->partition + sol->numParts, -1);

      /// Repeat until there are no uncolored partitions
      for (int j = 0; j < s.getNumPartition(); j++) {	

         /// Reset the minimal color degree to maximum each iteration
         fill(minDegree, minDegree + s.getNumPartition(), s.getNumPartition() + 1);

         /// Reset the maximum degree
         maxDegree = -1;

         /// For each vertex in the graph
         for (vp = vertices(*sol->g); vp.first != vp.second; ++vp.first) {
            /// Compute the color degree for the vertex
            cd = sol->getColorDegree(*vp.first);

            /// If the color degree of the selected vertex is less than that of
            /// previous vertices in the same partition, and the partition is
            /// uncolored
            if (cd < minDegree[sol->getPartition(*vp.first)] && !sol->isPartitionColored(*vp.first)) {
               /// Set the minimal color degree for the vertex' partition to cd
               minDegree[sol->getPartition(*vp.first)] = cd;

               /// If the vertex' color degree is more then the current maximum
               /// degree, select the current node as the new target
               if (cd > maxDegree) {	
                  maxDegree = cd;
                  target = *vp.first;
               }
            }
         }

         /// Compute the minimal possible color of the target vertex, set the
         /// corresponding partition to that color and remove all other vertices
         /// in the partition
         int color = sol->minPossibleColor(target);
         sol->setPartitionColor(target, color);
         if (numColors < color)
         numColors = color;

         removeOthers(target, *sol);
      }

      /// Fill in the representatives
      for (vp = vertices(*sol->g); vp.first != vp.second; vp.first++)
         sol->representatives[sol->getPartition(*vp.first)] = *vp.first;

      sol->colorsUsed = numColors + 1;

      if (DEBUG_LEVEL > 3) {
         cout << "onestepCD complete!" << endl;
      }

      return sol;
   }

   // Remove all other nodes of a partition for which a representative has
   // already been selected
   // node is the chosen representative
   // s the solution to which removeOthers should be applied
   void removeOthers(FVertex node, Solution& s) {
      partition_t part = s.getPartition(node);
      vector<Vertex>& nodes = s.getPartitionNodes(part);
   
      auto i;
      
      // Toggle all nodes in the same partition
      for (i = nodes.begin(); i < nodes.end(); ++i) {
         if (*i != node) {
            s.toggleNode(*i);
         }
      }
      
      if (DEBUG_LEVEL > 3) {
        cout << "removeOthers complete!" << endl;
      }
   }

   /* DEPRECATED */
   // Remove edges within the same parition
   // s is the solution to which removeEdges should be applied
   void removePartEdges(Solution& s) {
      graph_traits<Graph>::edge_iterator i, end;
      for (tie(i, end) = edges(*s.g); i != end; i++) {
         if (s.getPartition(source(*i, *s.g)) == s.getPartition(target(*i, *s.g))) {
            if (DEBUG_LEVEL > 3) {
               cout << "Removeing edge " << *i << endl;
            }
            remove_edge(*i--, *s.g);
         }
      }

      if (DEBUG_LEVEL > 3) {
         cout << "removePartEdges complete!" << endl;
      }
   }
}
