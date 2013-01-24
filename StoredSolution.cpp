#include "header/StoredSolution.hpp"

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

bool operator==(StoredSolution const& s1, StoredSolution const& s2)
{
	return false;
}

size_t hash_value(StoredSolution const& s) {
	size_t seed = 0;
	hash_combine(seed, s.n);
	hash_combine(seed, s.colors);
	hash_combine(seed, s.representatives);
	return seed;
}
