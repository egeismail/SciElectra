#define OEMRESOURCE 
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>    // include the basic windows header file

#include "./../Public/SciElectra3D.hpp"
#include <iostream>
#include <filesystem>
#include <math.h>

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
static Entity *targetEntity;
void scenario_test3d(SciElectra3D *simulation) {
    
    simulation->camera.pos = glm::vec3(24.0f, 0.0f, 0.0f);

    simulation->camera.updateCamera();
    static Model nanosuit("TestModels\\gordon\\gordon.obj");

    Entity backpack_entity(
        glm::vec3(.0f, .0f, .0f),
        glm::vec3(.0f, .0f, .0f),
        glm::vec3(.0f, .0f, .0f),
        1.0f,
        10.0f,
        nanosuit
    );
    Light lighten(
        LightType::Diffuse,
        glm::vec3((float)0xfd / (float)0xff, (float)0x70 / (float)0xff, 0.0f));
    Entity light_entity(
        glm::vec3(6.0f, .0f, .0f),
        glm::vec3(.0f, .0f, .0f),
        glm::vec3(.0f, .0f, .0f),
        0.1f,
        10.0f,
        nanosuit,
        lighten
    );
    simulation->electra.addEntity(light_entity);
    targetEntity = simulation->electra.getEntity(simulation->electra.addEntity(backpack_entity));
    simulation->mainShader = Shader("Shaders\\testVertex.hlsl","Shaders\\testFragment.hlsl");
    if (!simulation->mainShader.vertexIsReady) {
        std::cout << "Vertex shader not found." << std::endl;
    }
    if (!simulation->mainShader.fragmentIsReady) {
        std::cout << "Fragment shader not found."<< std::endl;
    }
    
    if (simulation->mainShader.compile()) {
        std::cout << "Successfully compiled"<< std::endl;
    }
    else {
        std::cout << "Compile failed"<< std::endl;
    }

}
static float buffer = 0.0f,
             speed = 1.0f,
             radius = 17.0f;
void onTick(SciElectra3D* simulation) {
    if (targetEntity) {
        targetEntity->pos.x = radius * sinf(buffer * speed);
        targetEntity->pos.y = 130.0f;
        targetEntity->pos.z = radius*cosf(buffer*speed);
        buffer += simulation->eT;
    }
}
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    attach_console();
    simulation.onLoad = scenario_test3d;
    simulation.onTick = onTick;
    simulation.InitializeWindow(); 
    return simulation.Start();
}

