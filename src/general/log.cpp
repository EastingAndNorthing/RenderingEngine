#include <iostream>
#include "Log.h"

void Log(const char* msg, bool success) {
    printf("%s [RENDERER] %s\n", (success ? "[SUCCESS]" : "ERR"), msg);
}
