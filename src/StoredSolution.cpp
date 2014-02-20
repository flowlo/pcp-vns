// This file contains all the necessary definitions and declarations to store
// a compressed solution in a hash map
#include "../include/StoredSolution.hpp"

using namespace boost;
using namespace std;
using namespace pcp;

// Construct a StoredSolution from a normal Solution
StoredSolution::StoredSolution(Solution& toStore) {
	this->n = toStore.getNumPartition();
	this->colors = new int[n];
	this->representatives = new int[n];
	
	for (int i = 0; i < n; i++) {
		this->colors[i] = toStore.getColor(i);
		// TODO
		//this->representatives[i] = toStore.getOriginalId(i);
	}
}

// Destructor for a stored solution
StoredSolution::~StoredSolution() {
	delete[] colors;
	delete[] representatives;
}

// Produce a string out of a stored solution for easy humanly driven comparison
string StoredSolution::toString() {
	stringstream ss;
	
	ss << "{ \"representatives\": [ " << this->representatives[0];
	
	for (int i = 1; i < this->n; i++)
		ss << ", " << this->representatives[i];
	
	ss << " ], \"colors\": [ " << this->colors[0];
	for (int i = 1; i < this->n; i++)
		ss << ", " << this->colors[i];
	
	ss << " ] }";
	return ss.str();
}

// calculate a hash by adding the hash values of the two defining array
size_t StoredHash::operator()(StoredSolution const& sol) const
{
	size_t seed = 0;
	hash_range(seed, sol.colors, sol.colors + sol.n);
	hash_range(seed, sol.representatives, sol.representatives + sol.n);
	return seed;
}

// Compare StoredSolution s1 to StoredSolution s2 and return their equality
bool StoredEqual::operator()(StoredSolution const& s1, 
									  StoredSolution const& s2) const
{
	if (s1.n != s2.n)
		return false;
		
	for (int i = 0; i < s1.n; i++)
		if (s1.colors[i] != s2.colors[i] || s1.representatives[i] != s2.representatives[i])
			return false;
			
	return true;
}
