#pragma once
#include <GL/gl.h>
#include <GL/glu.h>

#include "2639_defs.h"

#include <string>

#define OS_CLOCK_RATE       62500000LL
#define OS_CPU_COUNTER      (OS_CLOCK_RATE*3/4)
#define OS_NSEC_TO_CYCLES(n)    (((u64)(n)*(OS_CPU_COUNTER/15625000LL))/(1000000000LL/15625000LL))
#define OS_USEC_TO_CYCLES(n)    (((u64)(n)*(OS_CPU_COUNTER/15625LL))/(1000000LL/15625LL))
#define OS_CYCLES_TO_NSEC(c)    (((u64)(c)*(1000000000LL/15625000LL))/(OS_CPU_COUNTER/15625000LL))
#define OS_CYCLES_TO_USEC(c)    (((u64)(c)*(1000000LL/15625LL))/(OS_CPU_COUNTER/15625LL))


class Timer {
    private:
        u64 startTime;
        u64 endTime;
    public:
        Timer(std::string name);
        void start();
        void end();

        std::string name;
        u64 elapsedTime;
        f32 elapsed_us;
        f32 elapsed_ms;
        static Timer RegisterTimer(std::string name);
};


extern std::vector<Timer> timerPool;

