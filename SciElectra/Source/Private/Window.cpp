#include "../Public/Window.hpp"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}


Window::Window(LPCWCHAR ClassName)
{
    wcscpy_s(this->WindowClassName, ClassName);
}
void Window::Register(WNDPROC windowMessageProcess) {
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

    WindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

    WindowClass.lpszMenuName = nullptr;

    WindowClass.lpszClassName = this->WindowClassName;

    WindowClass.hInstance = HInstance();

    this->WindowClass.lpfnWndProc = *windowMessageProcess;

    RegisterClassEx(&WindowClass);
}
void Window::Register()
{
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

    WindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

    WindowClass.lpszMenuName = nullptr;

    WindowClass.lpszClassName = this->WindowClassName;

    WindowClass.hInstance = HInstance();

    this->WindowClass.lpfnWndProc = WndProc;

    RegisterClassEx(&WindowClass);
}
Window::Window(LPCWCHAR ClassName,WNDPROC windowMessageProcess)
{
    wcscpy_s(this->WindowClassName, ClassName);


    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

    WindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

    WindowClass.lpszMenuName = nullptr;

    WindowClass.lpszClassName = this->WindowClassName;

    WindowClass.hInstance = HInstance();

    this->WindowClass.lpfnWndProc = *windowMessageProcess;

    RegisterClassEx(&WindowClass);
}
BOOL Window::createWindow() {
    this->hWnd = CreateWindow(WindowClassName,
        WindowClassName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        WindowWidth, WindowHeight,
        nullptr, nullptr, HInstance(), nullptr);
    if (!hWnd) {
        MessageBox(0, L"Failed to create window!.", 0, 0);
        return 0;
    }
    return 1;
}
BOOL Window::displayWindow() {
    return ShowWindow(hWnd, SW_SHOW) && UpdateWindow(hWnd);
}
BOOL Window::listenMessages() {
    MSG msg = { 0 };
    BOOL gResult;
    while (msg.message != WM_QUIT)
    {
        if (gResult = PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (gResult == -1) { return gResult; }
    }
    return 0;
}
BOOL Window::listenMessage(MSG* message) {
    MSG msg = { 0 };
    BOOL gResult;
    if (gResult = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        *message = msg;

    }
    return gResult != -1 ? msg.message != WM_QUIT : -1;
}
BOOL Window::listenMessageWp(MSG* message) {
    MSG msg = { 0 };
    BOOL gResult;
    if (gResult = GetMessage(&msg,hWnd,0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    *message = msg;
    return gResult != -1 ? msg.message != WM_QUIT : -1;
}
Window::~Window()
{
    UnregisterClass(WindowClassName, HInstance());
}
