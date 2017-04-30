#pragma once
#ifndef B00289996_GAMEOBJECT_FACTORY_H
#define B00289996_GAMEOBJECT_FACTORY_H
#include <vector>
#include <memory>
#include <string>
#include <glm\gtc\quaternion.hpp>
#include <glm\vec3.hpp>
#include <GL\glew.h>
namespace B00289996 {
	/// <summary>
	/// Prefabricated Object Types
	/// </summary>
	enum PrefabObject {
		Barrel = 1,
		Globe = 2,
		Coin = 3,
		Wall = 4
	};

	class GameObject;
	/// <summary>
	/// Creates and Manages the life times of GameObjects
	/// </summary>
	class GameObjectFactory {
	public:
		/// <summary>
		/// Gets the Singleton instance.
		/// </summary>
		/// <returns>the Singleton instance</returns>
		static GameObjectFactory & GetInstance();
		/// <summary>
		/// Finalizes an instance of the <see cref="GameObjectFactory"/> class.
		/// </summary>
		~GameObjectFactory();
		/// <summary>
		/// Updates all GameObjects by the specified delta time.
		/// </summary>
		/// <param name="deltaTime">The frame time step.</param>
		void Update(const float & deltaTime);
		/// <summary>
		/// Updates all GameObjects by the specified fixed delta time.
		/// </summary>
		/// <param name="fixedDeltaTime">The physics time step.</param>
		void FixedUpdate(const float & fixedDeltaTime);
		/// <summary>
		/// Renders all GameObjects using the specified shader program.
		/// </summary>
		/// <param name="shaderID">The shader program.</param>
		void Render(const GLuint & shaderID);
		/// <summary>
		/// Creates a new GameObject.
		/// </summary>
		/// <param name="name">The name.</param>
		/// <param name="position">The position.</param>
		/// <param name="rotation">The rotation.</param>
		/// <param name="scale">The scale.</param>
		/// <returns></returns>
		std::weak_ptr<GameObject> CreateGameObject(const std::string & name = "New GameObject", const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat & rotation = glm::quat(), const glm::vec3 & scale = glm::vec3(1.0f, 1.0f, 1.0f));
		/// <summary>
		/// Gets the GameObject with the specified name.
		/// </summary>
		/// <param name="name">The name.</param>
		/// <returns>the first GameObject with the specified name (if any)</returns>
		std::weak_ptr<GameObject> GetGameObject(const std::string & name);
		/// <summary>
		/// Deletes the specified GameObject.
		/// </summary>
		/// <param name="toDelete">the GameObject to delete.</param>
		void DeleteGameObject(const std::shared_ptr<GameObject> & toDelete);
		/// <summary>
		/// Creates one of the prefabricated objects using the specified values.
		/// </summary>
		/// <param name="type">The type.</param>
		/// <param name="name">The name.</param>
		/// <param name="position">The position.</param>
		/// <param name="rotation">The rotation.</param>
		/// <param name="scale">The scale.</param>
		/// <returns> the created object </returns>
		std::weak_ptr<GameObject> CreatePrefab(const PrefabObject & type, const std::string & name, const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat & rotation = glm::quat(), const glm::vec3 & scale = glm::vec3(1.0f, 1.0f, 1.0f));
		/// <summary>
		/// Creates a building of the specified version.
		/// </summary>
		/// <param name="version">The version.</param>
		/// <param name="position">The position.</param>
		/// <param name="rotation">The rotation.</param>
		/// <param name="scale">The scale.</param>
		/// <returns>the building (if the version exists)</returns>
		std::weak_ptr<GameObject> CreateBuilding(const int & version, const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat & rotation = glm::quat(), const glm::vec3 & scale = glm::vec3(1.0f, 1.0f, 1.0f));
		/// <summary>
		/// Returns the total Number of game objects.
		/// </summary>
		/// <returns></returns>
		const unsigned int NumberOfGameObjects();
		/// <summary>
		/// Updates the transforms of all GameObjects (to ensure Kinematic Rigidbodies are repositioned).
		/// </summary>
		void UpdateTransforms();
	private:
		/// <summary>
		/// Prevents a default instance of the <see cref="GameObjectFactory"/> class from being created.
		/// </summary>
		GameObjectFactory();
		/// <summary>
		/// Invalidate all GameObjects that are scheduled for deletion
		/// </summary>
		void FlushObject();
		std::vector<std::shared_ptr<GameObject>> gameObjects, toBeDeleted;
	};
}
#endif // !B00289996_GAMEOBJECT_FACTORY_H
