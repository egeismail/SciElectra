#include <windows.h>    // include the basic windows header file
#include "./../Public/WinMain.h"
#include "./../Public/Window.h"
#include "./../Public/SciElectra2D.h"
#include <d2d1.h>
#include <iostream>
// the entry point for any Windows program
static SciElectra2D simulation;
static Window root(L"SciElectra Alpha");
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    simulation.InitializeWindow(&root);
    simulation.test(); // Test Scene
    return simulation.Start();
}

