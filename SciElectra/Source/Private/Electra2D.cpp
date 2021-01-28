#include "../Public/Electra2D.h"
Electra2D::Electra2D()
{
}

Electra2D::~Electra2D()
{
}

int Electra2D::Tick()
{
	/*EXECUTE RULES*/
	/*if (this->Rules & Rules::CollisionWindow)
		this->Collision();*/
	if (this->Rules & Rules::NewtonianGravity)
		this->NewtonianGravity();
	if (this->Rules & Rules::Collision)
		this->Collision();
	return 0;
}

int Electra2D::addEntity(Entity entity)
{
	entity.object->id = entityCounter;
	entities.push_back(entity);
	entityCounter++;
	return 0;
}

int Electra2D::removeEntity(size_t id)
{
	std::list<Entity>::iterator it;
	for (Entity entity: entities) {
		it++;
		if (entity.object->id == id)
			entities.erase(it);
	}
	return 0;
}
int Electra2D::NewtonianGravity()
{
	//for (Entity effector : entities) {
	//	for (Entity effecting : entities) {
	//		//XMVECTOR diff = effecting - effector;
	//	}
	//}
	return 0;
}

int Electra2D::Collision()
{
	return 0;
}

