#pragma once
#include "Model.hpp"

enum LightType
{
	Normal,
	Diffuse,
	Specular
};

class Light
{
public:
	Light();
	Light(LightType lightType_, glm::vec3 lightColor_);
	~Light();
	void uploadLights(Shader& shader);
	LightType lightType;
	glm::vec3 lightColor;
private:

};

inline Light::Light()
{
}

inline Light::Light(LightType lightType_,glm::vec3 lightColor_)
{
	lightType = lightType_;
	lightColor = lightColor_;

}

inline Light::~Light()
{
}

inline void Light::uploadLights(Shader& shader)
{
	shader.setVec3("lightColor", lightColor);
}
