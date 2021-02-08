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
    }

    return hr;
}
void SciElectra2D::DiscardDeviceResources()
{
    SafeRelease(&pRT);
    SafeRelease(&pDebugText);
    SafeRelease(&pObject);
    SafeRelease(&pBlack);
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
        /*Drawing*/
        this->Render();
        eTime = steady_clock::now();
        frameTime = eTime - sTime;
        elapsedTime = eTime-simulationStartTime;
#if SE2D_DEBUG
        WCHAR FPSText[DEBUG_TEXT_LENGTH], ElapsedTime[DEBUG_TEXT_LENGTH], E1[DEBUG_TEXT_LENGTH], E2[DEBUG_TEXT_LENGTH];
        double fps = 1.f / (duration_cast<microseconds>(frameTime).count() / 10e+6f);
        double simElapsedTime = (duration_cast<microseconds>(elapsedTime).count() / 10e+5f);
        if(duration_cast<milliseconds>(eTime - lastRender).count() > 16){
            swprintf_s(FPSText, DEBUG_TEXT_LENGTH,    L"FPS          : %f", fps);
            swprintf_s(ElapsedTime, DEBUG_TEXT_LENGTH, L"Elapsed Time : %f", simElapsedTime);
            std::list<Entity>::iterator itr = this->electra.entities.begin();
            swprintf_s(E1, DEBUG_TEXT_LENGTH, L"Entity1 : x:%f,y:%f,m:%f,r:%d", itr->pos.x, itr->pos.y, itr->mass,itr->isRenderable(rtSize));
            itr++;
            swprintf_s(E2, DEBUG_TEXT_LENGTH, L"Entity2 : x:%f,y:%f,m:%f,r:%d", itr->pos.x, itr->pos.y, itr->mass,itr->isRenderable(rtSize));
            this->SetDebugText(FPSText, 1);
            this->SetDebugText(ElapsedTime, 2);
            this->SetDebugText(E1, 3);
            this->SetDebugText(E2, 4);
            lastRender = eTime;
        }
#endif
    }
    return 0;
}

BOOL SciElectra2D::Render() {
    HRESULT hr = S_OK;
    hr =CreateDeviceResources();
    if (SUCCEEDED(hr))
    {
        rtSize = pRT->GetSize();
        pRT->BeginDraw();
        pRT->SetTransform(D2D1::Matrix3x2F::Identity());
        pRT->Clear(D2D1::ColorF(0x0));
        this->DrawObjects();
        this->DrawDebugText();
        hr = pRT->EndDraw();
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
        if (entity.type == DrawTypes::Circle && entity.isRenderable(rtSize)) {
            ObjectCircle* obj;
            obj = (ObjectCircle*)entity.object;
            Vector2 repos = entity.pos - CameraPos;
            D2D1_POINT_2F rPos = D2D1::Point2F(repos.x + rtSize.width / 2, rtSize.height / 2-repos.y);
            D2D1_ELLIPSE Circle = D2D1::Ellipse(rPos, obj->radius, obj->radius);
            pRT->FillEllipse(&Circle, pObject);
        }
    }
    return 0;
}
static SciElectra2D* hook = NULL;;
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
    static bool controlPress=false;
    switch (msg.message) {
        case WM_CLOSE:
            DiscardDeviceResources();
            PostQuitMessage(0);
            break;
        //case WM_KEYDOWN:
        //    // syskey commands need to be handled to track ALT key (VK_MENU) and F10
        //case WM_SYSKEYDOWN:
        //    // stifle this keyboard message if imgui wants to capture
        //    if (imio.WantCaptureKeyboard)
        //    {
        //        break;
        //    }
        //    if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
        //    {
        //        kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        //    }
        //    break;
        //case WM_KEYUP:
        //case WM_SYSKEYUP:
        //    // stifle this keyboard message if imgui wants to capture
        //    if (imio.WantCaptureKeyboard)
        //    {
        //        break;
        //    }
        //    kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        //    break;
        //case WM_CHAR:
        //    // stifle this keyboard message if imgui wants to capture
        //    if (imio.WantCaptureKeyboard)
        //    {
        //        break;
        //    }
        //    kbd.OnChar(static_cast<unsigned char>(wParam));
        //    break;
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


