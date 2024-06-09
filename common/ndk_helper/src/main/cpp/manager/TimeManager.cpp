#include "TimeManager.hpp"

ndk_helper::timemgr::TimeManager::TimeManager() {
    srand(static_cast<int>(time(nullptr)));
    reset();
}

float ndk_helper::timemgr::TimeManager::getDelta() const {
    return deltaPerFrame_;
}

float ndk_helper::timemgr::TimeManager::getDeltaTotal() const {
    return deltaTimeTotal_;
}

void ndk_helper::timemgr::TimeManager::update() {
    auto currentTime = Clock::now();
    deltaPerFrame_ = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastTime_).count();
    deltaTimeTotal_ = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - firstTime_).count();
    lastTime_ = currentTime;
    if (deltaPerFrame_ > 0.0f) {
        fps_ = 1.0f / deltaPerFrame_;
    }
}

void ndk_helper::timemgr::TimeManager::reset() {
    firstTime_ = Clock::now();
    lastTime_ = firstTime_;
    fps_ = 0.0f;
    deltaPerFrame_ = 0.0f;
    deltaTimeTotal_ = 0.0f;
}

float ndk_helper::timemgr::TimeManager::getFPS() const {
    return fps_;
}
