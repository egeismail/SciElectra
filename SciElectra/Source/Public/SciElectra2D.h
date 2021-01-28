#pragma once
#pragma comment(lib, "d2d1")
#define HARDWARE_ACC props.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE;
#define CPU props.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT;
#include <d2d1.h>
#include "Window.h"
#include "Electra2D.h"
class SciElectra2D
{
public:
	SciElectra2D(Window *root);
	~SciElectra2D();
	void test();
	BOOL ProcessMsgEvent(MSG msg);
	BOOL Render();
	BOOL Start();
	BOOL isDone = false;
	HWND hWnd;
	Window* root;
	ID2D1Factory* d2d1factory = NULL;
	Electra2D electra;
private:

};

