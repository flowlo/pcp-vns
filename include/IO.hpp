#include "Solution.hpp"
#include <istream>
#include <cstdint>

using namespace std;

namespace pcp {
	Solution readSolution(istream& in);
	Solution readPcpStream(istream& in);
	Solution readColStream(istream& in);
	Solution readColBStream(istream& in);
}
