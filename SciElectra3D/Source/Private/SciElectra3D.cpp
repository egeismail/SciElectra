#include "../Public/SciElectra3D.hpp"
#define DFC(x) (forDebug & x) == 0 
#define DF(x)   forDebug |= x

//Window* root
#pragma region Callbacks

static SciElectra3D* hselectra;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (!hselectra) {
        void* pp = glfwGetWindowUserPointer(window);
        hselectra = static_cast<SciElectra3D*>(glfwGetWindowUserPointer(window));
        return;
    }
    
    glViewport(0, 0, width, height);
    hselectra->WIDTH = width;
    hselectra->HEIGHT = height;
    hselectra->camera.setSize(width, height);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!hselectra){
        void* pp = glfwGetWindowUserPointer(window);
        hselectra = static_cast<SciElectra3D*>(glfwGetWindowUserPointer(window));
        return;
    }
    hselectra->processMouseInput(window, xpos, ypos);
    

}

void error_callback(int error, const char* msg) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}

#pragma endregion


#pragma region Initializations & Destroy
SciElectra3D::SciElectra3D()
{

}

SciElectra3D::~SciElectra3D()
{
}

HRESULT SciElectra3D::InitializeWindow()
{
    BOOL lr = true;
    glfwSetErrorCallback(error_callback);
    if (glfwInit() == GLFW_FALSE) {
        cout << "Failed to glfwInit()" << std::endl;
        return S_FALSE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "SciElectra", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    hWnd = glfwGetWin32Window(window);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    glfwSetWindowUserPointer(window, this);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 18.0f * highDPIscaleFactor, NULL, NULL);
    glfwMaximizeWindow(window);
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    framebuffer_size_callback(window, w, h);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);



    return lr ? S_OK : S_FALSE;
}

#pragma endregion
#pragma region Loops



float cubicBezier(float y1, float y2, float normalized) {
    return 3 * normalized * pow((1 - normalized), 2) * y1 + 3 * pow(normalized, 2) * (1 - normalized) * y2 + pow(normalized, 3);
}

