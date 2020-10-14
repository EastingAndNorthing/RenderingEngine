#include <iostream>
#include "Log.h"

void Log(const std::string& msg, LogLevel&& logLevel, const std::string& origin) {

    std::string logLevelString;

    switch(logLevel) {
        case LogLevel::DEBUG:
            logLevelString = "NOTICE";
            break;
        case LogLevel::ERROR:
            logLevelString = "ERROR";
            break;
    }

    if(logLevel >= LOG_LEVEL) {
        std::printf("[%s] %s %s\n", logLevelString.c_str(), msg.c_str(), origin.c_str());
    }
}

void Log(const glm::vec3& vec3, const std::string& name) {
    std::cout << name << " " << vec3.x << " " << vec3.y << " " << vec3.z << " " << std::endl;
}
