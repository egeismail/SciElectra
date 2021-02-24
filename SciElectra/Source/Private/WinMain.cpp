#define OEMRESOURCE 
#include <windows.h>    // include the basic windows header file
#include "./../Public/WinMain.hpp"
#include "./../Public/Window.hpp"
#include "./../Public/SciElectra2D.hpp"
#include <d2d1.h>
#include <iostream>
// the entry point for any Windows program
static SciElectra2D simulation;
static Window root(L"SciElectra Alpha");
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    simulation.InitializeWindow(&root);
    RECT windowRect;
    GetWindowRect(root.hWnd, &windowRect);
    ObjectCircle circle1, circle2;
    circle1.radius = 15;
    circle2.radius = 15;
    Entity entity1(
        Vector2(0,0),//Vector2(1 * windowRect.right / 2.f, windowRect.bottom / 2.f - circle1.radius / 2),
        Vector2(0, 0),
        14982844642000.8839,
        &circle1,
        DrawTypes::Circle
    ),
           entity2(
        Vector2(200,0),//Vector2(1.1f * windowRect.right / 2.f, windowRect.bottom / 2.f - circle2.radius / 2),
        Vector2(0,1),
        1,
        &circle2,
        DrawTypes::Circle
    );
    simulation.electra.addEntity(entity1);
    simulation.electra.addEntity(entity2);
    return simulation.Start();
}

