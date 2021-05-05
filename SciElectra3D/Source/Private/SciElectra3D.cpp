#include "../Public/SciElectra3D.hpp"
#define DFC(x) (forDebug & x) == 0 
#define DF(x)   forDebug |= x

//Window* root
#pragma region Initializations & Destroy
SciElectra3D::SciElectra3D()
{

}

SciElectra3D::~SciElectra3D()
{
    ClipCursor(NULL);
    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hWnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);
}

HRESULT SciElectra3D::InitializeWindow(Window* root)
{
    BOOL lr = true;
    /*Window Initialization*/
    root->Register((WNDPROC)this->ElectraListener);
    BOOL returnCode = root->createWindow();

    if (returnCode != 1) {
        MessageBox(0, L"Failed to create window!.", 0, 0);
    }
    lr = lr && returnCode;
    returnCode = root->displayWindow();
    if (!returnCode != 1) {
        MessageBox(0, L"Failed to display window!.", 0, 0);
    }
    lr = lr&&returnCode;
    this->root = root;
    hDC = GetDC(root->hWnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
        MessageBoxEx(NULL, L"ChoosePixelFormat() failed: Cannot find a suitable pixel format.", L"Error", MB_OK, 0);
        return S_FALSE;
    }

    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
        MessageBoxEx(NULL, L"SetPixelFormat() failed: Cannot set format specified.", L"Error", MB_OK, 0);
        return S_FALSE;
    }

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    
    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    InitGLObjects();

    /*Debug Text*/
//#if SE2D_DEBUG
//    this->AddDebugText(TEXT("SciElectra Alpha"));
//    this->AddDebugText(TEXT("FPS          : 0"));
//    this->AddDebugText(TEXT("Elapsed Time : 0"));
//#endif
    /*Configure ImDui Colors*/
    return lr ? S_OK : S_FALSE;
}
void SciElectra3D::InitGLObjects() {
    //Initialize extensions
    glewInit();

    //Basic global variables
    glClearColor(0.6f, 0.9f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    //Check GL functionality
    glGetQueryiv(GL_SAMPLES_PASSED_ARB, GL_QUERY_COUNTER_BITS_ARB, &occlusionCullingSupported);

    //Attempt to enalbe vsync (if failure then oh well)
    wglSwapIntervalEXT(1);

}
#pragma endregion
#pragma region Loops



float cubicBezier(float y1, float y2, float normalized) {
    return 3 * normalized * pow((1 - normalized), 2) * y1 + 3 * pow(normalized, 2) * (1 - normalized) * y2 + pow(normalized, 3);
}
POINT SciElectra3D::WorldToScreen(Vector3 Pos)
{
    POINT p;
    p.x = 0;
    p.y = 0;
    return p;
}
POINT SciElectra3D::WorldToScreen_bc(Vector3 Pos,Vector3 Camera)
{
    POINT p;
    p.x = 0;
    p.y = 0;
    return p;
}
Vector3 SciElectra3D::ScreenToWorld(POINT Pos)
{
    return Vector3(0, 0, 0);
}
Vector3 SciElectra3D::ScreenToWorld_bc(POINT Pos,Vector3 Camera)
{
    return Vector3(0, 0, 0);
}
Vector3 SciElectra3D::TransformWTS(POINT Pos)
{
    return Vector3(0, 0,0);
}
float SciElectra3D::ScreenToWorldX(long x)
{
    return 0;
}
float SciElectra3D::ScreenToWorldY(long y)
{
	return 0;
}
int SciElectra3D::WindowRectUpdate()
{
    return 0;
}
float SeqA051109(unsigned int n) {
    return ((n % 3) * (n % 3) + 1) * pow(10, int(n / 3));
}
int SciElectra3D::ShowGrids() {
    if (!showGrids)
        return 0;
    
    return 0;
}
int SciElectra3D::ShowVectors()
{
	if (!showVectors)
		return 0;
    
    return 0;
}
BOOL SciElectra3D::Start()
{
    simulationStartTime = steady_clock::now();
    steady_clock::time_point lastRender = steady_clock::now();
    while (!this->isDone) {
		sTime = steady_clock::now();

        /*Events*/
        MSG msg;
        BOOL hResult = this->root->listenMessage(&msg);
        if (hResult == 0)
            return 0;/*WM_QUIT*/
        if (hResult == -1)
            return -1;/*ERROR*/
        
        /*Interactions*/
        this->Render();
        eTime = steady_clock::now();
        frameTime = eTime - sTime;
        elapsedTime = eTime-simulationStartTime;
        eT = (duration_cast<microseconds>(frameTime).count() / 10e+5f);
        fps = 1.f / (duration_cast<microseconds>(frameTime).count() / 10e+5f);
        simElapsedTime = (duration_cast<microseconds>(elapsedTime).count() / 10e+5f);
		/*Physics*/
        /*SyncTick = (unsigned int)(eT / electra.tickTimef);
		for (int tickIteration = 0; tickIteration < SyncTick; tickIteration++)
		{
			this->electra.Tick();
		}*/
//#endif
    }
    return 0;
}
/*
 BBBBBBBBBBBBBBBBIIIIIIIIIIIIIIIIG TOODOOOO
    MAKE CAMERA POS MOVEMENT x^2 LIKE
*/
int SciElectra3D::RegisterWindows() {
    return 0;
}

BOOL SciElectra3D::Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    return S_OK;

}
int SciElectra3D::DrawDebugText() {
    return 0;
}
int SciElectra3D::DrawObjects() {
    return 0;
}

