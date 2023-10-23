#include "Application.h"
#include "Loader.h"

#pragma comment (lib, "assimp/libx86/assimp.lib")

Loader::Loader(Application* app, bool start_enabled) : Module(app, start_enabled) {

}

Loader::~Loader() {

}

bool Loader::Init() {
    //Assimp
    //Stream log messages to Debug window
    struct aiLogStream stream;
    stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
    aiAttachLogStream(&stream);

    const aiScene* scene = aiImportFile("../FBX/Test.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
    if (scene != nullptr && scene->HasMeshes())
    {
        // Use scene->mNumMeshes to iterate on scene->mMeshes array
        for (int i = 0; i < scene->mNumMeshes; i++) {
            OurMeshes[i] = scene->mMeshes[i];
            LOG("New mesh with %d vertices", OurMeshes[i]->mNumVertices);
        }
        LOG("Total meshes loaded: %d ", scene->mNumMeshes);
        NumMeshes = scene->mNumMeshes;
        aiReleaseImport(scene);
    }
    else {
        LOG("Error loading scene");
    }

    // copy vertices
    for (int i = 0; i < NumMeshes; i++) {
        MyMeshes[i].NumVertices = OurMeshes[i]->mNumVertices;
        MyMeshes[i].Vertices = new float[OurMeshes[i]->mNumVertices * 3];
        memcpy(MyMeshes[i].Vertices, OurMeshes[i]->mVertices, sizeof(float) * MyMeshes[i].NumVertices * 3);
        if (OurMeshes[i]->HasFaces()) {
            MyMeshes[i].numIndices = OurMeshes[i]->mNumFaces * 3;
            MyMeshes[i].Indices = new uint[MyMeshes[i].numIndices];
            for (uint j = 0; j < OurMeshes[j]->mNumFaces; ++j)
            {
                if (OurMeshes[i]->mFaces[j].mNumIndices != 3) {
                    LOG("WARNING, geometry face with != 3 indices!");
                }
                else {
                    //memcpy(&m.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
                }
            }
        }
    }
    LOG("Mesh 1 %d indices", MyMeshes[0].Vertices);
    LOG("Mesh 2 %d indices", MyMeshes[1].Vertices);

    return true;
}

bool Loader::CleanUp() {
    //Assimp
    aiDetachAllLogStreams();

    return false;
}
