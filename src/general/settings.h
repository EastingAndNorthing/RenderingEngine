#pragma once
#include "../common.h"
#include <iostream>

struct Settings
{
	const char* window_title = "Hello World";
	int window_width = 800;
	int window_height = 600;
	int vsync = 1;

	Settings() {
		std::cout << "[SETTINGS] Initialized." << std::endl;
	}
};

extern Settings g_settings;