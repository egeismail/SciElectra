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
    simulation.CameraPos = Vector2(0, 2700);
    simulation.zoom = 4.17;
    simulation.electra.timeMultiplier = 0.1;
    simulation.electra.resolution = 0.001;
    simulation.ShowObjectManager = true;
    circle1.radius = 25;
    circle2.radius = 2500;
    Entity entity1(
        Vector2(0, 2700),
        Vector2(0, -20),
        25,//904982844642000.8839,
        &circle1,
        DrawTypes::Circle
    ),
        entity2(
            Vector2(-200, 200),
            Vector2(20, -20),
            904982844642000.8839,
            &circle2,
            DrawTypes::Circle
        );
    simulation.electra.addEntity(entity1);
    simulation.electra.addEntity(entity2);
}

void scenario_collision_1(SciElectra2D& simulation) {
    simulation.CameraPos = Vector2(-10000, 0);
    simulation.zoom = 4.17;
    simulation.electra.timeMultiplier = 0.006;
    simulation.electra.resolution = 0.01;
    simulation.ShowObjectManager = true;
    circle1.radius = 4;
    circle2.radius = 10;
    Entity entity1(
        Vector2(-10200, 0),
        Vector2(20, 0),
        10,//904982844642000.8839,
        &circle1,
        DrawTypes::Circle
    ),
        entity2(
            Vector2(0, 0),
            Vector2(0, 0),
            604982844642000.8839,
            &circle2,
            DrawTypes::Circle
        );
    simulation.electra.addEntity(entity1);
    simulation.electra.addEntity(entity2);
}
void scenario_collision_2(SciElectra2D& simulation) {
#define COUNT_OF_ITEMS 20
#define RADIUS 100
    static ObjectCircle circles[COUNT_OF_ITEMS];
    static ObjectCircle circleSun;
    circleSun.radius = 50;
    for (size_t i = 0; i < COUNT_OF_ITEMS; i++)
    {
        circles[i].radius = 1 + 2.0f * rand() / RAND_MAX;;
    }
    float spos = 10;
    simulation.CameraPos = Vector2(0, 0);
    simulation.zoom = 1.75;
    simulation.electra.timeMultiplier = 1;
    simulation.electra.resolution = 0.01;
    simulation.ShowObjectManager = true;

    Entity entities[COUNT_OF_ITEMS] = { };
    Entity entitiySun(
        Vector2(0,0),
        Vector2(0,0),
        EARTH/1000,
        &circleSun,
        DrawTypes::Circle
    );
    for (size_t i = 0; i < COUNT_OF_ITEMS; i++)
    {
        Entity ent(
            Vector2(
                RADIUS * cosf(2 * PI * i / COUNT_OF_ITEMS),
                RADIUS * sinf(2 * PI * i / COUNT_OF_ITEMS)
            ),
            Vector2(0, 0),
            1e+13,
            &circles[i],
            DrawTypes::Circle
        );
        entities[i] = ent;
        simulation.electra.addEntity(entities[i]);
    }
}
int CALLBACK WinMain(HINSTANCE,HINSTANCE,LPSTR,INT){
    simulation.InitializeWindow(&root);
    RECT windowRect;
    GetWindowRect(root.hWnd, &windowRect);
    scenario_collision_0(simulation);
    return simulation.Start();
}

