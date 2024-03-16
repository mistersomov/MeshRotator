#ifndef MESHROTATOR_ANDROIDOUT_HPP
#define MESHROTATOR_ANDROIDOUT_HPP

#include <android/log.h>
#include <sstream>
#include <string>

extern std::ostream aout;

namespace ndk_helper {
    class AndroidOut : public std::stringbuf {
    public:
        inline AndroidOut(std::string kLogTag) : logTag_(kLogTag) {}

    protected:
        int sync() override {
            __android_log_print(ANDROID_LOG_DEBUG, logTag_.c_str(), "%s", str().c_str());
            str("");
            return 0;
        }

    private:
        std::string logTag_;
    };
}

#endif //MESHROTATOR_ANDROIDOUT_HPP