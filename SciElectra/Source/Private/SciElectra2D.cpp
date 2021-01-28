#include "../Public/SciElectra2D.h"

SciElectra2D::SciElectra2D(Window* root)
{
    this->root = root;
    this->d2d1factory = NULL;
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &this->d2d1factory
    );
    hWnd = root->hWnd;

}



SciElectra2D::~SciElectra2D()
{
}

void SciElectra2D::test()
{
    RECT windowRect;
    GetWindowRect(hWnd, &windowRect);
    static ObjectCircle circle1, circle2;
    circle1.radius = 5;
    circle2.radius = 10;
    Entity entity1, entity2;
    entity1.pos = XMFLOAT2(0.6f * windowRect.left / 2.f, windowRect.top / 2.f - circle1.radius / 2);
    entity2.pos = XMFLOAT2(1.4f * windowRect.left / 2.f, windowRect.top / 2.f-circle2.radius / 2);
    entity1.object = &circle1;
    entity2.object = &circle2;
    this->electra.addEntity(entity1);
    this->electra.addEntity(entity2);
}
BOOL SciElectra2D::ProcessMsgEvent(MSG msg) {
    return 1;
}
BOOL SciElectra2D::Render() {
    RECT rc;
    GetClientRect(hWnd, &rc);
    ID2D1HwndRenderTarget* pRT = NULL;
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
    //HARDWARE_ACC
    HRESULT hr = this->d2d1factory->CreateHwndRenderTarget(
            props,
            D2D1::HwndRenderTargetProperties(
                hWnd,
                D2D1::SizeU(
                    rc.right - rc.left,
                    rc.bottom - rc.top
                )
            ),
            &pRT
        );

    ID2D1SolidColorBrush* pOrangeBrush = NULL;
    ID2D1SolidColorBrush* pBlack = NULL;
    if (SUCCEEDED(hr)) {
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(0xfd7000),
            &pOrangeBrush
        );
        pRT->CreateSolidColorBrush(
            D2D1::ColorF(0x0),
            &pBlack
        );
        pRT->BeginDraw();
        pRT->Clear(D2D1::ColorF(0x0));
        for (Entity entity : electra.entities)
        {
            if (entity.type & DrawTypes::Circle) {
                char ODB[200];

                ObjectCircle *obj;
                obj = (ObjectCircle*)entity.object;
                OutputDebugStringA((LPSTR)sprintf_s(ODB,"id:%d x:%f y:%f\n", obj->id,entity.pos.x, entity.pos.y));
                D2D1_ELLIPSE Circle=D2D1::Ellipse(D2D1::Point2F(entity.pos.x, entity.pos.y), obj->radius, obj->radius);
                pRT->FillEllipse(&Circle, pOrangeBrush);
            }
        }
        hr = pRT->EndDraw();
        return SUCCEEDED(hr);
    }
    else {
        OutputDebugStringA("Failed");
    }
    
    return SUCCEEDED(hr);

}
BOOL SciElectra2D::Start()
{
    while(!this->isDone){
        /*Events*/
        MSG msg;
        BOOL hResult = this->root->listenMessage(&msg);
        if (hResult == 0) 
            return 0;/*WM_QUIT*/
        if (hResult == -1)
            return -1;/*ERROR*/
        ProcessMsgEvent(msg);
        /*Physics*/
        this->electra.Tick();
        /*Drawing*/
        this->Render();
    }
    return 0;
}
