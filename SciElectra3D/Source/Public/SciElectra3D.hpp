#pragma once

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600



#include <sstream>
#include <chrono>
#include <list>
#include <vector>
#include <string>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Electra3D.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

using namespace std;
using namespace std::chrono;

#define SE2D_INITIALIZE WM_USER+1
#define DEBUG_TEXT_LENGTH 80
#define DEBUG_TEXT_LIMIT 64
#define SE2D_DEBUG 1
#define HISTANCE GetModuleHandle(NULL)
#define EARTH 5.972e+24f
static const WCHAR fDebugText[] = L"Consolas";
static const FLOAT fDebugSize   = 12;

class SciElectra3D
{
public:
#pragma region Build-Up
    SciElectra3D();
	~SciElectra3D();
    HRESULT InitializeWindow();
	BOOL Start();
	BOOL isDone = false;
#pragma endregion
#pragma region Event

#pragma endregion
#pragma region Debug
	void SetDebugText(const WCHAR* text, unsigned short index);
	void AddDebugText(const WCHAR* text);
	std::wstring debugTexts[DEBUG_TEXT_LIMIT];
	size_t debugTextIterator;
#pragma endregion
#pragma region Window-OpenGL
	HWND hWnd;
	GLFWwindow* window;
	int WIDTH, HEIGHT;
	GLint occlusionCullingSupported;
	unsigned int SyncTick = 1;
	int RegisterWindows();


#pragma endregion
#pragma region Render
	BOOL Render();
	int DrawDebugText();
	int DrawObjects();
#pragma endregion
#pragma region Shaders
	Shader mainShader = Shader("Shaders\\testFragment.hlsl", "Shaders\\testVertex.hlsl",false);
#pragma endregion
#pragma region Universe
	Electra3D electra;
	RECT windowRectangle;
	steady_clock::time_point simulationStartTime, sTime, eTime;
	steady_clock::duration elapsedTime, frameTime;
	double fps,eT;
	Camera camera;
	float zoom = 1;
	float zoomLinearer = 0;
	size_t renderingObjects;
	int WindowRectUpdate();
	bool showGrids = true;
	bool showVectors = true;
	bool showAngles = false;
	bool angleWidthOffset = 20;
	int ShowGrids();
	int ShowVectors();
#pragma endregion
	const int readablecolors[12] = {
		0xFF0000,
		0x800000,
		0xFFFF00,
		0x808000,
		0x00FF00,
		0x008000,
		0x00FFFF,
		0x008080,
		0x0000FF,
		0x000080,
		0xFF00FF,
		0x800080
	};
	bool controlDown;
	bool shiftDown;
	bool mouseLeftDown;
	bool mouseMiddleDown;
	int  deltaMouseWheel;
	POINT mousePos;
	POINT deltaMousePos;
	bool ShowDebugWindow = true,
		ShowSimulationSettings = false,
		ShowGraphicalSettings = false,
		ShowObjectManager = false,
		ShowStyleEditor = false;
	int SelectedObject = -1;
	int hoverObject = -1;
	double simElapsedTime;
	int intap = 0;

	
	
	
private:
	int forDebug;
};

