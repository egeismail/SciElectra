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
	for (const Entity entity: entities) {
		it++;
		if (entity.object->id == id)
			entities.erase(it);
	}
	return 0;
}
int Electra2D::NewtonianGravity()
{
	for (Entity affecting : entities) {
		for (Entity affected : entities) {
			Vector2 diff = affecting.pos - affected.pos;
			Angle toAffecting = atan2f(diff.y, diff.x);
			Angle toAffected(toAffecting.pitch + PI);
			
			float radius = diff.getLength();
			float force = (affecting.mass * affected.mass *GRAVITATIONAL_CONSTANT) /
				(radius * radius);
			float acc = force / affected.mass;
			affected.pos += Vector2(
				acc * cosf(toAffected.pitch),
				acc * sinf(toAffected.pitch)
			);
			
			if (this->Rules & Rules::MultiEffect) {
				acc = force / affecting.mass;
				affecting.pos += Vector2(
					acc * cosf(toAffected.pitch),
					acc * sinf(toAffected.pitch)
				);
			}

		}
	}
	return 0;
}

int Electra2D::Collision()
{
	return 0;
}

