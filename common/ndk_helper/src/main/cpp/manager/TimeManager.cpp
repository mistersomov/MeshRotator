#include "TimeManager.hpp"

#include <cstdlib>

constexpr double NANOS_IN_SEC = 1000000000.0;

ndk_helper::timemgr::TimeManager::TimeManager() {
    srand(time(nullptr));
}

double ndk_helper::timemgr::TimeManager::now() {
    timespec timeVal;
    clock_gettime(CLOCK_MONOTONIC, &timeVal);

    return timeVal.tv_sec + timeVal.tv_nsec / NANOS_IN_SEC;
}

float ndk_helper::timemgr::TimeManager::delta() const {
    return deltaTimeTotal_;
}

void ndk_helper::timemgr::TimeManager::update_time() {
    double currentTime = now();
    deltaTimeTotal_ = currentTime - firstTime_;
}

void ndk_helper::timemgr::TimeManager::reset() {
    firstTime_ = now();
}
