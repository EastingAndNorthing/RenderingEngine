#include <iostream>
#include "log.h"

void Log(const char* msg, bool success) {
    printf("%s [RENDERER] %s\n", (success ? "[SUCCESS]" : "ERR"), msg);
}
