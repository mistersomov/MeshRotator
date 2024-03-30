#ifndef MESHROTATOR_TIMEMANAGER_HPP
#define MESHROTATOR_TIMEMANAGER_HPP

#include <time.h>

namespace ndk_helper {
    namespace timemgr {
        class TimeManager {
        public:
            TimeManager();

            double now();
            float delta() const;
            void update_time();
            void reset();

        private:
            double firstTime_{0.0};
            float deltaTimeTotal_{0.0};
        };
    }
}

#endif //MESHROTATOR_TIMEMANAGER_HPP
