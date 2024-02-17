#ifndef MESHROTATOR_ANDROIDOUT_H
#define MESHROTATOR_ANDROIDOUT_H

#include <android/log.h>
#include <sstream>
#include <string>

const std::string EMPTY_STRING{""};

extern std::ostream aout;

class AndroidOut : public std::stringbuf {
public:
    inline AndroidOut(std::string kLogTag) : logTag_(kLogTag) {}
protected:
    int sync() override {
        __android_log_print(ANDROID_LOG_DEBUG, logTag_.c_str(), "%s", str().c_str());
        str(EMPTY_STRING);
        return 0;
    }
private:
    std:: string logTag_;
};

#endif //MESHROTATOR_ANDROIDOUT_H