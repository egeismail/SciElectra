#include "../Public/SciElectra2D.h"
#define DFC(x) (forDebug & x) == 0 
#define DF(x)   forDebug |= x
//Window* root
#pragma region Initializations & Destroy
SciElectra2D::SciElectra2D()
{

}

SciElectra2D::~SciElectra2D()
{
}

void SciElectra2D::InitializeWindow(Window* root)
{
    root->Register((WNDPROC)this->ElectraListener);
    BOOL returnCode = root->createWindow();

    if (returnCode != 1) {
        MessageBox(0, L"Failed to create window!.", 0, 0);
    }
    returnCode = root->displayWindow();
    if (!returnCode != 1) {
        MessageBox(0, L"Failed to display window!.", 0, 0);
    }
    this->root = root;
    this->d2d1factory = NULL;
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &this->d2d1factory
    );
    hWnd = root->hWnd;
    OutputDebugStringA("Initializing Factory\n");
    GetClientRect(hWnd, &this->windowRectangle);
    props = D2D1::RenderTargetProperties();
    HARDWARE_ACC;
    hr = this->d2d1factory->CreateHwndRenderTarget(
        props,
        D2D1::HwndRenderTargetProperties(
            hWnd,
            D2D1::SizeU(
                this->windowRectangle.right - this->windowRectangle.left,
                this->windowRectangle.bottom - this->windowRectangle.top
            )
        ),
        &pRT
    );
    if (SUCCEEDED(hr)) {
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(0xfd7000),
            &pOrangeBrush
        );
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(0x0),
            &pBlack
        );
    }
    PostMessage(hWnd, SE2D_INITIALIZE, (WPARAM)this, NULL);
}
#pragma endregion
#pragma region Loops

BOOL SciElectra2D::Start()
{
    while (!this->isDone) {
        /*Events*/
        MSG msg;
        BOOL hResult = this->root->listenMessage(&msg);
        //dat = "RECT: " + std::to_string(windowRectangle.top) + ":" + std::to_string(windowRectangle.right) + ":" + std::to_string(windowRectangle.bottom) + ":" + std::to_string(windowRectangle.left) + "\n";
        ProcessMsgEvent(msg);
        if (hResult == 0)
            return 0;/*WM_QUIT*/
        if (hResult == -1)
            return -1;/*ERROR*/
        /*Physics*/
        this->electra.Tick();
        /*Drawing*/
        this->Render();
    }
    return 0;
}

BOOL SciElectra2D::Render() {
    HRESULT hr = S_OK;
    GetClientRect(hWnd, &this->windowRectangle);
    if (pRT != NULL) {
        if (DFC(0b10)) {
            OutputDebugStringA("Success\n");
            DF(0b10);
        }
        pRT->BeginDraw();
        pRT->Clear(D2D1::ColorF(0x0));
        for (Entity entity : electra.entities)
        {
            if (entity.type == DrawTypes::Circle) {
                char ODB[200];
                ObjectCircle* obj;
                obj = (ObjectCircle*)entity.object;
                //obj = *(ObjectCircle**)entity.object;
                //OutputDebugStringA((LPSTR)sprintf_s(ODB,"id:%d x:%f y:%f\n", obj->id,entity.pos.x, entity.pos.y));
                D2D1_ELLIPSE Circle = D2D1::Ellipse(D2D1::Point2F(entity.pos.x, entity.pos.y), obj->radius, obj->radius);
                pRT->FillEllipse(&Circle, pOrangeBrush);
            }
        }
        hr = pRT->EndDraw();
        return SUCCEEDED(hr);
    }
    else {
        if (DFC(0b1)) {
            std::string dat;
            dat += "IsWindow: ";
            BOOL res = IsWindow(hWnd);
            dat += (LPSTR)std::to_string(res).c_str();
            dat += "\n";
            OutputDebugStringA("Failed\n");
            DF(0b1);
        }
    }

    return SUCCEEDED(hr);

}

static SciElectra2D* hook = NULL;;
LRESULT SciElectra2D::ElectraListener(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (hook != NULL) {
        switch (msg) {

        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            hook->ResizeEvent();
            break;
            /*default:
                std::string dat;
                if (msg == 160 || msg == 0 || msg == 512) goto nothing;
                dat = "msg: " + std::to_string(msg)+"\n";
                OutputDebugStringA((LPSTR)dat.c_str());
                nothing:
                break;*/
        }
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
    switch (msg.message) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    }
    return 0;
}
HRESULT SciElectra2D::ResizeEvent() {
    HRESULT hr;

    GetClientRect(hWnd, &this->windowRectangle);
    //std::string dat;
    //dat = "Resized to "+std::to_string(this->windowRectangle.top) + ":" + std::to_string(this->windowRectangle.right) + ":" + std::to_string(this->windowRectangle.bottom) + ":" + std::to_string(this->windowRectangle.left) + "\n";
    //OutputDebugStringA((LPSTR)dat.c_str());
    hr = this->d2d1factory->CreateHwndRenderTarget(
        props,
        D2D1::HwndRenderTargetProperties(
            hWnd,
            D2D1::SizeU(
                this->windowRectangle.right - this->windowRectangle.left,
                this->windowRectangle.bottom - this->windowRectangle.top
            )
        ),
        &pRT
    );
    if (SUCCEEDED(hr)) {
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(0xfd7000),
            &pOrangeBrush
        );
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(0x0),
            &pBlack
        );
        return S_OK;
    }
    return S_FALSE;

}
#pragma endregion

#pragma region Tests
void SciElectra2D::test()
{
    RECT windowRect;
    GetWindowRect(hWnd, &windowRect);
    static ObjectCircle circle1, circle2;
    circle1.radius = 5;
    circle2.radius = 10;
    Entity entity1, entity2;
    entity1.type = DrawTypes::Circle;
    entity2.type = DrawTypes::Circle;
    entity1.pos = XMFLOAT2(0.6f * windowRect.right / 2.f, windowRect.bottom / 2.f - circle1.radius / 2);
    entity2.pos = XMFLOAT2(1.4f * windowRect.right / 2.f, windowRect.bottom / 2.f - circle2.radius / 2);
    entity1.object = &circle1;
    entity2.object = &circle2;
    this->electra.addEntity(entity1);
    this->electra.addEntity(entity2);
}
#pragma endregion
