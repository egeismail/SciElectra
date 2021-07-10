#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 rotation, float fov, float width, float height, float near_p, float far_p);
	Camera();
	~Camera();
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPos;
	glm::vec3 cameraRotation;
	void updateViewMatrix();

private:

};
inline Camera::Camera() {
	projectionMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
}
inline Camera::Camera(glm::vec3 pos,glm::vec3 rotation,float fov, float width, float height, float near_p, float far_p)
{
	cameraPos = pos;
	cameraRotation = rotation;
	projectionMatrix = glm::perspective(glm::radians(fov), width / height, near_p, far_p);
	updateViewMatrix();
	
}

inline Camera::~Camera()
{
}
inline void Camera::updateViewMatrix()
{
	viewMatrix = glm::mat4(1.0f);
	cameraRotation = glm::clamp(cameraRotation, -3.1415926535f, 3.1415926535f);
	glm::rotate(viewMatrix, cameraRotation.x, glm::vec3(1.0f, 0, 0));
	glm::rotate(viewMatrix, cameraRotation.y, glm::vec3(0, 1.0f, 0));
	glm::rotate(viewMatrix, cameraRotation.z, glm::vec3(0, 0, 1.0f));
	glm::translate(viewMatrix, cameraPos);
}