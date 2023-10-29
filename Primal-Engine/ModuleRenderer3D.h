#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Loader.h"

#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void DrawCube();

public:
	uint VBO, EBO, VAO;
	uint my_id = 0;

	GLfloat Vertices1[16][3] = {   // x     y     z
		{-1, -1,  1}, // 1  left    First Strip
		{-1.0,  1.0,  1.0}, // 3
		{-1.0, -1.0, -1.0}, // 0s
		{-1.0,  1.0, -1.0}, // 2
		{ 1.0, -1.0, -1.0}, // 4  back
		{ 1.0,  1.0, -1.0}, // 6
		{ 1.0, -1.0,  1.0}, // 5  right
		{ 1.0,  1.0,  1.0}, // 7
		{ 1.0,  1.0, -1.0}, // 6  top     Second Strip
		{-1.0,  1.0, -1.0}, // 2
		{ 1.0,  1.0,  1.0}, // 7
		{-1.0,  1.0,  1.0}, // 3
		{ 1.0, -1.0,  1.0}, // 5  front
		{-1.0, -1.0,  1.0}, // 1
		{ 1.0, -1.0, -1.0}, // 4  bottom
		{-1.0, -1.0, -1.0}  // 0
	};
	Loader* MeshLoader;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;
	bool ExitApp = true;
	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;
};