#include "Application.h"
#include "assimp/include/cimport.h"
#include "assimp/include/scene.h"
#include "assimp/include/postprocess.h"
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

    //const aiScene* scene = aiImportFile("FBX/Test.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
    //if (scene != nullptr && aiScene > HasMeshes())
    //{
    //    // Use scene->mNumMeshes to iterate on scene->mMeshes array
    //    aiReleaseImport(scene);
    //}
    //else {
    //    LOG("Error loading scene");
    //}

    return true;
}

bool Loader::CleanUp() {
    //Assimp
    aiDetachAllLogStreams();

    return false;
}
