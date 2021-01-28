#include <windows.h>    // include the basic windows header file
#include "./../Public/WinMain.h"
#include "./../Public/Window.h"
#include "./../Public/SciElectra2D.h"
#include <d2d1.h>
#include <iostream>
// the entry point for any Windows program
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    static Window root(L"SciElectra Alpha");
    static SciElectra2D simulation(&root);
    OutputDebugString(L"A");
    BOOL returnCode = root.createWindow();
    OutputDebugString(L"B");

    if (returnCode != 1) {
        MessageBox(0, L"Failed to create window!.", 0, 0);
    }
    OutputDebugString(L"C");
    returnCode = root.displayWindow();
    if (!returnCode != 1) {
        MessageBox(0, L"Failed to display window!.", 0, 0);
    }
    OutputDebugString(L"D");
    simulation.test(); // Test Scene
    OutputDebugString(L"E");
    return simulation.Start();
}