#pragma once

struct Settings {
	const char* window_title = "Hello World";
	int window_width = 1280;
	int window_height = 720;
	float fov = 70.0f;
	bool vsync = true;
	bool wireframe = false;	// https://vitaliburkov.wordpress.com/2016/09/17/simple-and-fast-high-quality-antialiased-lines-with-opengl/
};

extern Settings g_settings;
