#pragma once
#include <d2d1.h>
#include <DirectXMath.h>
#include <list>
using namespace DirectX;
#define ENTITY_LIMIT 1024
#define GRAVITATIONAL_CONSTANT 6.67430e-11f
enum DrawTypes
{
	Circle=0x0
};
enum Rules {
	NewtonianGravity=0x1,
	Collision = 0x2,
	CollisionWindow = 0x80000000,
};
struct Object {
	size_t id;
};
struct ObjectCircle : Object{
	float radius;
};
struct Entity {
	unsigned char type;
	XMFLOAT2 pos;
	float angle;
	Object *object;
};
class Electra2D
{
public:
	Electra2D();
	~Electra2D();
	size_t Rules = 0b0;
	int Tick();
	std::list<Entity> entities;
	/*Interaction*/
	int addEntity(Entity entity);
	int removeEntity(size_t id);
	/*RULES*/
	int NewtonianGravity();
	int Collision();
	//int CollisionWindow();
private:
	size_t entityCounter;
};

