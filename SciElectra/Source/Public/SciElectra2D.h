#pragma once
#pragma comment(lib, "d2d1")
#define HARDWARE_ACC props.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE
#define CPU props.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT
#include <d2d1.h>
#include "Window.h"
#include "Electra2D.h"



class SciElectra2D
{
public:
    SciElectra2D();
	~SciElectra2D();
    void InitializeWindow(Window *root);
	void test();
	BOOL ProcessMsgEvent(MSG msg);
	HRESULT ResizeEvent();
	BOOL Render();
	BOOL Start();
	static LRESULT ElectraListener(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL isDone = false;
	HWND hWnd;
	Window* root;
	ID2D1Factory* d2d1factory = NULL;
	Electra2D electra;
	ID2D1HwndRenderTarget* pRT;
	RECT windowRectangle;
	ID2D1SolidColorBrush* pOrangeBrush = NULL;
	ID2D1SolidColorBrush* pBlack = NULL;
	D2D1_RENDER_TARGET_PROPERTIES props;
private:
	int forDebug;
};

