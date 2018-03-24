#include "PhysicsEngine.hpp"

#include "Shapes/PlaneShape.hpp"
#include "Shapes/SphereShape.hpp"

#include "CollisionFuncs.hpp"

PhysicsEngine::PhysicsEngine()
	: gravity(0.0F, 9.82F, 0.0F) {

}

PhysicsEngine::~PhysicsEngine() {

}

IPhysicsMemoryProfile* PhysicsEngine::getPhysicsMemoryProfile() {
	return nullptr;
}

StaticObject* PhysicsEngine::createStaticObject() {

	StaticObject* obj = new StaticObject();

	staticObjects.push_back(obj);

	return obj;
}

RigidBody* PhysicsEngine::createRigidBody() {

	RigidBody* obj = new RigidBody();

	rigidBodys.push_back(obj);

	return obj;
}

void PhysicsEngine::freeStaticObject(StaticObject* objectId) {
	delete objectId;
}

void PhysicsEngine::freeRigidBody(RigidBody* objectId) {
	delete objectId;
}

void PhysicsEngine::update(float dt) {

	pUpdate(dt);
	checkCollisions();
	resolveCollisions();
	checkConstraints();
}

/**********************
*  PRIVATE FUNCTIONS  *
***********************/

void PhysicsEngine::pUpdate(float dt) {

	for (size_t i = 0; i < rigidBodys.size(); i++) {
		RigidBody* rb = rigidBodys[i];
		rb->velocity -= gravity * dt;
		rb->oldPos = rb->position;
		rb->position += rb->velocity * dt;

		printf("Pos %f\n", rb->position.y);

	}

}

void PhysicsEngine::checkCollisions() {

	for (size_t s = 0; s < staticObjects.size(); s++) {

		for (size_t r = 0; r < rigidBodys.size(); r++) {

			if (isColliding(staticObjects[s]->shape, rigidBodys[r])) {
				rigidBodys[r]->velocity *= -1;
				printf("Collision\n");
			}
		}
	}

}

void PhysicsEngine::resolveCollisions() {

}

void PhysicsEngine::checkConstraints() {

}

bool PhysicsEngine::isColliding(IPhysicsShape* s1, RigidBody* rb) {
	bool collided = false;

	PlaneShape* ps = (PlaneShape*)s1;
	SphereShape* ss = (SphereShape*)rb->shape;

	SphereShape ts = *ss;
	ts.center += rb->position;

	collided = PlaneVsSphere(*ps, ts);

	//glm::vec3 po = ps->normal * ps->distance;
	//glm::vec3 sp = ss->center + rb->position;
	//
	//float d = abs(glm::dot(sp - po, ps->normal));
	//
	//if (d < ss->radius) {
	//	collided = true;
	//}

	// might have passed through object due to high speed
	if (!collided) {

		//rb-> oldPos

	}

	return collided;
}