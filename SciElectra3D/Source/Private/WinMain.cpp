#define OEMRESOURCE 
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>    // include the basic windows header file

#include "./../Public/SciElectra3D.hpp"
#include <iostream>
#include <filesystem>


void attach_console()
{
    bool attached = AttachConsole(ATTACH_PARENT_PROCESS) != 0;

    // Only force attach when running a debug build

    if (!attached)
    {
        attached = AllocConsole() != 0;
    }

    if (attached)
    {
        freopen("CON", "w", stdout);
        freopen("CON", "r", stdin);
        freopen("CON", "w", stderr);
    }
}

static SciElectra3D simulation;

void scenario_test3d(SciElectra3D& simulation,RECT windowRect) {
    
    simulation.camera.pos = glm::vec3(5.0f, 0.0f, 0.0f);

    simulation.camera.view = glm::lookAt(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), simulation.camera.cameraUp);
    char md[] = "TestModels\\untitled.obj";
    char fullFilename[MAX_PATH];
    GetFullPathNameA(md, MAX_PATH, fullFilename, nullptr);
    cout << fullFilename << std::endl;
    Model backpack(fullFilename);
    Entity backpack_entity(
        glm::vec3(.0f, .0f, .0f),
        glm::vec3(.0f, .0f, .0f),
        glm::vec3(.0f, .0f, .0f),
        10.0f,
        backpack
    );

    simulation.electra.addEntity(backpack_entity);
    simulation.mainShader = Shader("Shaders\\testVertex.hlsl","Shaders\\testFragment.hlsl");
    if (!simulation.mainShader.vertexIsReady) {
        std::cout << "Vertex shader not found." << std::endl;
    }
    if (!simulation.mainShader.fragmentIsReady) {
        std::cout << "Fragment shader not found."<< std::endl;
    }
    
    if (simulation.mainShader.compile()) {
        std::cout << "Successfully compiled"<< std::endl;
    }
    else {
        std::cout << "Compile failed"<< std::endl;
    }
}
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    attach_console();
    simulation.InitializeWindow();
    
    RECT windowRect;
    GetWindowRect(simulation.hWnd, &windowRect);
    scenario_test3d(simulation, windowRect);
    return simulation.Start();
}

