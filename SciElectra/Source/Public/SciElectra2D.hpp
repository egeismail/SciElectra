#pragma once
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#include <d2d1.h>
#include <dwrite.h>
#include <sstream>
#include <chrono>
#include "Window.hpp"
#include "Electra2D.hpp"
using namespace std::chrono;
#define HARDWARE_ACC props.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE
#define CPU props.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT
#define SE2D_INITIALIZE WM_USER+1
#define DEBUG_TEXT_LENGTH 80
#define DEBUG_TEXT_LIMIT 64
#define SE2D_DEBUG 1
#define HISTANCE GetModuleHandle(NULL)

static const WCHAR fDebugText[] = L"Consolas";
static const FLOAT fDebugSize   = 12;
class SciElectra2D
{
public:
#pragma region Build-Up
    SciElectra2D();
	~SciElectra2D();
    HRESULT InitializeWindow(Window *root);
	BOOL Start();
	BOOL isDone = false;
#pragma endregion
#pragma region Event
	BOOL ProcessMsgEvent(MSG msg);
	void ResizeEvent(UINT width, UINT height);
#pragma endregion
#pragma region Debug
	void SetDebugText(const WCHAR* text, unsigned short index);
	void AddDebugText(const WCHAR* text);
	std::wstring debugTexts[DEBUG_TEXT_LIMIT];
	size_t debugTextIterator;
#pragma endregion
#pragma region Window-Direct2D
	HWND hWnd;
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	static LRESULT ElectraListener(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window* root;
	D2D1_RENDER_TARGET_PROPERTIES props;
	ID2D1Factory* d2d1factory = NULL;
	IDWriteFactory* writeFactory;
	IDWriteTextFormat* textFormat;
	ID2D1HwndRenderTarget* pRT;
	ID2D1SolidColorBrush* pObject = NULL;
	ID2D1SolidColorBrush* pBlack = NULL;
	ID2D1SolidColorBrush* pDebugText = NULL;
#pragma endregion
#pragma region Render
	BOOL Render();
	int DrawDebugText();
	int DrawObjects();
#pragma endregion
#pragma region Universe
	Electra2D electra;
	RECT windowRectangle;
	D2D1_SIZE_F rtSize;
	steady_clock::time_point simulationStartTime, sTime, eTime;
	steady_clock::duration elapsedTime, frameTime;
	Vector2 CameraPos;
#pragma endregion

	

	
	
	
private:
	int forDebug;
};

