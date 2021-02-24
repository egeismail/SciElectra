#pragma once
#include <d2d1.h>
#include <list>
#include "elecmath.hpp"
#define ENTITY_LIMIT 1024
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
	Vector2 pos;
	Vector2 velocity;
	float mass = 1;
	Object *object;
	Entity(Vector2 pos_,Vector2 velocity_,float mass_,Object *obj, DrawType type_) {
		pos = pos_;
		velocity = velocity_;
		mass = mass_;
		object = obj;
		type = type_;
	}
	bool isRenderable(D2D1_SIZE_F size,Vector2 camera) {
		if (type == DrawTypes::Circle) {
			float radius = ((ObjectCircle*)object)->radius;
			return (radius+size.width/2)>abs(pos.x-camera.x)&&(radius+size.height/2)>abs(pos.y-camera.y);
		}
		return true;
	}

};
class Electra2D
{
public:
	Electra2D();
	~Electra2D();
	size_t Rules = 0b110;
	int Tick();
	std::list<Entity> entities;
	/*Interaction*/
	int addEntity(Entity entity);
	int removeEntity(size_t id);
	int ProcessPosition();
	/*RULES*/
	int NewtonianGravity();
	int Collision();
	//int CollisionWindow();
private:
	size_t entityCounter;
};

