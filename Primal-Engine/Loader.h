#pragma once

#ifndef LOADER
#define LOADER

#include "Module.h"
#include "Globals.h"
#include <vector>
#include "Glew/include/glew.h"

class Loader : public Module
{
public:

	Loader(Application* app, bool start_enabled = true);
	~Loader();

	bool Init() override;

	bool CleanUp() override;

	std::vector<float> mFPSLog;
	int mFPSLogSize = 50;

	std::vector<const char*> mConsoleLog;
	bool Initializated = false;

	//Assimp
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

};
#endif //LOADER
