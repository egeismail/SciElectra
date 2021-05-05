#define OEMRESOURCE 
#include <windows.h>    // include the basic windows header file
#include <GL\glew.h>
#include <GL\wglew.h>
#include "./../Public/WinMain.hpp"
#include "./../Public/Window.hpp"
#include "./../Public/SciElectra3D.hpp"
#include <iostream>


static SciElectra3D simulation;
static Window root(L"SciElectra Alpha");

void scenario_collision(SciElectra3D& simulation) {

}
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    simulation.InitializeWindow(&root);
    RECT windowRect;
    GetWindowRect(root.hWnd, &windowRect);
    scenario_collision(simulation);
    return simulation.Start();
}

