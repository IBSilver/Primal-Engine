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

#include "DevIL/include/ilut.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/il.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

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


	GLuint ImportTexture(const char* filePath);

public:
	uint VBO, EBO, VAO;
	uint my_id = 0;

	Loader* MeshLoader;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;
	bool ExitApp = true;
	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;
	//Textures
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	GLuint checkerTexture;
	GLuint textureID;
	GLuint IDtex, width, heigth;
};