#ifndef MESHROTATOR_TIMEMANAGER_HPP
#define MESHROTATOR_TIMEMANAGER_HPP

#include <time.h>

namespace ndk_helper {
    namespace timemgr {
        class TimeManager {
            double firstTime_;
            float deltaTimeTotal_;
        public:
            TimeManager() noexcept;

            double now();
            float delta();
            void update_time();
            void reset();
        };
    }
}

#endif //MESHROTATOR_TIMEMANAGER_HPP
