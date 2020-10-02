#pragma once

// https://github.com/DylanGuidry95/OpenGL-Lighting/blob/master/src/tiny_obj_loader.h

#include "util/Filesystem.h"

class OBJLoader {
public:

    OBJLoader();
    OBJLoader(const std::string& filepath);
    ~OBJLoader();

};
