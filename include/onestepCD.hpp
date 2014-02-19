#ifndef ONE_STEP_H
#define ONE_STEP_H
// #include "pcp.hpp"
#include "Solution.hpp"

namespace pcp {
	Solution onestepCD(Solution& s);
	void removeOthers(FVertex x, Solution& s);
}

#endif
