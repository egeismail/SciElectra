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

int Electra2D::Collision_1()
{
	for (auto& affecting : entities) {
		for (auto& affected: entities) {
			if (affecting.type == DrawTypes::Circle && affected.type == DrawTypes::Circle && affecting.object->id != affected.object->id) {
				float distance = affected.pos.getDistance(affecting.pos);
				float limit = ((ObjectCircle*)affected.object)->radius + ((ObjectCircle*)affecting.object)->radius;
				if (distance < limit) {
					float m1 = affecting.mass, m2 = affected.mass,M=m1+m2;
					Vector2 v1 = affecting.velocity, v2 = affected.velocity;
					Vector2 r1 = affecting.pos, r2 = affected.pos;
					float d = powf((r1 - r2).getLength(),2);
					float dP1 = (v1 - v2).dot(r1 - r2),
						  dP2 = (v2 - v1).dot(r2 - r1);
					affecting.velocity = v1 - (r1 - r2)*(dP1 * 2 * m2) / (d * M);
					affected.velocity = v2  - (r2 - r1)*(dP2 * 2 * m1) / (d * M);
					//correction pos
					float corrLength = (distance - limit);
					if (corrLength >= 0)
						continue;
					// THERE IS ONE CORRECTION ERROR FROM NON CONSTANT VARIABLE
					corrLength = abs(corrLength);
					float l1 = corrLength * m1 / M,
					      l2 = corrLength * m2 / M;
					Angle ang = affecting.pos.getAngleTo(affected.pos),
						  ang2 = Angle(-ang.pitch);
					Vector2 V21c = Vector2(l1*cosf(ang2.pitch),l1*sinf(ang2.pitch)),
							V22c = Vector2(l2*cosf(ang.pitch), l2 * sinf(ang.pitch));
					Vector2 CorrectedPosAffecting = affected.pos -V21c,
						    CorrectedPosAffected  = affecting.pos -V22c;
					affecting.pos = CorrectedPosAffecting;
					affecting.pos = CorrectedPosAffected;
					float bdistance = distance;
					distance = affected.pos.getDistance(affecting.pos);

					/*dPrint("V1", affecting.velocity);
					dPrint("V2", affected.velocity);
					dPrint("C1", V21c);
					dPrint("C2", V22c);
					dPrint("Distance", distance);
					dPrint("DistanceBefore", bdistance);
					dPrint("Limit", limit);
					dPrint("L1L2", l1 + l2);
					dPrint("L1", V21c.getLength());
					dPrint("L2", V22c.getLength());
					dPrint("L1L2D", corrLength);
					dPrint("OKDL", (int)(distance < limit));*/

				}
			}
		}
	}
	return 0;
}
int Electra2D::Collision_2()
{
	for (auto& affecting : entities) {
		for (auto& affected : entities) {
			if (affecting.type == DrawTypes::Circle && affected.type == DrawTypes::Circle && affecting.object->id != affected.object->id) {
				float distance = affected.pos.getDistance(affecting.pos);
				float limit = ((ObjectCircle*)affected.object)->radius + ((ObjectCircle*)affecting.object)->radius;
				if (distance < limit) {
					float m1 = affecting.mass, m2 = affected.mass, M = m1 + m2;
					Vector2 v1 = affecting.velocity, v2 = affected.velocity;
					Vector2 r1 = affecting.pos, r2 = affected.pos;
					float d = powf((r1 - r2).getLength(), 2);
					float dP1 = (v1 - v2).dot(r1 - r2),
						dP2 = (v2 - v1).dot(r2 - r1);
					affecting.velocity = v1 - (r1 - r2) * (dP1 * 2 * m2) / (d * M);
					affected.velocity = v2 - (r2 - r1) * (dP2 * 2 * m1) / (d * M);
					//correction pos
					float corrLength = (distance - limit);
					if (corrLength >= 0)
						continue;
					// THERE IS ONE CORRECTION ERROR FROM NON CONSTANT VARIABLE
					corrLength = abs(corrLength);
					float l1 = corrLength * m1 / M,
						l2 = corrLength * m2 / M;
					Angle ang = affecting.pos.getAngleTo(affected.pos),
						ang2 = Angle(-ang.pitch);
					Vector2 V21c = Vector2(l1 * cosf(ang.pitch), l1 * sinf(ang.pitch)),
						V22c = Vector2(l2 * cosf(ang2.pitch), l2 * sinf(ang2.pitch));
					Vector2 CorrectedPosAffecting = affected.pos - V21c ,
						CorrectedPosAffected = affecting.pos - V22c;
					affecting.pos = CorrectedPosAffecting;
					affecting.pos = CorrectedPosAffected;
					float bdistance = distance;
					distance = affected.pos.getDistance(affecting.pos);

					/*dPrint("V1", affecting.velocity);
					dPrint("V2", affected.velocity);
					dPrint("C1", V21c);
					dPrint("C2", V22c);
					dPrint("Distance", distance);
					dPrint("DistanceBefore", bdistance);
					dPrint("Limit", limit);
					dPrint("L1L2", l1 + l2);
					dPrint("L1", V21c.getLength());
					dPrint("L2", V22c.getLength());
					dPrint("L1L2D", corrLength);
					dPrint("OKDL", (int)(distance < limit));*/

				}
			}
		}
	}
	return 0;
}

