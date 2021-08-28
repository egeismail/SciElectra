#include "../Public/SciElectra3D.hpp"
#define DFC(x) (forDebug & x) == 0 
#define DF(x)   forDebug |= x

//Window* root
#pragma region Callbacks

static SciElectra3D* hselectra;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (hselectra == NULL) {
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
    if (hselectra == NULL){
        void* pp = glfwGetWindowUserPointer(window);
        hselectra = static_cast<SciElectra3D*>(glfwGetWindowUserPointer(window));
        return;
    }
    hselectra->camera.processMouseInput(window, xpos, ypos);
    

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
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    hWnd = glfwGetWin32Window(window);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    glfwSetWindowUserPointer(window, this);


    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);



    return lr ? S_OK : S_FALSE;
}

#pragma endregion
#pragma region Loops



float cubicBezier(float y1, float y2, float normalized) {
    return 3 * normalized * pow((1 - normalized), 2) * y1 + 3 * pow(normalized, 2) * (1 - normalized) * y2 + pow(normalized, 3);
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
void SciElectra3D::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        camera.pos = glm::vec3(0, 0, 0);
    camera.processInputs(window, eT);

}
BOOL SciElectra3D::Start()
{
    simulationStartTime = steady_clock::now();
    steady_clock::time_point lastRender = steady_clock::now();
    while (!this->isDone && !glfwWindowShouldClose(window)) {
		sTime = steady_clock::now();
        processInput(window);
        camera.updateCamera();

        this->Render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        /*Interactions*/
        eTime = steady_clock::now();
        frameTime = eTime - sTime;
        elapsedTime = eTime-simulationStartTime;
        eT = (duration_cast<microseconds>(frameTime).count() / 10e+5f);
        fps = 1.f / (duration_cast<microseconds>(frameTime).count() / 10e+5f);
        simElapsedTime = (duration_cast<microseconds>(elapsedTime).count() / 10e+5f);
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
int SciElectra3D::RegisterWindows() {
    return 0;
}

BOOL SciElectra3D::Render() {
    glClearColor(31 / 255.0f, 40 / 255.0f, 45/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (mainShader.compiled) {
        mainShader.use();
        for (Entity &ent : electra.entities)
        {
        
            camera.prepareProjection(mainShader);
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


