#pragma once
#include <d2d1.h>
#include <list>
#include <chrono>
#include "elecmath.hpp"

#define ENTITY_LIMIT 1024
using namespace std::chrono;

enum DrawTypes
{
	Circle=0x1
};
typedef unsigned char DrawType;
enum Rules {
	MultiEffect = 0x1, // When calculating velocity in some rules, calculates affecting and affected entities in same tick
					   // I do not decided which one better calculates but until i will close
	NewtonianGravity=0x2,
	Collision = 0x4,
	CollisionWindow = 0x80000000,
};
struct Object {
	size_t id;
};
struct ObjectCircle : Object{
	float radius;
};
struct Entity {
	DrawType type = NULL;
	Vector3 pos;
	Vector3 velocity;
	float mass = 1;
	Object object;
	bool UIVisible = false;
	Entity(Vector3 pos_,Vector3 velocity_,float mass_,Object obj) {
		pos = pos_;
		velocity = velocity_;
		mass = mass_;
		object = obj;
	}
	Entity() {
	}

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

