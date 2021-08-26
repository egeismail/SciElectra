#pragma once
#include <glm/glm.hpp>
#include <list>
#include <chrono>
#include "Model.hpp"
using namespace std::chrono;

#define ENTITY_LIMIT 1024


typedef unsigned char DrawType;
enum Rules {
	MultiEffect = 0x1, // When calculating velocity in some rules, calculates affecting and affected entities in same tick
					   // I do not decided which one better calculates but until i will close
	NewtonianGravity=0x2,
	Collision = 0x4,
	CollisionWindow = 0x80000000,
};

struct Entity {
	size_t id;
	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 rotation;
	glm::mat4 model_m = glm::mat4(1.0f);
	float mass = 1;
	Model model;
	bool UIVisible = false;
	Entity(glm::vec3 pos_,glm::vec3 rotation_, glm::vec3 velocity_,float mass_,Model model_) {
		pos = pos_;
		velocity = velocity_;
		rotation = rotation_;
		mass = mass_;
		model = model_;
		updateModelMatrix();

	}
	void updateModelMatrix();
	

};
class Electra3D
{
public:
	Electra3D();
	~Electra3D();
	size_t Rules = 6;
	unsigned long long elapsedTimeUS;
	float resolution = 0.001f;
	float tickTimef = 0.001f;
	float timeMultiplier = 1;
	float elasticity = 1;
	float tickTimef_C = tickTimef* timeMultiplier;
	int Tick();
	std::list<Entity> entities;
	/*Timing*/
	
	std::chrono::steady_clock::time_point sTime, eTime;
	std::chrono::steady_clock::duration  tickTime;
	/*Interaction*/
	int addEntity(Entity entity);
	int removeEntity(size_t id);
	int ProcessPosition();
	/*RULES*/
	int NewtonianGravity();
	int Collision_1();
	//int CollisionWindow();
private:
	size_t entityCounter;
};

