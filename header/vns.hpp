#ifndef VNS_H
#define VNS_H
#include "pcp.hpp"

namespace pcp {
	Solution vnsRun(Solution& best, Solution& orig, int unsuccessfulShake, 
																	int maxTime);
}

#endif
