#pragma once
#include <Windows.h>
#include <cstring>   
#include <string>
#define ApplicationWindow Window;
#define MAX_CLASS_NAME_LENGTH 256
#define HInstance() GetModuleHandle(NULL)

static const INT     WindowWidth = 800;
static const INT     WindowHeight = 600;
class Window
{
public:
	Window(LPCWCHAR ClassName);
	Window(LPCWCHAR ClassName, WNDPROC windowMessageProcess);
	void Register(WNDPROC windowMessageProcess);
	void Register();
	~Window();
	BOOL createWindow();
	BOOL displayWindow();
	BOOL listenMessages();
	BOOL listenMessage(MSG* message);
	BOOL listenMessageWp(MSG* message);
	HWND hWnd;
private:
	WNDCLASSEX WindowClass = { 0 };
	WCHAR WindowClassName[MAX_CLASS_NAME_LENGTH];
};
