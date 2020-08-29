#include <iostream>
#include "log.h"

void Log(const char* msg, int prio = 0)
{
    std::cout << msg << std::endl;
}