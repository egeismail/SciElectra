#pragma once
class Camera
{
public:
	Camera();
	Camera(glm::vec3 pos_, glm::vec3 rotation, float fov, int w, int h, float n, float f);
	~Camera();
	void updateCamera();
	void prepareProjection(Shader& shader);
	void setSize(float w, float h);
	void getSize(float& w, float& h);
	void setFOV(float fov);
	float getFOV();
	void setNear(float near_);
	float getNear();
	void setFar(float far_);
	float getFar();
	void processInputs(GLFWwindow* window, float deltaTime);
	void processMouseInput(GLFWwindow* window, double xpos, double ypos);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	const float cameraSpeed = 10;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	bool firstMouse = true;
	float yaw = 0;
	float pitch = 0.0f;
	float lastX = width / 2.0;
	float lastY = height / 2.0;
	float sensitivity = 0.1f;

private:
	float FOV = glm::radians(60.0f);
	float Near = 0.1f;
	float Far = 100.0f;

	float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	int width;
	int height;
};

inline Camera::Camera()
{
}

inline Camera::Camera(glm::vec3 pos_, glm::vec3 rotation, float fov, int w, int h, float n, float f)
{
	pos = pos_;
	rotation = rotation;
	FOV = glm::radians(fov);
	width = w;
	height = h;
	Near = n;
	Far = f;
	updateCamera();
}

inline Camera::~Camera()
{
}

inline void Camera::updateCamera()
{
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
	view = glm::lookAt(pos, pos + cameraFront, cameraUp);
	projection = glm::perspective(FOV, aspectRatio, Near, Far);
	//std::cout << "Pos" << " : (" << pos.x << "," << pos.y << "," << pos.z << ") " << "Pitch:" << pitch<<" Yaw:" << yaw <<  "        \r";

	//std::cout << "FOV : " << glm::degrees(FOV) << " AspectRatio : " << aspectRatio << "Near : " << near << " Far : " << far << std::endl;

}

inline void Camera::prepareProjection(Shader& shader)
{
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
}

inline void Camera::setSize(float w, float h)
{
	width = w;
	height = h;
	aspectRatio = (float)w / (float)h;
}

inline void Camera::getSize(float& w, float& h)
{
	w = width;
	h = height;
}

inline void Camera::setFOV(float fov)
{
	FOV = fov;
}

inline float Camera::getFOV()
{
	return FOV;
}

inline void Camera::setNear(float near_)
{
	Near = near_;
}

inline float Camera::getNear()
{
	return Near;
}

inline void Camera::setFar(float far_)
{
	Far = far_;
}

inline float Camera::getFar()
{
	return Far;
}


inline void Camera::processInputs(GLFWwindow* window, float deltaTime)
{

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pos += cameraSpeed * deltaTime * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos -= cameraSpeed * deltaTime * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		pos += cameraSpeed * deltaTime * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		pos -= cameraSpeed * deltaTime * cameraUp;
}

inline void Camera::processMouseInput(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

}

