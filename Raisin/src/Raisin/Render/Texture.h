#pragma once
#include <string>

struct Texture
{
	int id = -1;
	char mPath[128] = "resources/textures/checker.png";
	int width = -1, heigth = -1, nr_channels = -1;

	Texture() {}
	Texture(const char* _path);
	void Setup();
};
