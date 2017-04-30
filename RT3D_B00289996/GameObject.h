#pragma once
#ifndef B00289996_GAMEOBJECT_H
#define B00289996_GAMEOBJECT_H
#include "Moveable.h"
#include <string>
#include <vector>
#include "Model.h"
#include "Behaviour.h"

namespace B00289996 {
	class RigidBody;
	struct Collision;
	/// <summary>
	/// Transformable, potentially collidable, potentially visible visible object with attchable behaviour system
	/// </summary>
	/// <seealso cref="Moveable" />
	/// <seealso cref="std::enable_shared_from_this{GameObject}" />
	class GameObject : public Moveable, public std::enable_shared_from_this<GameObject> {
		friend class PhysicsSystem;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="GameObject"/> class.
		/// </summary>
		GameObject();
		/// <summary>
		/// Initializes a new instance of the <see cref="GameObject"/> class.
		/// </summary>
		/// <param name="name">The name.</param>
		/// <param name="position">The position.</param>
		/// <param name="rotation">The rotation.</param>
		/// <param name="scale">The scale.</param>
		GameObject(const std::string & name, const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat & rotation = glm::quat(), const glm::vec3 & scale = glm::vec3(1.0f, 1.0f, 1.0f));
		/// <summary>
		/// Finalizes an instance of the <see cref="GameObject"/> class.
		/// </summary>
		~GameObject();
		/// <summary>
		/// Gets the name.
		/// </summary>
		/// <returns>the gameobjects name</returns>
		std::string GetName();
		/// <summary>
		/// Sets the name.
		/// </summary>
		/// <param name="name">The new name.</param>
		void SetName(const std::string & name);
		/// <summary>
		/// Gets the tag.
		/// </summary>
		/// <returns>the gameobjects tag</returns>
		std::string GetTag();
		/// <summary>
		/// Sets the tag.
		/// </summary>
		/// <param name="tag">The tag.</param>
		void SetTag(const std::string & tag);
		/// <summary>
		/// Updates the gameobject by the specified delta time.
		/// </summary>
		/// <param name="deltaTime">The frame time step.</param>
		void Update(const float & deltaTime);
		/// <summary>
		/// Updates the gameobject by the specified fixed delta time (for physics updates)
		/// </summary>
		/// <param name="fixedDeltaTime">The physics time step.</param>
		void FixedUpdate(const float & fixedDeltaTime);
		/// <summary>
		/// Renders the object using the specified shader program.
		/// </summary>
		/// <param name="shaderID">The shader program.</param>
		void Render(const GLuint & shaderID);
		/// <summary>
		/// Gets the attached rigid body (if any).
		/// </summary>
		/// <returns>the attached rigid body (if any)</returns>
		std::weak_ptr<RigidBody> GetRigidBody();
		/// <summary>
		/// Sets the renderable model of this object.
		/// </summary>
		/// <param name="newModel">The new model.</param>
		void SetModel(const std::shared_ptr<Model> & newModel);
		/// <summary>
		/// Gets the renderable model of this object.
		/// </summary>
		/// <returns>the renderable model (if any)</returns>
		std::weak_ptr<Model> GetModel();
		/// <summary>
		/// Attach the provided behaviour.
		/// </summary>
		/// <param name="newBehaviour">The new behaviour.</param>
		void AddBehaviour(const std::shared_ptr<Behaviour> & newBehaviour);
		/// Only Generates compile-time implementations for functions where T is derived from Behaviour
		/// <summary>
		/// Gets the first found behaviour of the type specified by <T>  .
		/// </summary>
		/// <returns>the specified behavior (if it is attached)</returns>
		template <typename T = std::enable_if<std::is_base_of<Behaviour, T>::value>::type> const std::weak_ptr<T> GetBehaviour();
		bool operator == (const GameObject & other);
		bool operator == (const std::shared_ptr<GameObject> & other);
		/// <summary>
		/// Gets this GameObjects unique identifier.
		/// </summary>
		/// <returns>this GameObjects unique identifier</returns>
		const unsigned int GetID();
		/// <summary>
		/// Called when this GameObject begins colliding with another.
		/// </summary>
		/// <param name="collision">The collision data.</param>
		void OnCollisionEnter(const Collision & collision);
		/// <summary>
		/// Called when this GameObject stops colliding with another.
		/// </summary>
		/// <param name="collision">The collision data.</param>
		void OnCollisionExit(const Collision & collision);
		/// <summary>
		/// Destroys this GameObject.
		/// </summary>
		void Destroy();
	private:
		std::shared_ptr<RigidBody> rigidBody; // for potential use with physics system
		std::shared_ptr<Model> model; // for potential use with rendering system
		std::vector<std::shared_ptr<Behaviour>> behaviours; // for adding extra functionality
		std::string name, tag; // used for object identifiction
		unsigned int id;
	};
}

#endif // !B00289996_GAMEOBJECT_H
namespace B00289996 {
	template<typename T>
	inline const std::weak_ptr<T> GameObject::GetBehaviour() {
		for(std::vector<std::shared_ptr<Behaviour>>::iterator i = behaviours.begin(); i != behaviours.end(); ++i) {
			std::shared_ptr<T> t = std::dynamic_pointer_cast<T>(*i); // try to cast the behaviour to the requested type
			if(t) return t; // if the cast was successful, return the behaviour
		}
		return std::weak_ptr<T>(); // return a null weak pointer
	}

	/// <summary>
	/// Find a GameObject with the provided name (Used for find_if operations)
	/// </summary>
	struct FindWithName {
		FindWithName(const std::string & goName) : name(goName) {}
		bool operator()(std::shared_ptr<GameObject> l) { return l->GetName().compare(name) == 0; }
	private:
		std::string name;
	};
}