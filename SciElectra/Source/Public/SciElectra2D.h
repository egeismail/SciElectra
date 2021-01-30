#pragma once
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#define HARDWARE_ACC props.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE
#define CPU props.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT
#define SE2D_INITIALIZE WM_USER+1
#define DEBUG_TEXT_LENGTH 80
#define DEBUG_TEXT_LIMIT 64
#define SE2D_DEBUG 1
#include <d2d1.h>
#include <dwrite.h>
#include <sstream>
#include <chrono>
#include "Window.h"
#include "Electra2D.h"
using namespace std::chrono;
static const WCHAR fDebugText[] = L"Consolas";
static const FLOAT fDebugSize   = 12;
class SciElectra2D
{
public:
    SciElectra2D();
	~SciElectra2D();
    HRESULT InitializeWindow(Window *root);
	BOOL ProcessMsgEvent(MSG msg);
	void ResizeEvent(UINT width, UINT height);
	void SetDebugText(const WCHAR* text, unsigned short index);
	void AddDebugText(const WCHAR* text);
	int DrawDebugText();
	int DrawObjects();
	BOOL Render();
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	BOOL Start();
	static LRESULT ElectraListener(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL isDone = false;
	HWND hWnd;
	Window* root;
	ID2D1Factory* d2d1factory = NULL;
	IDWriteFactory* writeFactory;
	IDWriteTextFormat* textFormat;
	Electra2D electra;
	ID2D1HwndRenderTarget* pRT;
	RECT windowRectangle;
	D2D1_SIZE_F rtSize;
	ID2D1SolidColorBrush* pObject    = NULL;
	ID2D1SolidColorBrush* pBlack     = NULL;
	ID2D1SolidColorBrush* pDebugText = NULL;
	std::wstring debugTexts[DEBUG_TEXT_LIMIT];
	steady_clock::time_point simulationStartTime,sTime, eTime;
	steady_clock::duration elapsedTime, frameTime;
	size_t debugTextIterator;
	D2D1_RENDER_TARGET_PROPERTIES props;
private:
	int forDebug;
};

