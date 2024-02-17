#include "TimeManager.hpp"

#include <cstdlib>

ndk_helper::timemgr::TimeManager::TimeManager() noexcept: firstTime_{0.0}, deltaTimeTotal_{0.0} {
    srand(time(nullptr));
}

double ndk_helper::timemgr::TimeManager::now() {
    timespec timeVal;
    clock_gettime(CLOCK_MONOTONIC, &timeVal);

    return timeVal.tv_sec + timeVal.tv_nsec / 1000000000.0;
}

float ndk_helper::timemgr::TimeManager::delta() {
    return deltaTimeTotal_;
}

void ndk_helper::timemgr::TimeManager::update_time() {
    double currentTime = now();
    deltaTimeTotal_ = currentTime - firstTime_;
}

void ndk_helper::timemgr::TimeManager::reset() {
    //aout << "Resetting the time manager" << std::endl;

    firstTime_ = now();
}
