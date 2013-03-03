#ifndef PILOT_H
#define PILOT_H
#include "pcp.hpp"
#include "oneStepCd.hpp"

namespace pcp {
	Solution *pilot(Solution& s);
	Solution *pilot(Solution& s, int fixed);
}

#endif
