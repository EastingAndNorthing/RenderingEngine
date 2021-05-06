#pragma once

struct Settings {
	const char* window_title = "Hello World";
	int window_width = 1280;
	int window_height = 720;
	int vsync = 0;
	float fov = 70.0f;
};

extern Settings g_settings;
