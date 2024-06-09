#ifndef MESHROTATOR_TIMEMANAGER_HPP
#define MESHROTATOR_TIMEMANAGER_HPP

#include <chrono>
#include <cstdlib>

namespace ndk_helper {
    namespace timemgr {
        class TimeManager {
        public:
            TimeManager();

            float getDelta() const;
            float getDeltaTotal() const;
            float getFPS() const;
            void update();
            void reset();

        private:
            using Clock = std::chrono::steady_clock;
            using TimePoint = std::chrono::time_point<Clock>;

            TimePoint firstTime_;
            TimePoint lastTime_;
            float fps_;
            float deltaPerFrame_;
            float deltaTimeTotal_;
        };
    }
}

#endif //MESHROTATOR_TIMEMANAGER_HPP
