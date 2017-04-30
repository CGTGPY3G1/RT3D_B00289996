#include "GameObjectFactory.h"
#include "GameObject.h"
#include <algorithm>
#include "IndexedModel.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Random.h"
#include "AutoRotate.h"
namespace B00289996 {
	GameObjectFactory::GameObjectFactory() {
	}

	GameObjectFactory & GameObjectFactory::GetInstance() {
		static GameObjectFactory instance;
		return instance;
	}

	GameObjectFactory::~GameObjectFactory() {
	}

	void GameObjectFactory::Update(const float & deltaTime) {
		for(std::vector<std::shared_ptr<GameObject>>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i) {
			(*i)->Update(deltaTime);
		}
	}

	void GameObjectFactory::FixedUpdate(const float & fixedDeltaTime) {
		for(std::vector<std::shared_ptr<GameObject>>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i) {
			(*i)->FixedUpdate(fixedDeltaTime);
		}
	}

	void GameObjectFactory::Render(const GLuint & shaderID) {
		for(std::vector<std::shared_ptr<GameObject>>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i) {
			(*i)->Render(shaderID);
		}
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateGameObject(const std::string & name, const glm::vec3 & position, const glm::quat & rotation, const glm::vec3 & scale) {
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(name, position, rotation, scale);
		gameObjects.push_back(gameObject);
		return gameObject;
	}

	std::weak_ptr<GameObject> GameObjectFactory::GetGameObject(const std::string & name) {
		std::vector<std::shared_ptr<GameObject>>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), FindWithName(name));
		if(it != gameObjects.end()) return (*it);
		return std::weak_ptr<GameObject>();
	}


	void GameObjectFactory::DeleteGameObject(const std::shared_ptr<GameObject> & toDelete) {
		std::vector<std::shared_ptr<GameObject>>::iterator it = std::find(gameObjects.begin(), gameObjects.end(), toDelete);
		if(it != gameObjects.end()) {
			toBeDeleted.push_back(*it);
			gameObjects.erase(it);
		}
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreatePrefab(const PrefabObject & type, const std::string & name, const glm::vec3 & position, const glm::quat & rotation, const glm::vec3 & scale) {
		std::shared_ptr<GameObject> toReturn;
		if(type == PrefabObject::Barrel) {
			toReturn = CreateGameObject(name, position, rotation, scale).lock();
			toReturn->SetTag("Barrel");
			std::shared_ptr<IndexedModel> barrelModel = std::make_shared<IndexedModel>("barrel");
			barrelModel->LoadTexture("fuelcan" + std::to_string(Random::RandomInt(4)));
			toReturn->SetModel(std::static_pointer_cast<Model>(barrelModel));
			PhysicsSystem::GetInstance().AddCylinder(toReturn, glm::vec3(0.0f, -0.1f, 0.0f), glm::vec3(1, 1.4f, 1), 5.0f);
			toReturn->GetRigidBody().lock()->SetFriction(1.0f);
		}
		else if(type == PrefabObject::Globe) {
			toReturn = CreateGameObject(name, position, rotation, scale).lock();
			toReturn->SetTag("Globe");
			std::shared_ptr<IndexedModel> globeModel = std::make_shared<IndexedModel>("sphere");
			globeModel->LoadTexture("earth");
			toReturn->SetModel(std::static_pointer_cast<Model>(globeModel));
			PhysicsSystem::GetInstance().AddSphere(toReturn, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 10.0f);
		}
		else if(type == PrefabObject::Coin) {
			toReturn = GameObjectFactory::GetInstance().CreateGameObject("Coin", position).lock();
			toReturn->GetTransform().lock()->Rotate(glm::vec3(1, 0, 0), 90);
			toReturn->SetTag("Coin");
			toReturn->AddBehaviour(std::make_shared<AutoRotate>(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f));
			std::shared_ptr<IndexedModel> coinModel = std::make_shared<IndexedModel>("coin");
			coinModel->LoadTexture("coin");
			coinModel->SetOpacity(0.4f);
			toReturn->SetModel(std::static_pointer_cast<Model>(coinModel));
			PhysicsSystem::GetInstance().AddCylinder(toReturn, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.1f, 1.0f), 0.0f);
			toReturn->GetRigidBody().lock()->SetTrigger(true);
		}
		else if(type == PrefabObject::Wall) {
			toReturn = GameObjectFactory::GetInstance().CreateGameObject("Wall", position, rotation).lock();
			std::shared_ptr<IndexedModel> wallModel = std::make_shared<IndexedModel>("wall");
			wallModel->LoadTexture("concrete");
			toReturn->SetModel(std::static_pointer_cast<Model>(wallModel));
			PhysicsSystem::GetInstance().AddBox(toReturn, glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(202.0f, 10.0f, 2.0f), 0.0f);
			toReturn->SetTag("Wall");
			toReturn->GetRigidBody().lock()->SetCollisionGroup(btBroadphaseProxy::StaticFilter);
		}
		return toReturn;
	}

	std::weak_ptr<GameObject> GameObjectFactory::CreateBuilding(const int & version, const glm::vec3 & position, const glm::quat & rotation, const glm::vec3 & scale) {
		if(version <= 0 || version >= 6) {
			std::cout << "Invalid Building Number" << std::endl;
			return std::weak_ptr<GameObject>();
		}
		std::shared_ptr<GameObject> toReturn = GameObjectFactory::GetInstance().CreateGameObject("Wall", position, rotation).lock();
		std::shared_ptr<IndexedModel> wallModel = std::make_shared<IndexedModel>("building" + std::to_string(version));
		wallModel->LoadTexture("buildings");
		toReturn->SetModel(std::static_pointer_cast<Model>(wallModel));
		glm::vec3 size, offset;
		switch(version) {
		case 1:
			size = glm::vec3(36.3f, 25.0f, 28.9f);
			break;
		case 2:
			size = glm::vec3(44.0f, 23.85f, 51.1f);
			break;
		case 3:
			size = glm::vec3(20.0f, 34.9f, 20.0f);
			break;
		case 4:
			size = glm::vec3(19.8f, 34.3f, 19.8f);
			break;
		case 5:
			size = glm::vec3(22.7f, 34.4f, 25.15f);
			break;
		default:
			size = glm::vec3(1, 1, 1);
			offset = glm::vec3(0, 0, 0);
			break;
		}
		offset = glm::vec3(0.0f, size.y *0.5f, 0.0f);
		PhysicsSystem::GetInstance().AddBox(toReturn, offset, size, 0.0f);
		toReturn->SetTag("Building");
		toReturn->GetRigidBody().lock()->SetCollisionGroup(btBroadphaseProxy::StaticFilter);
		return std::weak_ptr<GameObject>();
	}

	const unsigned int GameObjectFactory::NumberOfGameObjects() {
		return gameObjects.size();
	}

	void GameObjectFactory::UpdateTransforms() {
		for(std::vector<std::shared_ptr<GameObject>>::iterator i = gameObjects.begin(); i != gameObjects.end(); ++i) {
			(*i)->GetTransform().lock()->UpdateTransform();
		}
	}

	void GameObjectFactory::FlushObject() {
		toBeDeleted.clear();
	}
}