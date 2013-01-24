#include "header/StoredSolution.hpp"

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
