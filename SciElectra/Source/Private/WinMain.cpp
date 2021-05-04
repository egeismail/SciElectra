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
    simulation.electra.timeMultiplier = 0.1;
    simulation.electra.resolution = 0.001;
    simulation.ShowObjectManager = true;
    circle1.radius = 25;
    circle2.radius = 25;
    Entity entity1(
        Vector2(0, 200),
        Vector2(0, -20),
        25,//904982844642000.8839,
        &circle1,
        DrawTypes::Circle
    ),
        entity2(
            Vector2(-200, 200),
            Vector2(20, -20),
            25,
            &circle2,
            DrawTypes::Circle
        );
    simulation.electra.addEntity(entity1);
    simulation.electra.addEntity(entity2);
}

void scenario_collision_1(SciElectra2D& simulation) {
    simulation.CameraPos = Vector2(0, 0);
    simulation.zoom = 4.17;
    simulation.electra.timeMultiplier = 0.03;
    simulation.electra.resolution = 0.01;
    simulation.ShowObjectManager = true;
    circle1.radius = 4;
    circle2.radius = 4;
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
void scenario_collision_2(SciElectra2D& simulation) {
    static ObjectCircle circle1, circle2, circle3, circle4, circle5, circle6, circle7;
    float spos = 10;
    simulation.CameraPos = Vector2(-23.7684f, spos +10.823f);
    simulation.zoom = 1.75;
    simulation.electra.timeMultiplier = 0.003;
    simulation.electra.resolution = 0.01;
    simulation.ShowObjectManager = true;
    circle1.radius = spos;
    circle2.radius = 5;
    circle3.radius = 10;
    circle4.radius = 12;
    circle5.radius = 14;
    circle6.radius = 16;
    circle7.radius = 18;
    Entity entity1(
        Vector2(0, 0),
        Vector2(0, 2),
        1904982844642000.8839,
        &circle1,
        DrawTypes::Circle
    ),
        entity2(
            Vector2(-40, spos+20),
            Vector2(0, 0),
            1904982844642000.8839,
            &circle2,
            DrawTypes::Circle
        ),
        entity3(
            Vector2(-30, spos+20),
            Vector2(0, 0),
            1904982844642000.8839,
            &circle3,
            DrawTypes::Circle
        ),
        entity4(
            Vector2(-10, spos+20),
            Vector2(0, 0),
            1904982844642000.8839,
            &circle4,
            DrawTypes::Circle
        ),
        entity5(
            Vector2(8, spos+20),
            Vector2(0, 0),
            1904982844642000.8839,
            &circle5,
            DrawTypes::Circle
        ),
        entity6(
            Vector2(24, spos+20),
            Vector2(0, 0),
            1904982844642000.8839,
            &circle6,
            DrawTypes::Circle
        ),
        entity7(
            Vector2(42, spos+20),
            Vector2(0, 0),
            1904982844642000.8839,
            &circle7,
            DrawTypes::Circle
        );
    simulation.electra.addEntity(entity1);
    simulation.electra.addEntity(entity2);
    simulation.electra.addEntity(entity3);
    simulation.electra.addEntity(entity4);
    simulation.electra.addEntity(entity5);
    simulation.electra.addEntity(entity6);
    simulation.electra.addEntity(entity7);
}
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    simulation.InitializeWindow(&root);
    RECT windowRect;
    GetWindowRect(root.hWnd, &windowRect);
    scenario_collision_0(simulation);
    return simulation.Start();
}

