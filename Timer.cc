#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>

#include <string>
#include <vector>

#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"
#include "swap.h"

#include "Timer.h"

std::vector<Timer> timerPool;


Timer::Timer(std::string name) {
    this->name = name;

    this->startTime = 0;
    this->endTime = 0;
    this->elapsedTime = 0;
}

void Timer::start() {
    this->startTime = get_ticks();
}

void Timer::end() {
    this->endTime = get_ticks();
    this->elapsed_us = (f32)OS_CYCLES_TO_USEC(this->endTime - this->startTime);
    this->elapsed_ms = this->elapsed_us / 1000.0f;
}

Timer Timer::RegisterTimer(std::string name) {
    timerPool.emplace_back(Timer(name));

    return timerPool.back();
}

