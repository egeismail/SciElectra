#include "../Public/SciElectra2D.hpp"
#define DFC(x) (forDebug & x) == 0 
#define DF(x)   forDebug |= x
template<class Interface>
inline void SafeRelease(
    Interface** ppInterfaceToRelease
)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

//Window* root
#pragma region Initializations & Destroy
SciElectra2D::SciElectra2D()
{

}

SciElectra2D::~SciElectra2D()
{
    ImDui::Shutdown();
    DiscardDeviceResources();
}

HRESULT SciElectra2D::InitializeWindow(Window* root)
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
    /*Create Factory*/
    hWnd = root->hWnd;
    lr = CreateDeviceIndependentResources() && lr;
    PostMessage(hWnd, SE2D_INITIALIZE, (WPARAM)this, NULL);
    /*Debug Text*/
#if SE2D_DEBUG
    this->AddDebugText(TEXT("SciElectra Alpha"));
    this->AddDebugText(TEXT("FPS          : 0"));
    this->AddDebugText(TEXT("Elapsed Time : 0"));
    this->AddDebugText(TEXT("Entity1      : x:0,y:0,m:0"));
    this->AddDebugText(TEXT("Entity2      : x:0,y:0,m:0"));
#endif
    /*Configure ImDu� Colors*/
    return lr ? S_OK : S_FALSE;
}
#pragma endregion
#pragma region Loops

HRESULT SciElectra2D::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_MULTI_THREADED,
        &this->d2d1factory
    );
    if (SUCCEEDED(hr)) hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(writeFactory),
        reinterpret_cast<IUnknown**>(&writeFactory)
    );
    if(SUCCEEDED(hr))
        hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&imagingFactory));
    return hr;
}
HRESULT SciElectra2D::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!pRT)
    {
        RECT rc;
        GetClientRect(hWnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
        );
        props = D2D1::RenderTargetProperties();
        HARDWARE_ACC;
        // Create a Direct2D render target.
        hr = d2d1factory->CreateHwndRenderTarget(
            props,
            D2D1::HwndRenderTargetProperties(hWnd, size),
            &pRT
        );
        if (SUCCEEDED(hr)) hr = pRT->CreateSolidColorBrush(
            D2D1::ColorF(0xfd7000),
            &pObject
        );
        if (SUCCEEDED(hr)) hr = pRT->CreateSolidColorBrush(
            D2D1::ColorF(0x0),
            &pBlack
        );
        if (SUCCEEDED(hr)) hr = pRT->CreateSolidColorBrush(
            D2D1::ColorF(0x00ff00),
            &pDebugText
        );
		if (SUCCEEDED(hr)) hr = pRT->CreateSolidColorBrush(
			D2D1::ColorF(0xaaaaaa),
			&pGrid
		);


        if (SUCCEEDED(hr)) hr = writeFactory->CreateTextFormat(
            fDebugText,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            fDebugSize,
            L"", //locale
            &this->textFormat
        );
        if (SUCCEEDED(hr)) ImDui::InitResources(d2d1factory, writeFactory, imagingFactory, pRT);
    }

    return hr;
}
void SciElectra2D::DiscardDeviceResources()
{
    SafeRelease(&pRT);
    SafeRelease(&pDebugText);
    SafeRelease(&pObject);
	SafeRelease(&pBlack);
	SafeRelease(&pGrid);
}

