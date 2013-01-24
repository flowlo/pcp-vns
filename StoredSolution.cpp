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

std::size_t StoredHash::operator()(StoredSolution const& sol) const
{
	return 1;
}

bool StoredEqual::operator()(StoredSolution const& s1, 
									  StoredSolution const& s2) const
{
	return true;
}
