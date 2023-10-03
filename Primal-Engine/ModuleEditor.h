#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>

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

	//About
	bool AboutOpen = false;
	ImGuiWindowFlags window_about;

};
#endif //MODULE_EDITOR