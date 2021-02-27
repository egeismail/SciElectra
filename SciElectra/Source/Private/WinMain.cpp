#define OEMRESOURCE 
#include <windows.h>    // include the basic windows header file
#include "./../Public/WinMain.hpp"
#include "./../Public/Window.hpp"
#include "./../Public/SciElectra2D.hpp"
#include <d2d1.h>
#include <iostream>
static SciElectra2D simulation;
static Window root(L"SciElectra Alpha");
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    simulation.InitializeWindow(&root);
    RECT windowRect;
    GetWindowRect(root.hWnd, &windowRect);
    simulation.CameraPos = Vector2(0, 0);
    simulation.zoom = 4.17;
    simulation.electra.timeMultiplier = 0.4;
    simulation.electra.resolution = 0.0001;
    simulation.ShowObjectManager = true;
    ObjectCircle circle1, circle2;
    circle1.radius = 5;
    circle2.radius = 5;
    Entity entity1(
        Vector2(20, 40),//Vector2(1 * windowRect.right / 2.f, windowRect.bottom / 2.f - circle1.radius / 2),
        Vector2(-5,-5),
        5,//904982844642000.8839,
        &circle1,
        DrawTypes::Circle
    ),
    entity2(
        Vector2(-20,40),//Vector2(1.1f * windowRect.right / 2.f, windowRect.bottom / 2.f - circle2.radius / 2),
        Vector2(5,-5),
        10,
        &circle2,
        DrawTypes::Circle
    );
    simulation.electra.addEntity(entity1);
    simulation.electra.addEntity(entity2);
    return simulation.Start();
}