float cubicBezier(float y1, float y2, float normalized) {
    return 3 * normalized * pow((1 - normalized), 2) * y1 + 3 * pow(normalized, 2) * (1 - normalized) * y2 + pow(normalized, 3);
}
POINT SciElectra2D::WorldToScreen(Vector2 Pos)
{
    D2D1_SIZE_F size = pRT->GetSize();
    Vector2 cPos = (Pos - CameraPos)/zoom;
    POINT ScreenPos;
	ScreenPos.x = cPos.x - size.width / 2;
	ScreenPos.y = cPos.y + size.height/ 2;
    return ScreenPos;
}
Vector2 SciElectra2D::ScreenToWorld(POINT Pos)
{
	D2D1_SIZE_F size = pRT->GetSize();
	Vector2 sPos(Pos.x, Pos.y);
    sPos.x -= size.width / 2;
    sPos.y += size.height / 2;
    return (sPos/zoom) + CameraPos;
}
int SciElectra2D::ShowGrids() {
    if (!showGrids)
        return 0;
    D2D1_SIZE_F size = pRT->GetSize();
    /*X lines*/
    return 0;
}
BOOL SciElectra2D::Start()
{
    simulationStartTime = steady_clock::now();
    steady_clock::time_point lastRender = steady_clock::now();
    while (!this->isDone) {
        /*Events*/
        MSG msg;
        BOOL hResult = this->root->listenMessage(&msg);
        sTime = steady_clock::now();
        if (hResult == 0)
            return 0;/*WM_QUIT*/
        if (hResult == -1)
            return -1;/*ERROR*/
        /*Physics*/
        this->electra.Tick();
        /*CameraPos*/
        static float animIntegrate = 0;
        if (controlDown && mouseLeftDown) {
            if (!dbs_initiated) {
                dbs_initiated = true;
                dbs_startPointCamera = CameraPos;
                dbs_startPoint = mousePos;
            }
            else {
                /*if (deltaMousePos.x > 1 || deltaMousePos.y > 1) {
                    animIntegrate = 0;
                }*/
                dbs_endPoint = mousePos;
                dbs_distance.x = dbs_endPoint.x - dbs_startPoint.x;
                dbs_distance.y = dbs_endPoint.y - dbs_startPoint.y;
                //animIntegrate +=  eT;
                //float tY  = cubicBezier(.35,.9, animIntegrate);
                CameraPos = (dbs_startPointCamera - Vector(dbs_distance.x / zoom, -dbs_distance.y / zoom));
            }
        }
        else {
            dbs_initiated = false;
        }
        /*Drawing*/
        this->Render();
        eTime = steady_clock::now();
        frameTime = eTime - sTime;
        elapsedTime = eTime-simulationStartTime;
#if SE2D_DEBUG
        WCHAR FPSText[DEBUG_TEXT_LENGTH], ElapsedTime[DEBUG_TEXT_LENGTH], E1[DEBUG_TEXT_LENGTH], E2[DEBUG_TEXT_LENGTH];
        eT = (duration_cast<microseconds>(frameTime).count() / 10e+6f);
        fps = 1.f / (duration_cast<microseconds>(frameTime).count() / 10e+6f);
        double simElapsedTime = (duration_cast<microseconds>(elapsedTime).count() / 10e+5f);
        if(duration_cast<milliseconds>(eTime - lastRender).count() > 16){
            swprintf_s(FPSText, DEBUG_TEXT_LENGTH,    L"FPS          : %f", fps);
            swprintf_s(ElapsedTime, DEBUG_TEXT_LENGTH, L"Elapsed Time : %f", simElapsedTime);
            //std::list<Entity>::iterator itr = this->electra.entities.begin();
            //swprintf_s(E1, DEBUG_TEXT_LENGTH, L"Entity1 : x:%f,y:%f,m:%f,r:%d", itr->pos.x, itr->pos.y, itr->mass,itr->isRenderable(rtSize));
            //itr++;
            //swprintf_s(E2, DEBUG_TEXT_LENGTH, L"Entity2 : x:%f,y:%f,m:%f,r:%d", itr->pos.x, itr->pos.y, itr->mass,itr->isRenderable(rtSize));
            this->SetDebugText(FPSText, 1);
            this->SetDebugText(ElapsedTime, 2);
            //this->SetDebugText(E1, 3);
            //this->SetDebugText(E2, 4);
            lastRender = eTime;
        }
#endif
    }
    return 0;
}
/*
 BBBBBBBBBBBBBBBBIIIIIIIIIIIIIIIIG TOODOOOO
    MAKE CAMERA POS MOVEMENT x^2 LIKE
*/
static bool ShowDebugWindow = true,
            ShowSimulationSettings = false,
            ShowGraphicalSettings = false,
            ShowObjectManager = false,
            ShowStyleEditor = false;
