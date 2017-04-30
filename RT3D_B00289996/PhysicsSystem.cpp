#include "PhysicsSystem.h"
#include "Time.h"
#include <iostream>
#include "GameObject.h"
#include "Transform.h"
#include "RigidBody.h"
#include "DebugDraw.h"
#include "GameObjectData.h"
#include "TypeConverter.h"
#include "PlayerController.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "Collision.h"
namespace B00289996 {
	PhysicsSystem & PhysicsSystem::GetInstance() {
		static PhysicsSystem instance;
		return instance;
	}

	PhysicsSystem::PhysicsSystem() {
		// Configure the Physics World
		collisionConfig = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfig);
		broadphase = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();
		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
		world->setGravity(TypeConverter::ConvertToBulletVector3(gravity));
		debug = new DebugDraw();
		debug->setDebugMode(btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawWireframe);
		world->setDebugDrawer(debug);
		// Set Up Collision Callbacks
		gContactProcessedCallback = (ContactProcessedCallback)BeginContactCallBack;
		gContactDestroyedCallback = (ContactDestroyedCallback)EndContactCallBack;
	}

	PhysicsSystem::~PhysicsSystem() {
		delete world;
		delete dispatcher;
		delete solver;
		delete broadphase;
	}

	void PhysicsSystem::Update(const float & deltaTime) {
		world->stepSimulation(deltaTime); // step the simulation
	}

	void PhysicsSystem::Draw() {
		if(drawDebug) world->debugDrawWorld(); // Draw the Debud Drawer 
	}

	void PhysicsSystem::AddSphere(const std::shared_ptr<GameObject> & gameObject, const glm::vec3 & offset, const float & radius, const float & mass) {
		// Try to get the RigidBody from the GameObject
		std::shared_ptr<RigidBody> rb = gameObject->GetRigidBody().lock();
		// create the sphere
		btSphereShape* sphere = new btSphereShape(radius); 
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(offset.x, offset.y, offset.z));
		if(rb) { // if the object has a RigidBody
			// Add The Sphere to it
			btCompoundShape * shape = static_cast<btCompoundShape*>(rb->body->getCollisionShape());
			shape->addChildShape(t, sphere);
			btVector3 inertia(0, 0, 0);
			btScalar masses[2] = {
				rb->body->getInvMass() != 0.0f ? 1.0f / rb->body->getInvMass() : 0.0f,
				mass};
			btScalar m = masses[0] + mass;
			if(m != 0.0f) shape->calculatePrincipalAxisTransform(masses, t, inertia);// if object has mass calculate inertia
			else rb->body->setCollisionFlags(rb->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // else make object kinematic
		}
		else {
			// Add a new RigidBody then attach the shape
			btCompoundShape * shape = new btCompoundShape();
			shape->addChildShape(t, sphere);
			btVector3 inertia(0, 0, 0);
			
			if(mass != 0.0f) shape->calculateLocalInertia(mass, inertia);// if object has mass calculate inertia
			btRigidBody::btRigidBodyConstructionInfo info(mass, std::static_pointer_cast<btMotionState>(gameObject->GetTransform().lock()).get(), shape, inertia);
			btRigidBody* body = new btRigidBody(info);
			world->addRigidBody(body);
			if(mass == 0.0f) body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // if object has no mass make object kinematic
			rb = std::make_shared<RigidBody>(gameObject);
			rb->body = body;
			gameObject->rigidBody = rb;
			// set the RigidBodies user data
			shape->setUserPointer(rb->GetData());
			body->setUserPointer(rb->GetData());
		}
		rb->body->setActivationState(DISABLE_DEACTIVATION);
	}

	void PhysicsSystem::AddBox(const std::shared_ptr<GameObject> & gameObject, const glm::vec3 & offset, const glm::vec3 & dimensions, const float & mass) {
		// Try to get the RigidBody from the GameObject
		std::shared_ptr<RigidBody> rb = gameObject->GetRigidBody().lock();
		btBoxShape* box = new btBoxShape(btVector3(dimensions.x * 0.5f, dimensions.y * 0.5f, dimensions.z * 0.5f));
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(offset.x, offset.y, offset.z));
		if(rb) { // if the object has a RigidBody
			// Add The Box to it
			btCompoundShape * shape = static_cast<btCompoundShape*>(rb->body->getCollisionShape());
			shape->addChildShape(t, box);
			btVector3 inertia(0, 0, 0);
			btScalar masses[2] = {
				rb->body->getInvMass() != 0.0f ? 1.0f / rb->body->getInvMass() : 0.0f,
				mass};
			btScalar m = masses[0] + mass;
			if(m != 0.0f) shape->calculatePrincipalAxisTransform(masses, t, inertia);// if object has mass calculate inertia
			else rb->body->setCollisionFlags(rb->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // else make object kinematic
		}
		else {
			// Add a new RigidBody then attach the shape
			btCompoundShape * shape = new btCompoundShape();
			shape->addChildShape(t, box);
			btVector3 inertia(0, 0, 0);
			if(mass != 0.0f) shape->calculateLocalInertia(mass, inertia);// if object has mass calculate inertia
			btRigidBody::btRigidBodyConstructionInfo info(mass, std::static_pointer_cast<btMotionState>(gameObject->GetTransform().lock()).get(), shape, inertia);
			btRigidBody* body = new btRigidBody(info);
			world->addRigidBody(body);
			rb = std::make_shared<RigidBody>(gameObject);
			rb->body = body;
			gameObject->rigidBody = rb;
			if(mass == 0.0f) body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // if object has no mass make object kinematic
			// set the RigidBodies user data
			shape->setUserPointer(rb->GetData());
			body->setUserPointer(rb->GetData());
		}
		rb->body->setActivationState(DISABLE_DEACTIVATION);
	}

	void PhysicsSystem::AddCapsule(const std::shared_ptr<GameObject>& gameObject, const glm::vec3 & offset, const float & height, const float & radius, const float & mass) {
		// Try to get the RigidBody from the GameObject
		std::shared_ptr<RigidBody> rb = gameObject->GetRigidBody().lock();
		btCapsuleShape* capsule = new btCapsuleShape(radius, height);
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(offset.x, offset.y, offset.z));
		if(rb) { // if the object has a RigidBody
			// Add The Capsule to it
			btCompoundShape * shape = static_cast<btCompoundShape*>(rb->body->getCollisionShape());
			shape->addChildShape(t, capsule);
			btVector3 inertia(0, 0, 0);
			btScalar masses[2] = {
				rb->body->getInvMass() != 0.0f ? 1.0f / rb->body->getInvMass() : 0.0f,
				mass};
			btScalar m = masses[0] + mass;
			if(m != 0.0f) shape->calculatePrincipalAxisTransform(masses, t, inertia);// if object has mass calculate inertia
			else rb->body->setCollisionFlags(rb->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // else make object kinematic
		}
		else {
			// Add a new RigidBody then attach the shape
			btCompoundShape * shape = new btCompoundShape();
			shape->addChildShape(t, capsule);
			btVector3 inertia(0, 0, 0);
			if(mass != 0.0f) shape->calculateLocalInertia(mass, inertia);// if object has mass calculate inertia
			btRigidBody::btRigidBodyConstructionInfo info(mass, std::static_pointer_cast<btMotionState>(gameObject->GetTransform().lock()).get(), shape, inertia);
			btRigidBody* body = new btRigidBody(info);
			world->addRigidBody(body);
			rb = std::make_shared<RigidBody>(gameObject);
			rb->body = body;
			if(mass == 0.0f) body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // if object has no mass make object kinematic
			gameObject->rigidBody = rb;
			// set the RigidBodies user data
			shape->setUserPointer(rb->GetData());
			body->setUserPointer(rb->GetData());
		}
		rb->body->setActivationState(DISABLE_DEACTIVATION);
	}

	void PhysicsSystem::AddCylinder(const std::shared_ptr<GameObject>& gameObject, const glm::vec3 & offset, const glm::vec3 & dimensions, const float & mass) {
		// Try to get the RigidBody from the GameObject
		std::shared_ptr<RigidBody> rb = gameObject->GetRigidBody().lock();
		btCylinderShape* cylinder = new btCylinderShape(btVector3(dimensions.x * 0.5f, dimensions.y * 0.5f, dimensions.z * 0.5f));
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(offset.x, offset.y, offset.z));
		if(rb) { // if the object has a RigidBody
			// Add The Cylinder to it
			btCompoundShape * shape = static_cast<btCompoundShape*>(rb->body->getCollisionShape());
			shape->addChildShape(t, cylinder);
			btVector3 inertia(0, 0, 0);
			btScalar masses[2] = {
				rb->body->getInvMass() != 0.0f ? 1.0f / rb->body->getInvMass() : 0.0f,
				mass};
			btScalar m = masses[0] + mass;
			if(m != 0.0f) shape->calculatePrincipalAxisTransform(masses, t, inertia);// if object has mass calculate inertia
			else rb->body->setCollisionFlags(rb->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // else make object kinematic
		}
		else {
			// Add a new RigidBody then attach the shape
			btCompoundShape * shape = new btCompoundShape();
			shape->addChildShape(t, cylinder);
			btVector3 inertia(0, 0, 0);
			if(mass != 0.0f) shape->calculateLocalInertia(mass, inertia);// if object has mass calculate inertia
			btRigidBody::btRigidBodyConstructionInfo info(mass, std::static_pointer_cast<btMotionState>(gameObject->GetTransform().lock()).get(), shape, inertia);
			btRigidBody* body = new btRigidBody(info);
			world->addRigidBody(body);
			if(mass == 0.0f) body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); // if object has no mass make object kinematic
			rb = std::make_shared<RigidBody>(gameObject);
			rb->body = body;
			gameObject->rigidBody = rb;
			// set the RigidBodies user data
			shape->setUserPointer(rb->GetData());
			body->setUserPointer(rb->GetData());
		}
		rb->body->setActivationState(DISABLE_DEACTIVATION);
	}

	void PhysicsSystem::AddCharacterController(const std::shared_ptr<GameObject>& gameObject, const glm::vec3 & offset, const float & height, const float & radius, const float & mass) {
		AddCapsule(gameObject, offset, height, radius, mass); // adds a capsule collider
		std::shared_ptr<RigidBody> rb = gameObject->GetRigidBody().lock();
		rb->body->setAngularFactor(btVector3(0, 1, 0)); // prevent the body from rotating on the X and Z axes
		rb->SetAngularDampening(0.98f); // Set a fairly high Angular Dampening factor
		rb->SetLinearDampening(0.9f); // and a fairly high Linear Dampening factor
		rb->SetFriction(1.0f); // and a large amount of friction
	}

	RayCastHit PhysicsSystem::Raycast(const glm::vec3 & from, const glm::vec3 & to, const int & filter) {
		const btVector3 start = TypeConverter::ConvertToBulletVector3(from), end = TypeConverter::ConvertToBulletVector3(to);
		btCollisionWorld::ClosestRayResultCallback rayCallback(start, end); // use a built in call back function to cast the ray 
		rayCallback.m_collisionFilterMask = filter; // define what it collides with
		world->rayTest(start, end, rayCallback);
		if(rayCallback.hasHit()) { // if the ray intersected an object
			GameObjectData* gameObjectData = (GameObjectData*)rayCallback.m_collisionObject->getUserPointer();
			if(gameObjectData) { // and the object is recognisable as a GameObject
				// return the Intersection data
				std::weak_ptr<GameObject> g = std::dynamic_pointer_cast<GameObject>(gameObjectData->gameObject.lock());
				std::shared_ptr<GameObject> gameObject = g.lock();
				return RayCastHit(true, TypeConverter::ConvertToGLMVector3(rayCallback.m_hitNormalWorld), TypeConverter::ConvertToGLMVector3(rayCallback.m_hitPointWorld), rayCallback.m_closestHitFraction, gameObject);
			}
		}
		return RayCastHit();
	}

	std::vector<RayCastHit> PhysicsSystem::RaycastAll(const glm::vec3 & from, const glm::vec3 & to, const int & filter) {
		std::vector<RayCastHit> toReturn = std::vector<RayCastHit>(); // create an array of hits
		const btVector3 start = TypeConverter::ConvertToBulletVector3(from), end = TypeConverter::ConvertToBulletVector3(to);
		btCollisionWorld::AllHitsRayResultCallback rayCallback(start, end); // use a built in call back function to cast the ray 
		rayCallback.m_collisionFilterMask = filter; // define what it collides with
		world->rayTest(start, end, rayCallback);
		for(int i = 0; i < rayCallback.m_hitFractions.size(); i++) {// for each ray intersection
			GameObjectData* gameObjectData = (GameObjectData*)rayCallback.m_collisionObjects[i]->getUserPointer();
			if(gameObjectData) { // if the object is recognisable as a GameObject
				// add the Intersection data to the array
				std::weak_ptr<GameObject> g = std::dynamic_pointer_cast<GameObject>(gameObjectData->gameObject.lock());
				std::shared_ptr<GameObject> gameObject = g.lock();
				toReturn.push_back(RayCastHit(true, TypeConverter::ConvertToGLMVector3(rayCallback.m_hitNormalWorld[i]), TypeConverter::ConvertToGLMVector3(rayCallback.m_hitPointWorld[i]), (to - from).length() * rayCallback.m_hitFractions[i], gameObject));
			}
		}
		return toReturn;
	}

	const bool PhysicsSystem::GetDrawDebug() const {
		return drawDebug;
	}

	bool PhysicsSystem::RemoveBody(btRigidBody * body) {
		if(world && body) {
			world->removeRigidBody(body);
			return true;
		}
		return false;
	}

	const glm::vec3 PhysicsSystem::GetGravity() const {
		return gravity;
	}

	void PhysicsSystem::SetGravity(const glm::vec3 & grav) {
		if(world) {
			gravity = grav;
			world->setGravity(TypeConverter::ConvertToBulletVector3(gravity));
		}
	}

	const glm::vec3 PhysicsSystem::GetGravity() {
		return gravity;
	}

	bool PhysicsSystem::BeginContactCallBack(btManifoldPoint & cp, const btCollisionObject * body1, const btCollisionObject * body2) {
		GameObjectData* gameObjectData1 = (GameObjectData*)body1->getUserPointer(), *gameObjectData2 = (GameObjectData*)body2->getUserPointer();
		if(gameObjectData1 && gameObjectData2) { // if both objects are recognisable as a GameObjects
			// unwrap the GameObjectData
			std::shared_ptr<GameObject> gameObject1 = gameObjectData1->gameObject.lock();
			std::shared_ptr<GameObject> gameObject2 = gameObjectData2->gameObject.lock();
			// cache the normal and contact point
			glm::vec3 normal = TypeConverter::ConvertToGLMVector3(cp.m_normalWorldOnB);
			glm::vec3 contactPoint = TypeConverter::ConvertToGLMVector3(cp.m_positionWorldOnA);
			Collision collision;
			collision.point = contactPoint;
			// send the collision notifications to the GameObjects
			if(gameObject1) {
				collision.normal = normal;
				collision.distance = cp.m_distance1;
				collision.gameObject = gameObject2;
				gameObject1->OnCollisionEnter(collision);
			}
			if(gameObject2) {
				collision.gameObject = gameObject1;
				collision.distance = cp.getDistance();
				collision.normal = -normal;
				gameObject2->OnCollisionEnter(collision);
			}
			if(!cp.m_userPersistentData) { // create Cache to be used when the Collision ends
				GameObjectsCache *cache = new GameObjectsCache();
				cache->gameObject1 = gameObject1;
				cache->gameObject2 = gameObject2;
				cp.m_userPersistentData = cache;
			}
		}
		return true;
	}

	bool PhysicsSystem::EndContactCallBack(GameObjectsCache * data) {
		if(data) { //if Collision cache was generated
			// unwrap the GameObjectsCache
			std::shared_ptr<GameObject> gameObject1 = data->gameObject1.lock(), gameObject2 = data->gameObject2.lock();
			// create a default collision, since the objects are not colliding anymore the only relevant value if gameObject
			Collision collision; 
			// send the collision end notifications to the GameObjects
			if(gameObject1) {
				collision.gameObject = gameObject2;
				gameObject1->OnCollisionExit(collision);
			}
			if(gameObject2) {
				collision.gameObject = gameObject1;
				gameObject2->OnCollisionExit(collision);
			}
			delete data;
			data = nullptr;
		}
		return false;
	}

	void PhysicsSystem::SetDrawDebug(const bool & shouldDraw) {
		drawDebug = shouldDraw;
	}
}