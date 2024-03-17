#ifndef MESHROTATOR_TIMEMANAGER_HPP
#define MESHROTATOR_TIMEMANAGER_HPP

#include <time.h>

namespace ndk_helper {
    namespace timemgr {
        class TimeManager {
        public:
            static TimeManager& instance();

            double now();
            float delta();
            void update_time();
            void reset();

        private:
            TimeManager();

            double firstTime_{0.0};
            float deltaTimeTotal_{0.0f};
        };
    }
}

#endif //MESHROTATOR_TIMEMANAGER_HPP