int SciElectra2D::RegisterWindows() {
    if (ShowDebugWindow) {
        ImDui::BeginWindow("SciElectra Debug", &ShowDebugWindow, ImFloat2(20, 100), ImFloat2(300, 500));
        std::stringstream sst;
        ImDui::Text("O-Simulation");
        sst << "FPS : " << fps; 
        ImDui::Text(sst.str().c_str());
#ifdef SE2D_DEBUG
        sst.str(std::string()); // Frametime
        sst << "Frametime : " << eT*1000.0f << " ms";
        ImDui::Text(sst.str().c_str());

        D2D1_SIZE_F size = pRT->GetSize();
		sst.str(std::string()); // Screen Size
		sst << "Screen Size : " << "(" << size.width << "," << size.height << ")";
		ImDui::Text(sst.str().c_str());

        sst.str(std::string()); //Mouse Pos 
        sst << "Mouse Pos : " << mousePos.x << "," << mousePos.y;
        ImDui::Text(sst.str().c_str());

		sst.str(std::string()); //Mouse World Pos 
        Vector2 worldPos = ScreenToWorld(mousePos);
		sst << "Mouse World Pos : " << worldPos.x << "," << worldPos.y;
		ImDui::Text(sst.str().c_str());

        sst.str(std::string()); //Delta Pos Mouse
        sst << "Mouse Delta Pos : " << deltaMousePos.x << "," << deltaMousePos.y;
        ImDui::Text(sst.str().c_str());

        sst.str(std::string()); //Delta Pos Mouse
        sst << "Mouse DPS : " << dbs_distance.x << "," << dbs_distance.y;
        ImDui::Text(sst.str().c_str());

        sst.str(std::string()); //Mouse Left Click
        sst << "ML: " << mouseLeftDown;
        ImDui::SameLine(); ImDui::Text(sst.str().c_str());

        sst.str(std::string()); //MWheel
        sst << "MWheel: " << deltaMouseWheel;
        ImDui::Text(sst.str().c_str());

        sst.str(std::string()); //Control Press
        sst << "CTRL: " << controlDown;
        ImDui::Text(sst.str().c_str());

        ImDui::Text("I-Simulation");
        sst.str(std::string()); //Control Press
        sst << "Camera Pos: " << "(" << CameraPos.x <<","<< CameraPos.y <<")";
        ImDui::Text(sst.str().c_str());

#endif
        ImDui::PushItemWidth(50);
        if (ImDui::Button("Reset")) {
            zoom = 1;
        }
        ImDui::SameLine();
        ImDui::PushItemWidth(120);
        ImDui::SliderFloat("Zoom", &zoom, 0.001, 10,"%.6f");
        if (ImDui::Button("Simulation Settings"))
            ShowSimulationSettings = !ShowSimulationSettings;
        if (ImDui::Button("Graphical  Settings"))
            ShowGraphicalSettings= !ShowGraphicalSettings;
        if (ImDui::Button("UI Style Manager"))
            ShowStyleEditor = !ShowStyleEditor;
        if (ImDui::Button("Object  Manager"))
            ShowObjectManager = !ShowObjectManager;
        
        ImDui::EndWindow();
    }
    static bool MultiEffect=this->electra.Rules & Rules::MultiEffect ? true : false,
                NewtonianGravity= this->electra.Rules & Rules::NewtonianGravity ? true : false,
                Collision= this->electra.Rules & Rules::Collision ? true : false,
                CollisionWindow= this->electra.Rules & Rules::CollisionWindow ? true : false;
    static float timeMultiplier = 1.0f;
    if (ShowSimulationSettings) {
        ImDui::BeginWindow("Simulation Settings", &ShowSimulationSettings, ImFloat2(400, 50), ImFloat2(400, 400));
        ImDui::PushItemWidth(150);
        if (ImDui::Button("Reset")) {
            timeMultiplier = 1.0f;
        }
        ImDui::SameLine();
        ImDui::PushItemWidth(200);
        ImDui::SliderFloat("Time Multiplier", &timeMultiplier, 0.1, 2, "%.3f");
        ImDui::CheckBox("Multi Effect", &MultiEffect); ImDui::SameLine();
        ImDui::CheckBox("Newtonian Gravity", &NewtonianGravity);
        ImDui::CheckBox("Collision", &Collision); ImDui::SameLine();
        ImDui::CheckBox("Window Collision", &CollisionWindow);
        if (MultiEffect != this->electra.Rules & Rules::MultiEffect > 0)
            this->electra.Rules  = MultiEffect ? this->electra.Rules | Rules::MultiEffect: this->electra.Rules & ~Rules::MultiEffect;
        if (NewtonianGravity != this->electra.Rules & Rules::NewtonianGravity > 0)
            this->electra.Rules = NewtonianGravity ? this->electra.Rules | Rules::NewtonianGravity : this->electra.Rules & ~Rules::NewtonianGravity;
        if (Collision != this->electra.Rules & Rules::Collision > 0)
            this->electra.Rules = Collision ? this->electra.Rules | Rules::Collision : this->electra.Rules & ~Rules::Collision;
        if (CollisionWindow != this->electra.Rules & Rules::CollisionWindow > 0)
            this->electra.Rules = CollisionWindow ? this->electra.Rules | Rules::CollisionWindow : this->electra.Rules & ~Rules::CollisionWindow;
        ImDui::EndWindow();
    }
    if (ShowGraphicalSettings) {
        ImDui::BeginWindow("Graphical Settings", &ShowGraphicalSettings, ImFloat2(400, 50), ImFloat2(400, 400));
        ImDui::EndWindow();
    }
    if (ShowObjectManager) {
        ImDui::BeginWindow("Object Manager", &ShowObjectManager, ImFloat2(400, 50), ImFloat2(400, 400));
        ImDui::EndWindow();
    }
    if (ShowStyleEditor) {
        ImDui::BeginWindow("Style Editor", &ShowStyleEditor, ImFloat2(400, 50), ImFloat2(400, 570));
        ImDui::ShowStyleEditor();
        ImDui::EndWindow();
    }
    return 0;
}

