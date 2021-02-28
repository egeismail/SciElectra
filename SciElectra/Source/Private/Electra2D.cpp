#include "../Public/Electra2D.hpp"
Electra2D::Electra2D()
{
}

Electra2D::~Electra2D()
{
}

int Electra2D::Tick()
{
	sTime = std::chrono::high_resolution_clock::now();
	/*EXECUTE RULES*/
	/*if (this->Rules & Rules::CollisionWindow)
		this->Collision();*/
	if (this->Rules & Rules::NewtonianGravity)
		this->NewtonianGravity();
	if (this->Rules & Rules::Collision)
		this->Collision();
	this->ProcessPosition();
	eTime = std::chrono::high_resolution_clock::now();
	tickTime = eTime - sTime;
	tickTimef = std::chrono::duration_cast<microseconds>(tickTime).count()/10e+5;
	tickTimef_C = resolution * timeMultiplier;
	elapsedTimeUS += tickTimef_C*10e+6;//std::chrono::duration_cast<microseconds>(tickTime).count()* timeMultiplier * 2;

	auto itr = this->entities.begin();

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
	for (auto entity = entities.begin(); entity != entities.end();++entity) {
		it++;
		if (entity->object->id == id)
			entities.erase(it);
	}
	return 0;
}
int Electra2D::ProcessPosition() {
	for (auto entity = entities.begin();entity != entities.end();++entity)
	{
		if(entity->velocity.x != NAN && entity->velocity.y != NAN){
			entity->pos += entity->velocity*tickTimef_C;
		}
	}
	return 0;

}
int Electra2D::NewtonianGravity()
{
	for (auto affecting = entities.begin(); affecting != entities.end(); ++affecting) {
		for (auto affected = entities.begin(); affected != entities.end(); ++affected) {
			if (affecting->object->id == affected->object->id) continue;
			Vector2 diff = affecting->pos - affected->pos;
			Angle toAffected = atan2f(diff.y, diff.x);
			Angle toAffecting(toAffected.pitch + PI);
			
			float radius = diff.getLength();
			float force = (affecting->mass * affected->mass *GRAVITATIONAL_CONSTANT) /
				(radius * radius);
			force *= tickTimef_C;
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
	for (auto& affecting : entities) {
		for (auto& affected: entities) {
			if (affecting.type == DrawTypes::Circle && affected.type == DrawTypes::Circle && affecting.object->id != affected.object->id) {
				float distance = affected.pos.getDistance(affecting.pos);
				float limit = ((ObjectCircle*)affected.object)->radius + ((ObjectCircle*)affecting.object)->radius;
				if (distance <= limit) {
					float m1 = affecting.mass, m2 = affected.mass,M=m1+m2;
					Vector2 v1 = affecting.velocity, v2 = affected.velocity;
					Vector2 r1 = affecting.pos, r2 = affected.pos;
					float d2 = powf(r1.getDistance(r2), 2);

					Vector2 u1 = v1-((r1 - r2)*(v1 - v2).dot(r1 - r2)*2 * m2)/(d2 *(m1+m2)),
						    u2 = v2 - ((r2 - r1)* (v2 - v1).dot(r2 - r1) * 2 * m1) / (d2 * (m1 + m2)) ;
					affecting.velocity = u1;
					affected.velocity = u2;
				}
			}
		}
	}
	return 0;
}

