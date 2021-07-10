#define OEMRESOURCE 
#include <windows.h>    // include the basic windows header file
#include <GL\glew.h>
#include <GL\wglew.h>
#include "./../Public/stb_image.h"
#include "./../Public/Window.hpp"
#include "./../Public/SciElectra3D.hpp"
#include <iostream>
#include <filesystem>


static SciElectra3D simulation;
static Window root(L"SciElectra3D Alpha");

void scenario_test3d(SciElectra3D& simulation,RECT windowRect) {
    simulation.camera = Camera(
        glm::vec3(5.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        70,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        0.1f,
        100.0f);
    const char* md = "TestModels\\Survival_BackPack_2.fbx";
    Model backpack(md);
    Entity backpack_entity(
        glm::vec3(.0f, .0f, .0f),
        glm::vec3(.0f, .0f, .0f),
        glm::vec3(.0f, .0f, .0f),
        10.0f,
        backpack
    );
    simulation.mainShader = Shader("Shaders\\testFragment.hlsl", "Shaders\\testVertex.hlsl");

}
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    simulation.InitializeWindow(&root);
    RECT windowRect;
    GetWindowRect(root.hWnd, &windowRect);
    scenario_test3d(simulation, windowRect);
    return simulation.Start();
}

