#pragma once
#include "common.h"

enum LogLevel { DEBUG, ERROR };

void Log(const std::string& msg, LogLevel&& logLevel = LogLevel::DEBUG, const std::string& origin = "");
