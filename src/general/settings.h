#pragma once
#include "../common.h"
#include <string>

struct Settings
{
	const char* window_title = "Hello World";
	int window_width = 1920;
	int window_height = 1080;
	int vsync = 1;
};

extern Settings settings;