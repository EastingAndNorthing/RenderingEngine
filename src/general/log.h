#pragma once
#include "common.h"

enum LogLevel { DEBUG, ERROR };

void Log(const std::string& msg, LogLevel&& logLevel = LogLevel::DEBUG, const std::string& origin = "");

void Log(const glm::vec3& vec3, const std::string& name = "");
