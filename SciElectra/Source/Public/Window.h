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
	~Window();
	BOOL createWindow();
	BOOL displayWindow();
	BOOL listenMessages();
	BOOL listenMessage(MSG* message);
	HWND hWnd;
private:
	WNDCLASSEX WindowClass;
	WCHAR WindowClassName[MAX_CLASS_NAME_LENGTH];
};
