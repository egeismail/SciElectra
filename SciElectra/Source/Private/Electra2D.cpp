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
	this->ProcessPosition();
	std::list<Entity>::iterator itr = this->entities.begin();
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
	for (std::list<Entity>::iterator entity = entities.begin(); entity != entities.end();++entity) {
		it++;
		if (entity->object->id == id)
			entities.erase(it);
	}
	return 0;
}
int Electra2D::ProcessPosition() {
	for (std::list<Entity>::iterator entity = entities.begin();entity != entities.end();++entity)
	{
		if(entity->velocity.x != NAN && entity->velocity.y != NAN){
			entity->pos += entity->velocity;
		}
	}
	return 0;

}
int Electra2D::NewtonianGravity()
{
	for (std::list<Entity>::iterator affecting = entities.begin(); affecting != entities.end(); ++affecting) {
		for (std::list<Entity>::iterator affected = entities.begin(); affected != entities.end(); ++affected) {
			if (affecting->object->id == affected->object->id) continue;
			Vector2 diff = affecting->pos - affected->pos;
			Angle toAffected = atan2f(diff.y, diff.x);
			Angle toAffecting(toAffected.pitch + PI);
			
			float radius = diff.getLength();
			float force = (affecting->mass * affected->mass *GRAVITATIONAL_CONSTANT) /
				(radius * radius);
			float acc = force / affected->mass;
			affected->velocity += Vector2(
				acc * cosf(toAffected.pitch),
				acc * sinf(toAffected.pitch)
			);
			
			if (this->Rules & Rules::MultiEffect) {
				acc = force / affecting->mass;
				affecting->velocity += Vector2(
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

