#pragma once
#ifndef B00289996_RIGIDBODY_H
#define B00289996_RIGIDBODY_H
#include <memory>
#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>
#include "btBulletDynamicsCommon.h"

namespace B00289996 {
	class GameObject;
	struct GameObjectData;
	/// <summary>
	/// Wrapper for btRigidBody class
	/// </summary>
	class RigidBody {
		friend class PhysicsSystem;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RigidBody"/> class.
		/// </summary>
		/// <param name="g">The GameObject it is being attached to.</param>
		RigidBody(const std::shared_ptr<GameObject> & g);
		/// <summary>
		/// Finalizes an instance of the <see cref="RigidBody"/> class.
		/// </summary>
		~RigidBody();
		/// <summary>
		/// Adds a force at the centre of the body.
		/// </summary>
		/// <param name="force">The force.</param>
		/// <param name="impulse">is the force an impulse?</param>
		void AddForce(const glm::vec3 & force, const bool & impulse = false);
		/// <summary>
		/// Adds a force at an offset from the centre of the body.
		/// </summary>
		/// <param name="force">The force.</param>
		/// <param name="offset">The offset.</param>
		/// <param name="impulse">is the force an impulse?</param>
		void AddForce(const glm::vec3 & force, const glm::vec3 & offset, const bool & impulse = false);
		/// <summary>
		/// Adds torque to the body.
		/// </summary>
		/// <param name="torque">The torque.</param>
		/// <param name="impulse">has the torque to be applied as an impulse?</param>
		void AddTorque(const glm::vec3 & torque, const bool & impulse = false);
		/// <summary>
		/// Sets the linear velocity.
		/// </summary>
		/// <param name="velocity">The velocity.</param>
		void SetLinearVelocity(const glm::vec3 & velocity);
		/// <summary>
		/// Gets the linear velocity.
		/// </summary>
		/// <returns>the linear velocity</returns>
		const glm::vec3 GetLinearVelocity();
		/// <summary>
		/// Sets the angular velocity.
		/// </summary>
		/// <param name="velocity">The velocity.</param>
		void SetAngularVelocity(const glm::vec3 & velocity);
		/// <summary>
		/// Gets the angular velocity.
		/// </summary>
		/// <returns>the angular velocity</returns>
		const glm::vec3 GetAngularVelocity();
		/// <summary>
		/// Sets the position.
		/// </summary>
		/// <param name="position">The position.</param>
		void SetPosition(const glm::vec3 & position);
		/// <summary>
		/// Gets the position.
		/// </summary>
		/// <returns>the position</returns>
		const glm::vec3 GetPosition();
		/// <summary>
		/// Sets the rotation.
		/// </summary>
		/// <param name="rotationn">The rotationn.</param>
		void SetRotation(const glm::quat & rotationn);
		/// <summary>
		/// Gets the rotation.
		/// </summary>
		/// <returns>the rotation</returns>
		const glm::quat GetRotation();
		/// <summary>
		/// Sets the angular dampening.
		/// </summary>
		/// <param name="angularDampening">The angular dampening.</param>
		void SetAngularDampening(const float & angularDampening);
		/// <summary>
		/// Gets the angular dampening.
		/// </summary>
		/// <returns>the angular dampening</returns>
		const float GetAngularDampening();
		/// <summary>
		/// Sets the linear dampening.
		/// </summary>
		/// <param name="linearDampening">The linear dampening.</param>
		void SetLinearDampening(const float & linearDampening);
		/// <summary>
		/// Gets the linear dampening.
		/// </summary>
		/// <returns>the linear dampening</returns>
		const float GetLinearDampening();
		/// <summary>
		/// Sets the local gravity.
		/// </summary>
		/// <param name="gravity">The local gravity.</param>
		void SetLocalGravity(const glm::vec3 & gravity);
		/// <summary>
		/// Gets the bodies local gravity.
		/// </summary>
		/// <returns>the bodies local gravity</returns>
		const glm::vec3 GetLocalGravity();
		/// <summary>
		/// Sets the friction.
		/// </summary>
		/// <param name="friction">The friction.</param>
		void SetFriction(const float & friction);
		/// <summary>
		/// Gets the friction.
		/// </summary>
		/// <returns>the friction</returns>
		const float GetFriction();
		/// <summary>
		/// Sets the restitition.
		/// </summary>
		/// <param name="restitution">The restitution.</param>
		void SetRestitition(const float & restitution);
		/// <summary>
		/// Gets the restitition.
		/// </summary>
		/// <returns>the restitition</returns>
		const float GetRestitition();
		/// <summary>
		/// Gets the mass.
		/// </summary>
		/// <returns>the mass</returns>
		const float GetMass();
		/// <summary>
		/// Determines whether this instance is a trigger / sensor.
		/// </summary>
		/// <returns>true if the RigidBody is a trigger, else false</returns>
		const bool IsTrigger() const;
		/// <summary>
		/// Sets the RigidBody as a trigger / sensor.
		/// </summary>
		/// <param name="val">Should the body be a trigger</param>
		void SetTrigger(const bool & val);
		/// <summary>
		/// Sets the bodies collision group.
		/// </summary>
		/// <param name="group">The collision group.</param>
		void SetCollisionGroup(const int & group);
		/// <summary>
		/// Gets the collision group.
		/// </summary>
		/// <returns>the bodies collision group</returns>
		const int GetCollisionGroup();
		/// <summary>
		/// Determines whether this instance is initialised.
		/// </summary>
		/// <returns>true if initialised, else false</returns>
		const bool IsInitialised();
		/// <summary>
		/// Determines whether this instance is kinematic.
		/// </summary>
		/// <returns>true if is kinematic, else false</returns>
		const bool IsKinematic();
		/// <summary>
		/// Determines whether this instance is static.
		/// </summary>
		/// <returns>true if is static, else false</returns>
		const bool IsStatic();
		/// <summary>
		/// Determines whether this instance is dynamic.
		/// </summary>
		/// <returns>true if is dynamic, else false</returns>
		const bool IsDynamic();
		/// <summary>
		/// Destroys the RigidBody, removing it from the Physics world.
		/// </summary>
		void Destroy();
	private:
		/// <summary>
		/// Sets the reference body.
		/// </summary>
		/// <param name="body">The body.</param>
		void SetBody(btRigidBody * body);
		/// <summary>
		/// Gets data about the RigidBodies GameObject.
		/// </summary>
		/// <returns>data about the RigidBodies GameObject</returns>
		GameObjectData * GetData();
		GameObjectData * gameObjectData;
		btRigidBody * body;
		int collisionGroup = btBroadphaseProxy::DefaultFilter;
		std::weak_ptr<GameObject> gameObject;
	};
}
#endif // !B00289996_RIGIDBODY_H