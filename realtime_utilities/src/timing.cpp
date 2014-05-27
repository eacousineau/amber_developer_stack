#include <sched.h>
#include <stdexcept>

#include <realtime_utilities/timing.hpp>

namespace realtime_utilities
{

void enable_realtime()
{
    // Following from hrt_meka_main.c    
    sched_param param;
    int policy = SCHED_FIFO;
    param.sched_priority = sched_get_priority_max(policy);
    if (sched_setscheduler(0, policy, &param) == -1)
        throw std::runtime_error("sched_setscheduler failed");
}

} // namespace realtime_utilities
