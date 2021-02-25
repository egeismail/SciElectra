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
#endif
    /*Configure ImDui Colors*/
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
			D2D1::ColorF(0xffffff,1.0f),
			&pGrid
		);
		if (SUCCEEDED(hr)) hr = pRT->CreateSolidColorBrush(
			D2D1::ColorF(0x777777,0.5f),
			&pGridBack
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
        if (SUCCEEDED(hr)) {
            windowSize = pRT->GetSize();
			WindowRectUpdate();

        }
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
    Vector2 cPos = (Pos - CameraPos)*zoom;
    POINT ScreenPos;
	ScreenPos.x = cPos.x + windowSize.width / 2;
	ScreenPos.y = -cPos.y + windowSize.height/ 2;
    return ScreenPos;
}
long SciElectra2D::WorldToScreenX(float x)
{
	return ((x - CameraPos.x) * zoom) + windowSize.width / 2;
}
long SciElectra2D::WorldToScreenY(float y)
{
    return ((CameraPos.y-y)*zoom + windowSize.height/2);
}
Vector2 SciElectra2D::ScreenToWorld(POINT Pos)
{
	Vector2 sPos(Pos.x, -Pos.y);
    sPos.x -= windowSize.width / 2;
    sPos.y += windowSize.height / 2;
    Vector2 rPos = (sPos / zoom) + CameraPos;
    return rPos;
}
float SciElectra2D::ScreenToWorldX(long x)
{
    return (x-windowSize.width/2)/zoom + CameraPos.x;
}
float SciElectra2D::ScreenToWorldY(long y)
{
	return (windowSize.height/2-y)/zoom + CameraPos.y;
}
int SciElectra2D::WindowRectUpdate()
{
	/*UPDATE WORLD RECT*/
	POINT ptS, ptE;
	ptE.x = windowSize.width;
	ptE.y = windowSize.height;
	ptS.x = 0;
	ptS.y = 0;
	Vector2 ptS_ = ScreenToWorld(ptS), ptE_ = ScreenToWorld(ptE);
	windowWorldRect = D2D1_RECT_F(D2D1::RectF(ptS_.x, ptS_.y, ptE_.x, ptE_.y));
    return 0;
}
float SeqA051109(unsigned int n) {
    return ((n % 3) * (n % 3) + 1) * pow(10, int(n / 3));
}
int SciElectra2D::ShowGrids() {
    if (!showGrids)
        return 0;
    std::stringstream debugStream;
	D2D1_RECT_F wwR = windowWorldRect;
    /*
        Y lines
    */
    float deltaX = wwR.right - wwR.left,
        deltaY = wwR.top - wwR.bottom;
    float dipIn = abs(1/zoom),
          dipInFloor = dipIn-floor(dipIn),
          dipM= dipInFloor > 0.7 ? 0.3 : (dipInFloor > 0.4 ? 0.6 : (dipInFloor > 0 ? 1 : 1));
    
    float step = 100.0f/(zoom*dipM),
		  subStep = step / 5.0f;
    //debugStream << "LogStep:" << step << " SubStep" << subStep << "\n";
	//OutputDebugStringA(debugStream.str().c_str());

    float gStartX = step * (floorf(wwR.left / step)),
          gEndX = step * floorf(wwR.right / step),
          gDeltaX = gEndX-gStartX,
          gStepsX = gDeltaX/step+1,
	      gStartY = step * (floorf(wwR.bottom / step)),
	      gEndY = step * (floorf(wwR.top / step)),
		  gDeltaY = gEndY - gStartY,
		  gStepsY = gDeltaY / step + 1;
	//debugStream << "delta,gStart,gEnd,gDelta,gSteps : " << delta << "," << gStart << "," << gEnd << "," << gDelta << "," << gSteps << "\n";

    float axis, subAxis,
          axis_s,subAxis_s;

    
    //char axisString[24];
    for (size_t i=0;i<gStepsX;i++)
    {
		axis = gStartX + step * i;
        axis_s = WorldToScreenX(axis);
        //debugStream << "Axis : " << axis << "\n";
        for (size_t j = 1; j < subStep; j++)
        {
			subAxis = axis + j * subStep;
			subAxis_s = WorldToScreenX(subAxis);
            if (subAxis_s >= windowSize.width) break;
			pRT->DrawLine(
				D2D1::Point2F(subAxis_s, 0),
				D2D1::Point2F(subAxis_s, windowSize.width),
				pGridBack
			);
        }
		pRT->DrawLine(
			D2D1::Point2F(axis_s, 0),
			D2D1::Point2F(axis_s, windowSize.width),
			pGrid
		);
        //sprintf(axisString, "%d", ((int)axis));
		//pRT->DrawTextW(axisString, strlen(axisString), textFormat, D2D1::RectF(0, 0, 0,0), pGrid);

    }

    /*
        X Lines
    */

    

	//char axisString[24];
	for (size_t i = 0; i < gStepsY+1; i++)
	{
		axis = gStartY + step * i;
		axis_s = WorldToScreenY(axis);
		//debugStream << "Axis : " << axis << "\n";
		for (size_t j = 1; j < subStep; j++)
		{
			subAxis = axis - j * subStep;
			subAxis_s = WorldToScreenY(subAxis);
			pRT->DrawLine(
				D2D1::Point2F(windowSize.width,subAxis_s),
				D2D1::Point2F(0,subAxis_s),
				pGridBack
			);
		}
		pRT->DrawLine(
			D2D1::Point2F(windowSize.width, axis_s),
			D2D1::Point2F(0, axis_s),
			pGrid
		);
		//sprintf(axisString, "%d", ((int)axis));
		//pRT->DrawTextW(axisString, strlen(axisString), textFormat, D2D1::RectF(0, 0, 0,0), pGrid);

	}
	//OutputDebugStringA(debugStream.str().c_str());

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
        /*Interactions*/
        static float animIntegrate = 0;
        ExecuteInteractions();
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
int SciElectra2D::RegisterWindows() {
    for (std::list<Entity>::iterator entity = this->electra.entities.begin(); entity != electra.entities.end(); entity++)
    {
        if (entity->UIVisible) {
            if (entity->type == DrawTypes::Circle) {
                std::ostringstream st;
                ObjectCircle* objCircle = (ObjectCircle*)entity->object;
                st << "Object - " << objCircle->id;
                POINT pos = WorldToScreen(entity->pos);
                ImDui::BeginWindow(st.str().c_str(), &entity->UIVisible, ImFloat2((float)pos.x, (float)pos.y),
                    ImFloat2(275, 135));

                st.str(std::string());
                st << "ID : " << objCircle->id;
                ImDui::Text(st.str().c_str());


                /*st.str(std::string());
                st << "Type : " << E2D_DrawMap[DrawTypes::Circle];
                ImDui::Text(st.str().c_str());*/

                st.str(std::string());
                st << "Radius : " << objCircle->radius;
                ImDui::Text(st.str().c_str());

                st.str(std::string());
                st << "Position : " << "(" << entity->pos.x << "," << entity->pos.y << ")";
                ImDui::Text(st.str().c_str());

                st.str(std::string());
                st << "Velocity : " << "(" << entity->velocity.x << "," << entity->velocity.y << ")";
                ImDui::Text(st.str().c_str());

                st.str(std::string());
                st << "Mass : " << entity->mass << " kg";
                ImDui::Text(st.str().c_str());

                ImDui::EndWindow();
            }
        }
    }
    if (ShowDebugWindow) {
        ImDui::BeginWindow("SciElectra Debug", &ShowDebugWindow, ImFloat2(20, 60), ImFloat2(300, 655));
        std::stringstream sst;
        ImDui::Text("O-Simulation");
        sst << "FPS : " << fps; 
        ImDui::Text(sst.str().c_str());
#ifdef SE2D_DEBUG
        sst.str(std::string()); // Frametime
        sst << "Frametime : " << eT*1000.0f << " ms";
        ImDui::Text(sst.str().c_str());

        sst.str(std::string()); // Renderables
		sst << "Renderables : " <<  renderingObjects;
		ImDui::Text(sst.str().c_str());


        D2D1_SIZE_F size = pRT->GetSize();
		sst.str(std::string()); // Screen Size
		sst << "Screen Size : " << "(" << size.width << "," << size.height << ")";
		ImDui::Text(sst.str().c_str());

        
		sst.str(std::string()); // Window World Rect
		sst << "World Rect: " << "(" << windowWorldRect.left << "," << windowWorldRect.top << "," << windowWorldRect.right << "," << windowWorldRect.bottom << ")";
		ImDui::Text(sst.str().c_str());


        sst.str(std::string()); //Mouse Pos 
        sst << "Mouse Pos : " << mousePos.x << "," << mousePos.y;
        ImDui::Text(sst.str().c_str());

		Vector2 worldPos = ScreenToWorld(mousePos);
        float wPx =ScreenToWorldX(mousePos.x), wPy = ScreenToWorldY(mousePos.y);
		POINT screenPos = WorldToScreen(worldPos);
        float sPx=WorldToScreenX(wPx), sPy= WorldToScreenY(wPy);

		sst.str(std::string()); //Mouse World Pos  Not Struct 
		sst << "Mouse STW_NS : " << wPx << "," << wPy;
		ImDui::Text(sst.str().c_str());
		sst.str(std::string()); //Mouse World Pos 
		sst << "Mouse STW    : " << worldPos.x << "," << worldPos.y;
		ImDui::Text(sst.str().c_str());

		sst.str(std::string()); //Mouse WTS Pos 
		sst << "Mouse WTS    : " << screenPos.x << "," << screenPos.y;
		ImDui::Text(sst.str().c_str());
		sst.str(std::string()); //Mouse WTS Pos Not Struct 
		sst << "Mouse WTS NS : " << sPx << "," << sPy;
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
        ImDui::SameLine();
        sst.str(std::string()); //Control Press
        sst << "CTRL: " << controlDown;
        ImDui::Text(sst.str().c_str());

		sst.str(std::string()); //Shift Press
		sst << "SHIFT: " << shiftDown;
		ImDui::Text(sst.str().c_str());

        ImDui::Text("I-Simulation");
        sst.str(std::string()); //Camera Pos
        sst << "Camera Pos: " << "(" << CameraPos.x <<","<< CameraPos.y <<")";
        ImDui::Text(sst.str().c_str());

		sst.str(std::string()); //Hover Object
		sst << "Hover Object : " << hoverObject ;
		ImDui::Text(sst.str().c_str());

		sst.str(std::string()); //Selected Object
		sst << "SelectedObject : " << SelectedObject;
        ImDui::Text(sst.str().c_str());

		sst.str(std::string()); //ZOOM TL
		sst << "DT : " << windowWorldRect.right - windowWorldRect.left;
		ImDui::Text(sst.str().c_str());
        ImDui::SameLine();
		sst.str(std::string()); //ZOOM TL
		sst << "ZR : " << 1/zoom;
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
        ImDui::BeginWindow("Graphical Settings", &ShowGraphicalSettings, ImFloat2(400, 50), ImFloat2(200, 300));
        ImDui::CheckBox("Show Grids", &showGrids);
        ImDui::EndWindow();
    }
    if (ShowObjectManager) {
		if (SelectedObject == -1 && hoverObject == -1) {
			ImDui::BeginWindow("Object Manager", &ShowObjectManager, ImFloat2(400, 50), ImFloat2(275, 135));
            ImDui::Text("Please select object!");
            ImDui::EndWindow();

		}else{
			ImDui::BeginWindow("Object Manager", &ShowObjectManager, ImFloat2(400, 50), ImFloat2(275, 600));
            for (std::list<Entity>::iterator entity = this->electra.entities.begin(); entity != electra.entities.end(); entity++)
            {
                if (SelectedObject == -1 && hoverObject == entity->object->id) {
                    if (entity->type == DrawTypes::Circle) {
                        std::ostringstream st;
                        ObjectCircle* objCircle = (ObjectCircle*)entity->object;

                        st.str(std::string());
                        st << "ID : " << objCircle->id;
                        ImDui::Text(st.str().c_str());


                        /*st.str(std::string());
                        st << "Type : " << E2D_DrawMap[DrawTypes::Circle];
                        ImDui::Text(st.str().c_str());*/

                        st.str(std::string());
                        st << "Radius : " << objCircle->radius;
                        ImDui::Text(st.str().c_str());

                        st.str(std::string());
                        st << "Position : " << "(" << entity->pos.x << "," << entity->pos.y << ")";
                        ImDui::Text(st.str().c_str());

                        st.str(std::string());
                        st << "Velocity : " << "(" << entity->velocity.x << "," << entity->velocity.y << ")";
                        ImDui::Text(st.str().c_str());

                        st.str(std::string());
                        st << "Mass : " << entity->mass << " kg";
                        ImDui::Text(st.str().c_str());
                    }
		    		break;

                }
                if (entity->object->id == SelectedObject) {
                    if (entity->type == DrawTypes::Circle) {
                        std::ostringstream st;
                        ObjectCircle* objCircle = (ObjectCircle*)entity->object;

                        st.str(std::string());
                        st << "ID : " << objCircle->id;
                        ImDui::Text(st.str().c_str());


                        /*st.str(std::string());
                        st << "Type : " << E2D_DrawMap[DrawTypes::Circle];
                        ImDui::Text(st.str().c_str());*/

                        st.str(std::string());
                        st << "Radius : " << objCircle->radius;
                        ImDui::Text(st.str().c_str());

                        st.str(std::string());
                        st << "Position : " << "(" << entity->pos.x << "," << entity->pos.y << ")";
                        ImDui::Text(st.str().c_str());

                        st.str(std::string());
                        st << "Velocity : " << "(" << entity->velocity.x << "," << entity->velocity.y << ")";
                        ImDui::Text(st.str().c_str());

                        st.str(std::string());
                        st << "Mass : " << entity->mass << " kg";
                        ImDui::Text(st.str().c_str());
                    }
		    		break;
                }
            }
			ImDui::EndWindow();
        }
        
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
        windowSize = pRT->GetSize();
        ImDui::NewFrame();
        RegisterWindows();
        pRT->BeginDraw();
		/*Grids*/
        pRT->SetTransform(D2D1::Matrix3x2F::Identity());
        pRT->Clear(D2D1::ColorF(0x0));
		ShowGrids();
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
    pRT->DrawTextW(textBuffer.c_str(), length, textFormat, D2D1::RectF(0, 0, windowSize.width, windowSize.height), pDebugText);
    //pRT->DrawRectangle(D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height), pObject);
    return 0;

}
int SciElectra2D::DrawObjects() {
    size_t rendering = 0;
    for (Entity entity : electra.entities)
    {
        if (entity.type == DrawTypes::Circle && entity.isRenderable(windowWorldRect)) {
            rendering++;
            ObjectCircle* obj;
            obj = (ObjectCircle*)entity.object;
            Vector2 repos = entity.pos - CameraPos;
            D2D1_POINT_2F rPos = D2D1::Point2F(
                (repos.x * zoom + windowSize.width / 2),
                (windowSize.height/2-repos.y * zoom));
            D2D1_ELLIPSE Circle = D2D1::Ellipse(rPos, obj->radius * zoom, obj->radius*zoom);
            pRT->FillEllipse(&Circle, pObject);
        }
    }
    if (rendering != renderingObjects) {
        renderingObjects = rendering;
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
            zoom *= deltaMouseWheel > 0 ? 1.1 : 0.9;
            WindowRectUpdate();
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
void SciElectra2D::ResizeEvent(UINT width, UINT height) {
    if (pRT) {
        pRT->Resize(D2D1::SizeU(width, height));
        windowSize = D2D1::SizeF(width, height);
        Vector2 windowWorldSize(width*zoom,height*zoom);
		WindowRectUpdate();

    }
}
static Entity* interactedEntity;
static size_t lastChangeId = -1;
void SciElectra2D::ExecuteInteractions()
{

    if (shiftDown && mouseLeftDown) {
        Vector2 mouseWPos = ScreenToWorld(mousePos);
        for (std::list<Entity>::iterator i = this->electra.entities.begin();i != electra.entities.end();i++)
        {
            
            if (i->isInteractedPoint(mouseWPos) && lastChangeId != i->object->id) {
                /*Interacted object found and selecting*/
                i->UIVisible = !i->UIVisible;
                /*
                interactedEntity = &entity;
                interactedEntity->UIVisible= !interactedEntity->UIVisible;*/
                break;
            }
        }
	}
	
	/*Object Hovering*/
	if (!mouseLeftDown) {
		Vector2 mouseWPos = ScreenToWorld(mousePos);
		bool isSelected = false;
		for (std::list<Entity>::iterator i = this->electra.entities.begin(); i != electra.entities.end(); i++)
		{

			if (i->isInteractedPoint(mouseWPos)) {
				isSelected = true;
				/*Interacted object found and selecting*/
				hoverObject = i->object->id;
				break;
			}
			if (!isSelected) { hoverObject = -1; }
		}
	}
    if (mouseLeftDown && ShowObjectManager) {
        Vector2 mouseWPos = ScreenToWorld(mousePos);
        for (std::list<Entity>::iterator i = this->electra.entities.begin(); i != electra.entities.end(); i++)
        {

            if (i->isInteractedPoint(mouseWPos) && SelectedObject != i->object->id) {
                /*Interacted object found and selecting*/
                SelectedObject = i->object->id;
                break;
            }
        }
	}else {
		lastChangeId = -1;
	}
	/*Camera Movement*/

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
            WindowRectUpdate();
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


