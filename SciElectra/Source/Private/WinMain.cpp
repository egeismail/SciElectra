#define OEMRESOURCE 
#include <windows.h>    // include the basic windows header file
#include "./../Public/WinMain.hpp"
#include "./../Public/Window.hpp"
#include "./../Public/SciElectra2D.hpp"
#include <d2d1.h>
#include <iostream>
static SciElectra2D simulation;
static Window root(L"SciElectra Alpha");
static ObjectCircle circle1, circle2;

void scenario_collision_0(SciElectra2D& simulation) {
    simulation.CameraPos = Vector2(0, 0);
    simulation.zoom = 4.17;
    simulation.electra.timeMultiplier = 0.3;
    simulation.electra.resolution = 0.001;
    simulation.ShowObjectManager = true;
    circle1.radius = 5;
    circle2.radius = 2;
    Entity entity1(
        Vector2(0, 200),
        Vector2(0, -20),
        5,//904982844642000.8839,
        &circle1,
        DrawTypes::Circle
    ),
        entity2(
            Vector2(-200, 200),
            Vector2(20, -20),
            5,
            &circle2,
            DrawTypes::Circle
        );
    simulation.electra.addEntity(entity1);
    simulation.electra.addEntity(entity2);
}

void scenario_collision_1(SciElectra2D& simulation) {
    simulation.CameraPos = Vector2(0, 0);
    simulation.zoom = 4.17;
    simulation.electra.timeMultiplier = 0.3;
    simulation.electra.resolution = 0.01;
    simulation.ShowObjectManager = true;
    circle1.radius = 5;
    circle2.radius = 5;
    Entity entity1(
        Vector2(-200, 0),
        Vector2(20, 0),
        5,//904982844642000.8839,
        &circle1,
        DrawTypes::Circle
    ),
        entity2(
            Vector2(0, 0),
            Vector2(0, 0),
            5,
            &circle2,
            DrawTypes::Circle
        );
    simulation.electra.addEntity(entity1);
    simulation.electra.addEntity(entity2);
}
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    simulation.InitializeWindow(&root);
    RECT windowRect;
    GetWindowRect(root.hWnd, &windowRect);
    scenario_collision_0(simulation);
    return simulation.Start();
}

