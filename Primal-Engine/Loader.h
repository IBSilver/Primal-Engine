#pragma once

#ifndef LOADER
#define LOADER

#include "Globals.h"
#include <vector>
#include "Glew/include/glew.h"
#include "assimp/include/cimport.h"
#include "assimp/include/scene.h"
#include "assimp/include/postprocess.h"
#include "Glew/Include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "glmath.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct PrimalMesh {
	uint VBO, EBO;
	uint* index = nullptr;
	uint IDindex = 0;
	uint NUMindex = 0;
	float* vertex = nullptr;
	uint IDvertex = 0;
	uint NUMvertex = 0;
};


class Loader
{
public:

	Loader();

	void LoadPrimalMesh(const char* filePath);

	void LoadBuffers();

	void DrawPrimalMeshes();

	bool GetNormals();

	void ChangeNormals();
	
	bool GetUVs();

	void ChangeUVs();

private:
	std::vector<PrimalMesh*> PrimalMeshArray;
	bool Normals = false;
	bool UVs = false;
};

#endif //LOADER
