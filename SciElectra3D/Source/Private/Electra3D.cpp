#include "../Public/Electra3D.hpp"




 Entity::Entity(glm::vec3 pos_, glm::vec3 rotation_, glm::vec3 velocity_, float scale_, float mass_, Model model_) {
	pos = pos_;
	velocity = velocity_;
	rotation = rotation_;
	mass = mass_;
	model = model_;
	scale = scale_;
	updateModelMatrix();
}
 Entity::Entity(glm::vec3 pos_, glm::vec3 rotation_, glm::vec3 velocity_, float scale_, float mass_, Model model_, Light light_) {
	pos = pos_;
	velocity = velocity_;
	rotation = rotation_;
	mass = mass_;
	model = model_;
	light = light_;
	scale = scale_;
	usingLight = true;
	updateModelMatrix();
};


 Entity::~Entity() {

}
 void Entity::updateModelMatrix()
{
	model_m = glm::mat4(1.0f);
	model_m = glm::scale(model_m, glm::vec3(scale, scale, scale));
	rotation = glm::clamp(rotation, -3.1415926535f, 3.1415926535f);
	model_m = glm::rotate(model_m, rotation.x, glm::vec3(1.0f, 0, 0));
	model_m = glm::rotate(model_m, rotation.y, glm::vec3(0, 1.0f, 0));
	model_m = glm::rotate(model_m, rotation.z, glm::vec3(0, 0, 1.0f));
	model_m = glm::translate(model_m, pos);
}

Electra3D::Electra3D()
{
}

Electra3D::~Electra3D()
{
}

int Electra3D::Tick()
{

	sTime = high_resolution_clock::now();
	/*EXECUTE RULES*/
	/*if (this->Rules & Rules::CollisionWindow)
		this->Collision();*/
	this->ProcessPosition();
	if (this->Rules & Rules::NewtonianGravity)
		this->NewtonianGravity();
	if (this->Rules & Rules::Collision)
		this->Collision_1();
	eTime = high_resolution_clock::now();
	tickTime = eTime - sTime;
	tickTimef = duration_cast<microseconds>(tickTime).count()/10e+5;
	tickTimef_C = resolution * timeMultiplier;
	elapsedTimeUS += tickTimef_C*10e+6;//duration_cast<microseconds>(tickTime).count()* timeMultiplier * 2;

	auto itr = this->entities.begin();

	return 0;
}

int Electra3D::addEntity(Entity entity)
{
	entity.id = entityCounter;
	entities.push_back(entity);
	entityCounter++;
	return 0;
}

int Electra3D::removeEntity(size_t id)
{
	std::list<Entity>::iterator it;
	for (auto entity = entities.begin(); entity != entities.end();++entity) {
		it++;
		if (entity->id == id)
			entities.erase(it);
	}
	return 0;
}

Entity* Electra3D::getEntity(size_t id)
{
	for (auto& entity : entities) {
		if (entity.id == id)
			return &entity;
	}
	return 0;
}
int Electra3D::ProcessPosition() {
	/*for (auto entity = entities.begin();entity != entities.end();++entity)
	{
		if(entity->velocity.x != NaN && entity->velocity.y != NAN){
			entity->pos += entity->velocity*tickTimef_C;
		}
	}*/
	return 0;

}
int Electra3D::NewtonianGravity()
{
	/*for ( Entity& affecting :  entities)
	{
		for (Entity& affected: entities)
		{
			if (affecting.id == affected.id) continue;
			glm::vec3 diff = affecting.pos - affected.pos;
			glm::vec3 toAffected = affecting.pos(affected.pos);
			glm::vec3 toAffecting = affected.pos(affecting.pos);

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
	}*/
	return 0;
}


int Electra3D::Collision_1(){
	return 0;
}
