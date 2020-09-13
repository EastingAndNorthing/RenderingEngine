#pragma once
#include "../common.h"
#include <iostream>

struct Settings {
	const char* window_title = "Hello World";
	int window_width = 1280;
	int window_height = 720;
	int vsync = 1;
	float fov = 70.0f;

	Settings() {
		std::cout << "[SETTINGS] Loaded." << std::endl;
	}
};

extern Settings g_settings;