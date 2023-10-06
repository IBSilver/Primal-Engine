#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>
#include "Glew/include/glew.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;

	bool DrawEditor();

	void AddFPS(const float aFPS);

	bool CleanUp() override;

	std::vector<float> mFPSLog;
	int mFPSLogSize = 50;
	
	std::vector<const char*> mConsoleLog;
	bool Initializated = false;

	//Glew enable disable
	bool Depth_Test = true, Cull = true, Lightning = true, colorMaterial = true, Texture2D = true;

	//About
	bool AboutOpen = false;
	//ImGuiWindowFlags window_about;

};
#endif //MODULE_EDITOR