#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>
#include "Glew/include/glew.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;

	bool DrawEditor();

	bool CleanUp() override;

	void AddFPS(const float aFPS);
	std::vector<float> mFPSLog;
	int mFPSLogSize = 50;
	
	//Console window
	std::vector <std::string> mConsoleLog;
	bool Console = false;
	void ConsoleLog();

	bool Initializated = false;

	//Glew enable disable
	bool Depth_Test = true, Cull = true, Lightning = true, colorMaterial = true, Texture2D = true;

	//About
	bool About = false;
	void AboutWindow();
	//ImGuiWindowFlags window_about;


};
#endif //MODULE_EDITOR