void SciElectra3D::DrawGuis(float deltaTime)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //if (sL_hasLoader) {
    //    ImGui::Begin("Progress Indicators");
    //        const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
    //        const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);
    //        const ImVec2  vvsz = ImVec2(400, 6);
    //        ImGui::BufferingBar("##buffer_bar", sL_percent, vvsz,bg, col);
    //    ImGui::End();
    //}

    if (ShowDebugWindow) {
        ImGui::Begin("SciElectra Debug", &ShowDebugWindow);
        std::stringstream sst;
        ImGui::Text("O-Simulation");
        sst << "FPS : " << fps;
        ImGui::Text(sst.str().c_str());
#ifdef SE2D_DEBUG
        sst.str(std::string()); // Frametime
        sst << "Frametime : " << eT * 1000.0f << " ms; :" << duration_cast<microseconds>(frameTime).count() << "us";
        ImGui::Text(sst.str().c_str());

        sst.str(std::string()); // Frametime Sim
        sst << "Frametime Sim : " << duration_cast<microseconds>(electra.tickTime).count() << "ms";
        ImGui::Text(sst.str().c_str());

        sst.str(std::string()); // Elapsed Time
        sst << "Elapsed Time : " << simElapsedTime << " s";
        ImGui::Text(sst.str().c_str());

        sst.str(std::string()); // Elapsed Time
        sst << "Elapsed Time Sim : " << (electra.elapsedTimeUS) / 10e+6 << " s";
        ImGui::Text(sst.str().c_str());

        sst.str(std::string()); // Elapsed Time
        sst << "Tick per render: " << (SyncTick) << " t/r";
        ImGui::Text(sst.str().c_str());

        sst.str(std::string()); // Render And Simulation fTime
        sst << "R|S : " << eT << "," << electra.tickTimef;
        ImGui::Text(sst.str().c_str());

        sst.str(std::string()); // Renderables
        sst << "Renderables : " << renderingObjects;
        ImGui::Text(sst.str().c_str());
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        sst.str(std::string()); // Screen Size
        sst << "Screen Size : " << "(" << w << "," << h << ")";
        ImGui::Text(sst.str().c_str());


        //sst.str(std::string()); // Window World Rect
        //sst << "World Rect: " << "(" << windowWorldRect.left << "," << windowWorldRect.top << "," << windowWorldRect.right << "," << windowWorldRect.bottom << ")";
        //ImGui::Text(sst.str().c_str());

        //sst.str(std::string()); //Mouse Pos 
        //sst << "Mouse Pos : " << mousePos.x << "," << mousePos.y;
        //ImDui::Text(sst.str().c_str());

        /*ctor2 worldPos = ScreenToWorld(mousePos);
        float wPx = ScreenToWorldX(mousePos.x), wPy = ScreenToWorldY(mousePos.y);
        POINT screenPos = WorldToScreen(worldPos);
        float sPx = WorldToScreenX(wPx), sPy = WorldToScreenY(wPy);*/

        //sst.str(std::string()); //Mouse World Pos  Not Struct 
        //sst << "Mouse STW_NS : " << wPx << "," << wPy;
        //ImDui::Text(sst.str().c_str());
        //sst.str(std::string()); //Mouse World Pos 
        //sst << "Mouse STW    : " << worldPos.x << "," << worldPos.y;
        //ImDui::Text(sst.str().c_str());

        //sst.str(std::string()); //Mouse WTS Pos 
        //sst << "Mouse WTS    : " << screenPos.x << "," << screenPos.y;
        //ImDui::Text(sst.str().c_str());
        //sst.str(std::string()); //Mouse WTS Pos Not Struct 
        //sst << "Mouse WTS NS : " << sPx << "," << sPy;
        //ImDui::Text(sst.str().c_str());



        //sst.str(std::string()); //Delta Pos Mouse
        //sst << "Mouse Delta Pos : " << deltaMousePos.x << "," << deltaMousePos.y;
        //ImDui::Text(sst.str().c_str());

        //sst.str(std::string()); //Delta Pos Mouse
        //sst << "Mouse DPS : " << dbs_distance.x << "," << dbs_distance.y;
        //ImDui::Text(sst.str().c_str());

        //sst.str(std::string()); //Mouse Left Click
        //sst << "ML: " << mouseLeftDown;
        //ImDui::SameLine(); ImDui::Text(sst.str().c_str());

        //sst.str(std::string()); //MWheel
        //sst << "MWheel: " << deltaMouseWheel;
        //ImDui::Text(sst.str().c_str());
        //ImDui::SameLine();
        //sst.str(std::string()); //Control Press
        //sst << "CTRL: " << controlDown;
        //ImDui::Text(sst.str().c_str());

        //sst.str(std::string()); //Shift Press
        //sst << "SHIFT: " << shiftDown;
        //ImDui::Text(sst.str().c_str());
        if(mainShader.compiled){
            if (ImGui::Button("Reload Shader")){
                mainShader.reload();
            }
        }
        else {
            if (!mainShader.vertexIsReady && !mainShader.fragmentIsReady) {
                ImGui::Text("Loading shader...");
            }
            if (mainShader.errorOnFragment || mainShader.errorOnVertex) {
                if (ImGui::Button("Compile Shader")) {
                    mainShader.reload();
                }
            }
            if (mainShader.errorOnFragment) {
                ImGui::Text("Error on compiling fragment shader.");
            }
            if (mainShader.errorOnVertex) {
                ImGui::Text("Error on compiling vertex shader.");
            }
            
        }
        ImGui::Text("I-Simulation");
        sst.str(std::string()); //Camera Pos
        sst << "Camera Pos: " << "(" << camera.pos.x << "," << camera.pos.y << "," << camera.pos.z << ")";
        ImGui::Text(sst.str().c_str());

        //sst.str(std::string()); //Hover Object
        //sst << "Hover Object : " << hoverObject;
        //ImDui::Text(sst.str().c_str());

        //sst.str(std::string()); //Selected Object
        //sst << "SelectedObject : " << SelectedObject;
        //ImDui::Text(sst.str().c_str());

        //sst.str(std::string()); //ZOOM TL
        //sst << "DT : " << windowWorldRect.right - windowWorldRect.left;
        //ImDui::Text(sst.str().c_str());
        //ImDui::SameLine();
        //sst.str(std::string()); //ZOOM TL
        //sst << "ZR : " << 1 / zoom;
        //ImDui::Text(sst.str().c_str());
        //sst.str(std::string()); //ZOOM TL
        //sst << "INTP : " << intap;
        //ImDui::Text(sst.str().c_str());

#endif
        ImGui::PushItemWidth(50);
        if (ImGui::Button("Reset")) {
            zoom = 1;
        }
        ImGui::SameLine();
        ImGui::PushItemWidth(120);
        ImGui::SliderFloat("Zoom", &zoom, 0.001, 10, "%.6f");
        if (ImGui::Button("Simulation Settings"))
            ShowSimulationSettings = !ShowSimulationSettings;
        if (ImGui::Button("Graphical  Settings"))
            ShowGraphicalSettings = !ShowGraphicalSettings;
        if (ImGui::Button("UI Style Manager"))
            ShowStyleEditor = !ShowStyleEditor;
        if (ImGui::Button("Object  Manager"))
            ShowObjectManager = !ShowObjectManager;

        ImGui::End();
    }
    static bool MultiEffect = this->electra.Rules & Rules::MultiEffect ? true : false,
        NewtonianGravity = this->electra.Rules & Rules::NewtonianGravity ? true : false,
        Collision = this->electra.Rules & Rules::Collision ? true : false;
    if (ShowSimulationSettings) {
        if (ImGui::Begin("Simulation Settings", &ShowSimulationSettings)) {
            ImGui::PushItemWidth(150);
            if (ImGui::Button("Reset")) {
                electra.timeMultiplier = 1.0f;
            }
            ImGui::SameLine();
            ImGui::PushItemWidth(200);

            ImGui::SliderFloat("Time Multiplier", &electra.timeMultiplier, 0.001, 2, "%.3f");
            //ImGui::SliderFloat("Mouse Drag Strength", &mouseDragStrength, 0.1, 10, "%.3f");
            ImGui::Checkbox("Multi Effect", &MultiEffect); ImGui::SameLine();
            ImGui::Checkbox("Newtonian Gravity", &NewtonianGravity);
            ImGui::Checkbox("Collision", &Collision); ImGui::SameLine();
            if (MultiEffect != this->electra.Rules & Rules::MultiEffect > 0)
                this->electra.Rules = MultiEffect ? this->electra.Rules | Rules::MultiEffect : this->electra.Rules & ~Rules::MultiEffect;
            if (NewtonianGravity != this->electra.Rules & Rules::NewtonianGravity > 0)
                this->electra.Rules = NewtonianGravity ? this->electra.Rules | Rules::NewtonianGravity : this->electra.Rules & ~Rules::NewtonianGravity;
            if (Collision != this->electra.Rules & Rules::Collision > 0)
                this->electra.Rules = Collision ? this->electra.Rules | Rules::Collision : this->electra.Rules & ~Rules::Collision;
            ImGui::End();
        };
        
    }
    if (ShowGraphicalSettings) {
        ImGui::Begin("Graphical Settings", &ShowGraphicalSettings);
        ImGui::Checkbox("Show Grids", &showGrids);
        ImGui::Checkbox("Show Vectors", &showVectors);
        ImGui::Checkbox("Show Angles", &showAngles);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

#define glfwGetKeyOnce(WINDOW, KEY)	(glfwGetKey(WINDOW, KEY) ?	(keyOnce[KEY] ? false : (keyOnce[KEY] = true)) : (keyOnce[KEY] = false))
void SciElectra3D::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        camera.pos = glm::vec3(0, 0, 0);
    if (glfwGetKeyOnce(window, GLFW_KEY_LEFT_ALT))
    {
        glfwMouseEnabled = !glfwMouseEnabled;
        glfwSetInputMode(window, GLFW_CURSOR, glfwMouseEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
    camera.processInputs(window, eT);

}

void SciElectra3D::processMouseInput(GLFWwindow* window, double xpos, double ypos)
{
    if (!glfwMouseEnabled)
        camera.processMouseInput(window, xpos, ypos);
}
BOOL SciElectra3D::Start()
{
    simulationStartTime = steady_clock::now();
    steady_clock::time_point lastRender = steady_clock::now();
    if (onLoad) {
        onLoad(this);
    }
    while (!this->isDone && !glfwWindowShouldClose(window)) {
		sTime = steady_clock::now();
        processInput(window);
        camera.updateCamera();
        this->Render();
        DrawGuis(eT ? eT : 0.001f);

        glfwSwapBuffers(window);
        glfwPollEvents();

        /*Interactions*/
        eTime = steady_clock::now();
        frameTime = eTime - sTime;
        elapsedTime = eTime-simulationStartTime;
        eT = (duration_cast<microseconds>(frameTime).count() / 10e+5f);
        fps = 1.f / (duration_cast<microseconds>(frameTime).count() / 10e+5f);
        simElapsedTime = (duration_cast<microseconds>(elapsedTime).count() / 10e+5f);
        if (onTick) {
            onTick(this);
        }
		/*Physics*/
        /*SyncTick = (unsigned int)(eT / electra.tickTimef);
		for (int tickIteration = 0; tickIteration < SyncTick; tickIteration++)
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
BOOL SciElectra3D::Render() {
    glClearColor(31 / 255.0f, 40 / 255.0f, 45 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (mainShader.compiled) {
        mainShader.use();
        for (Entity& ent : electra.entities)
        {
            for (Entity& entL : electra.entities)
            {
                if (!entL.usingLight) continue;

                mainShader.setVec3("lightPos", entL.pos);
                entL.light.uploadLights(mainShader);
            }
            camera.prepareProjection(mainShader);
            ent.updateModelMatrix();
            mainShader.setVec3("viewPos", camera.pos);
            mainShader.setMat4("model", ent.model_m);
            ent.model.Draw(mainShader);
        }
    }
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


