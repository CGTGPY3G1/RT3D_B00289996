#include "GameObject.h"
#include "GameObjectFactory.h"
#include "RigidBody.h"
#include "Transform.h"
#include "PhysicsSystem.h"
#include <glm\gtx\string_cast.hpp>
#include "Collision.h"
namespace B00289996 {
	GameObject::GameObject() {
	}

	GameObject::GameObject(const std::string & name, const glm::vec3 & position, const glm::quat & rotation, const glm::vec3 & scale) : Moveable(position, rotation, scale) {
		static unsigned int idG = 0;
		this->name = name; 
		tag = "Default"; // All gameobjects begin with the default tag
		id = ++idG;
		if(id == 0) id = ++idG; // generate a new unique id
	}

	GameObject::~GameObject() {
	}

	std::string GameObject::GetName() {
		return name;
	}

	void GameObject::SetName(const std::string & name) {
		this->name = name;
	}

	std::string GameObject::GetTag() {
		return tag;
	}

	void GameObject::SetTag(const std::string & tag) {
		this->tag = tag;
	}


	void GameObject::Update(const float & deltaTime) {
		// update all behaviours (frame time)
		for(std::vector<std::shared_ptr<Behaviour>>::iterator i = behaviours.begin(); i < behaviours.end(); ++i) {
			(*i)->Update(deltaTime);
		}
		// update the model (if any)
		if(model) model->Update(deltaTime);
	}

	void GameObject::FixedUpdate(const float & fixedDeltaTime) {
		// update all behaviours (physics time)
		for(std::vector<std::shared_ptr<Behaviour>>::iterator i = behaviours.begin(); i < behaviours.end(); ++i) {
			(*i)->FixedUpdate(fixedDeltaTime);
		}
	}

	void GameObject::Render(const GLuint & shaderID) {
		// render the model (if any)
		if(model) model->Render(transform->GetWorldTransform(), shaderID);
	}

	std::weak_ptr<RigidBody> GameObject::GetRigidBody() {
		return rigidBody;
	}

	void GameObject::SetModel(const std::shared_ptr<Model>& newModel) {
		model = newModel;
	}

	std::weak_ptr<Model> GameObject::GetModel() {
		return model;
	}

	void GameObject::AddBehaviour(const std::shared_ptr<Behaviour>& newBehaviour) {
		if(newBehaviour) { // attach a new behaviour
			newBehaviour->SetOwner(shared_from_this());
			behaviours.push_back(newBehaviour);
		}
	}

	bool GameObject::operator==(const GameObject & other) {
		return id == other.id && name.compare(other.name) == 0 && tag.compare(other.tag) == 0;
	}

	bool GameObject::operator==(const std::shared_ptr<GameObject>& other) {
		return id == other->id && name.compare(other->name) == 0 && tag.compare(other->tag) == 0;
	}

	const unsigned int GameObject::GetID() {
		return id;
	}

	void GameObject::OnCollisionEnter(const Collision & collision) {
		// notify the behaviors of the collision
		for(std::vector<std::shared_ptr<Behaviour>>::iterator i = behaviours.begin(); i != behaviours.end(); ++i) {
			(*i)->OnCollisionEnter(collision);
		}
	}

	void GameObject::OnCollisionExit(const Collision & collision) {
		// notify the behaviors of the collisions end
		for(std::vector<std::shared_ptr<Behaviour>>::iterator i = behaviours.begin(); i != behaviours.end(); ++i) {
			(*i)->OnCollisionExit(collision);
		}
	}

	void GameObject::Destroy() {
		// remove the rigidbody from the physics world
		if(rigidBody) rigidBody->Destroy();
		// then remove the GameObject from the system
		GameObjectFactory::GetInstance().DeleteGameObject(shared_from_this());
	}
}
