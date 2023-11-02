#include "Loader.h"

Loader::Loader() {
	Normals = false;
}

void Loader::LoadPrimalMesh(const char* filePath) {
	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{

		for (int i = 0; i < scene->mNumMeshes; i++) {

			PrimalMesh* PrimalMeshInfo = new PrimalMesh;

			PrimalMeshInfo->NUMvertex = scene->mMeshes[i]->mNumVertices;
			PrimalMeshInfo->vertex = new float[PrimalMeshInfo->NUMvertex * 3];
			memcpy(PrimalMeshInfo->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * PrimalMeshInfo->NUMvertex * 3);
			LOG("New mesh with %d vertices", PrimalMeshInfo->NUMvertex);

			if (scene->mMeshes[i]->HasFaces())
			{
				PrimalMeshInfo->NUMindex = scene->mMeshes[i]->mNumFaces * 3;
				PrimalMeshInfo->index = new uint[PrimalMeshInfo->NUMindex];
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");

					}
					else {
						memcpy(&PrimalMeshInfo->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));

					}
				}
			}
			PrimalMeshArray.push_back(PrimalMeshInfo);
		}

		aiReleaseImport(scene);
	}
	else {
		LOG("Error loading scene % s", filePath);
	}
}

void Loader::LoadBuffers() {

	for (int i = 0; i < PrimalMeshArray.size(); ++i) {

		glGenBuffers(1, (GLuint*)&(PrimalMeshArray.at(i)->VBO));
		glBindBuffer(GL_ARRAY_BUFFER, PrimalMeshArray.at(i)->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * PrimalMeshArray.at(i)->NUMvertex * 3, PrimalMeshArray.at(i)->vertex, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(PrimalMeshArray.at(i)->EBO));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PrimalMeshArray.at(i)->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * PrimalMeshArray.at(i)->NUMindex * 3, PrimalMeshArray.at(i)->index, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Loader::DrawPrimalMeshes() {

	for (int i = 0; i < PrimalMeshArray.size(); ++i) {

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, PrimalMeshArray[i]->VBO);
		glVertexPointer(3, GL_FLOAT, sizeof(float) * 5, NULL);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 5, (void*)(sizeof(float) * 3));
		glBindTexture(GL_TEXTURE_2D, 2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PrimalMeshArray[i]->EBO);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, PrimalMeshArray[i]->NUMindex, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (Normals == true) {
		//Code for showing normals (Couldn't implement it)
	}
}

bool Loader::GetNormals() {
	return Normals;
}

void Loader::ChangeNormals() {
	Normals = !Normals;
}