BOOL SciElectra2D::Render() {
    HRESULT hr = S_OK;
    hr =CreateDeviceResources();
    if (SUCCEEDED(hr))
    {
        rtSize = pRT->GetSize();
        ImDui::NewFrame();
        RegisterWindows();
        pRT->BeginDraw();
		/*Grids*/
		//ShowGrids();
        pRT->SetTransform(D2D1::Matrix3x2F::Identity());
        pRT->Clear(D2D1::ColorF(0x0));
        this->DrawObjects();
        this->DrawDebugText();
        ImDui::Render();
;       hr = pRT->EndDraw();
    }   
    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

    return hr;

}
int SciElectra2D::DrawDebugText() {
    size_t sizeBuffer;
    std::wstring textBuffer;
    for (size_t k=0;k<debugTextIterator;++k)
    {
        std::wstring buffer = debugTexts[k];
        if(wcslen(buffer.c_str()) != 0)
        textBuffer = textBuffer + buffer + TEXT("\n");
    }
    size_t length = wcslen(textBuffer.c_str());
    pRT->DrawTextW(textBuffer.c_str(), length, textFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), pDebugText);
    //pRT->DrawRectangle(D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height), pObject);
    return 0;

}
int SciElectra2D::DrawObjects() {
    for (Entity entity : electra.entities)
    {
        if (entity.type == DrawTypes::Circle && entity.isRenderable(rtSize, CameraPos)) {
            ObjectCircle* obj;
            obj = (ObjectCircle*)entity.object;
            Vector2 repos = entity.pos - CameraPos;
            D2D1_POINT_2F rPos = D2D1::Point2F(
                (repos.x * zoom + rtSize.width / 2),
                (rtSize.height/2-repos.y * zoom));
            D2D1_ELLIPSE Circle = D2D1::Ellipse(rPos, obj->radius * zoom, obj->radius*zoom);
            pRT->FillEllipse(&Circle, pObject);
        }
    }
    return 0;
}
static SciElectra2D* hook = NULL;
LRESULT SciElectra2D::ElectraListener(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
            hook = (SciElectra2D*)wParam;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma endregion
#pragma region Events
BOOL SciElectra2D::ProcessMsgEvent(MSG msg) {
    static HCURSOR currentCursor = GetCursor();
    static POINT deltaPos = mousePos;

    ImDui::Event& events = ImDui::GetEvents();
    switch (msg.message)
    {
        case WM_LBUTTONDOWN:
            events.MouseDown = true;
            mouseLeftDown = true;
            break;
        case WM_LBUTTONUP:
            events.MouseDown = false;
            mouseLeftDown = false;
            break;
        case WM_MOUSEWHEEL:
            events.MouseWheel = GET_WHEEL_DELTA_WPARAM(msg.wParam) > 0 ? +1 : -1;
            deltaMouseWheel = GET_WHEEL_DELTA_WPARAM(msg.wParam);
            zoom *= deltaMouseWheel > 0 ? pow(2, (1.01 + 0.03 * (deltaMouseWheel / 480.0f))) : 1/ pow(2, (1.1 + 0.1 * (deltaMouseWheel / 480.0f)));
            break; 
        case WM_MOUSEMOVE:
            events.MousePos.x = (signed short)(msg.lParam);
            events.MousePos.y = (signed short)(msg.lParam >> 16);
            mousePos.x = (signed short)(msg.lParam);
            mousePos.y = (signed short)(msg.lParam >> 16);
            deltaMousePos.x = mousePos.x - deltaPos.x;
            deltaMousePos.y = mousePos.y - deltaPos.y;
            deltaPos = mousePos;
            break;
        case WM_CLOSE:
            DiscardDeviceResources();
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            if (msg.wParam == VK_CONTROL && !controlDown) {
                controlDown = true;
            }
            break;
        case WM_KEYUP:
            if (msg.wParam == VK_CONTROL && controlDown) {
                controlDown = false;
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
void SciElectra2D::ResizeEvent(UINT width, UINT height) {
    if (pRT) {
        pRT->Resize(D2D1::SizeU(width, height));
    }
}
#pragma endregion
#pragma region Functions
void SciElectra2D::SetDebugText(const WCHAR* text,unsigned short index) {
    debugTexts[index] = text;
}
void SciElectra2D::AddDebugText(const WCHAR* text) {
    debugTexts[debugTextIterator] = text;
    debugTextIterator++;
}
#pragma endregion