#pragma endregion
#pragma region Events

static SciElectra3D* hook = NULL;
LRESULT SciElectra3D::ElectraListener(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (hook != NULL) {
        MSG message;
        message.hwnd = hWnd;
        message.message = msg;
        message.wParam = wParam;
        message.lParam = lParam;
        hook->ProcessMsgEvent(message);
        /*switch (msg) {

        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            hook->ResizeEvent(width, height);
            break;

        }*/
    }
    else {
        if (msg == WM_CLOSE)
            PostQuitMessage(0);
        if (msg == SE2D_INITIALIZE) {
            hook = (SciElectra3D*)wParam;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
BOOL SciElectra3D::ProcessMsgEvent(MSG msg) {
    static HCURSOR currentCursor = GetCursor();
    static POINT deltaPos = mousePos;
    switch (msg.message)
    {
        case WM_LBUTTONDOWN:
            mouseLeftDown = true;
            break;
        case WM_LBUTTONUP:
            mouseLeftDown = false;
            break;
        case WM_MOUSEWHEEL:
            deltaMouseWheel = GET_WHEEL_DELTA_WPARAM(msg.wParam);
            zoom *= deltaMouseWheel > 0 ? 1.1 : 0.9;
            /*POINT mouse;
            GetCursorPos(&mouse);
            static POINT mousebefore;
            if(mousebefore.x != mouse.x || mousebefore.y != mouse.y){
                CameraPos = ScreenToWorld(mouse);
                mousebefore.x=mouse.x;
                mousebefore.y=mouse.y;
            }
            else {
                POINT ms = WorldToScreen(CameraPos);
                SetCursorPos(ms.x, ms.y);
            }*/
            WindowRectUpdate();
            break; 
        case WM_MBUTTONDOWN: mouseMiddleDown = true; break;
        case WM_MBUTTONUP:   mouseMiddleDown = false; break;

        case WM_MOUSEMOVE:
            mousePos.x = (signed short)(msg.lParam);
            mousePos.y = (signed short)(msg.lParam >> 16);
            deltaMousePos.x = mousePos.x - deltaPos.x;
            deltaMousePos.y = mousePos.y - deltaPos.y;
            deltaPos = mousePos;
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            switch (msg.wParam)
            {
            case VK_CONTROL:
					controlDown = true;
					break;
            case VK_SHIFT:
                    shiftDown = true;
                    break;
            default:
                break;
            }
            break;
        case WM_KEYUP:
			switch (msg.wParam)
			{
			case VK_CONTROL:
				controlDown = false;
				break;
			case VK_SHIFT:
				shiftDown = false;
				break;
			default:
				break;
			}
            break;
        case WM_SYSKEYDOWN:
            // stifle this keyboard message if imgui wants to capture
            break;
        
        case WM_SYSKEYUP:
            break;
        case WM_CHAR:
            // stifle this keyboard message if imgui wants to capture
            break;
        case WM_SIZE:
            UINT width = LOWORD(msg.lParam);
            UINT height = HIWORD(msg.lParam);
            hook->ResizeEvent(width, height);

            break;
    }
    return 0;
}
void SciElectra3D::ResizeEvent(UINT width, UINT height) {
    glViewport(0, 0, width, height);
}
static Entity* interactedEntity;
static size_t lastChangeId = -1;
void SciElectra3D::ExecuteInteractions()
{

}
#pragma endregion
#pragma region Functions
void SciElectra3D::SetDebugText(const WCHAR* text,unsigned short index) {
    debugTexts[index] = text;
}
void SciElectra3D::AddDebugText(const WCHAR* text) {
    debugTexts[debugTextIterator] = text;
    debugTextIterator++;
}
#pragma endregion


