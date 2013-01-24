#include "include/StoredSolution.hpp"

using namespace boost;
using namespace std;
using namespace pcp;

StoredSolution::StoredSolution(Solution& toStore) {
	this->n = toStore.numParts;
	this->colors = new int[n];
	this->representatives = new int[n];
	
	for (int i = 0; i < n; i++) {
		this->colors[i] = toStore.partition[i];
		this->representatives[i] = toStore.getOriginalId(i);
	}
}

StoredSolution::~StoredSolution() {
	delete[] colors;
	delete[] representatives;
}

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

size_t StoredHash::operator()(StoredSolution const& sol) const
{
	size_t seed = 0;
	hash_range(seed, sol.colors, sol.colors + sol.n);
	hash_range(seed, sol.representatives, sol.representatives + sol.n);
	return seed;
}

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
