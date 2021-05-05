#include "../Public/Electra3D.hpp"
Electra3D::Electra3D()
{
}

Electra3D::~Electra3D()
{
}

int Electra3D::Tick()
{
	sTime = std::chrono::high_resolution_clock::now();
	/*EXECUTE RULES*/
	/*if (this->Rules & Rules::CollisionWindow)
		this->Collision();*/
	this->ProcessPosition();
	if (this->Rules & Rules::NewtonianGravity)
		this->NewtonianGravity();
	if (this->Rules & Rules::Collision)
		this->Collision_1();
	eTime = std::chrono::high_resolution_clock::now();
	tickTime = eTime - sTime;
	tickTimef = std::chrono::duration_cast<microseconds>(tickTime).count()/10e+5;
	tickTimef_C = resolution * timeMultiplier;
	elapsedTimeUS += tickTimef_C*10e+6;//std::chrono::duration_cast<microseconds>(tickTime).count()* timeMultiplier * 2;

	auto itr = this->entities.begin();

	return 0;
}

int Electra3D::addEntity(Entity entity)
{
	entity.object.id = entityCounter;
	entities.push_back(entity);
	entityCounter++;
	return 0;
}

int Electra3D::removeEntity(size_t id)
{
	std::list<Entity>::iterator it;
	for (auto entity = entities.begin(); entity != entities.end();++entity) {
		it++;
		if (entity->object.id == id)
			entities.erase(it);
	}
	return 0;
}
int Electra3D::ProcessPosition() {
	for (auto entity = entities.begin();entity != entities.end();++entity)
	{
		if(entity->velocity.x != NAN && entity->velocity.y != NAN){
			entity->pos += entity->velocity*tickTimef_C;
		}
	}
	return 0;

}
int Electra3D::NewtonianGravity()
{
	for ( Entity& affecting :  entities)
	{
		for (Entity& affected: entities)
		{
			if (affecting.object.id == affected.object.id) continue;
			Vector3 diff = affecting.pos - affected.pos;
			Angle toAffected = affecting.pos.getAngleTo(affected.pos);
			Angle toAffecting = affected.pos.getAngleTo(affecting.pos);

			float radius = affecting.pos.getDistance(affecting.pos);
			float force = (affecting.mass * affected.mass * GRAVITATIONAL_CONSTANT) /
				(radius * radius);
			force *= tickTimef_C;
			float acc = force / affected.mass;
			affected.velocity += Vector3(
				acc * cosf(toAffected.pitch),
				acc * sinf(toAffected.pitch)
			);

			if (this->Rules & Rules::MultiEffect) {
				acc = force / affecting.mass;
				affecting.velocity += Vector3(
					acc * cosf(toAffected.pitch),
					acc * sinf(toAffected.pitch)
				);
			}
		}
	}
	return 0;
}


int Electra3D::Collision_1()
{
	
	return 0;
}


