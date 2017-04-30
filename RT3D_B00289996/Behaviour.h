#pragma once
#ifndef B00289996_BEHAVIOUR_H
#define B00289996_BEHAVIOUR_H
#include <memory>

namespace B00289996 {
	// Forward Declarations for GameObject and Collision
	class GameObject;
	struct Collision;
	/// <summary>
	/// A scriptable class used to facilitate custom behaviours
	/// </summary>
	class Behaviour {
		friend class GameObject;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Behaviour"/> class.
		/// </summary>
		Behaviour();
		/// <summary>
		/// Finalizes an instance of the <see cref="Behaviour"/> class.
		/// </summary>
		~Behaviour();
		/// <summary>
		/// For frame time updates.
		/// </summary>
		/// <param name="deltaTime">The frame time step.</param>
		virtual void Update(const float & deltaTime) {}
		/// <summary>
		/// For fixed time updates.
		/// </summary>
		/// <param name="fixedDeltaTime">The fixed time step.</param>
		virtual void FixedUpdate(const float & fixedDeltaTime) {}
		/// <summary>
		/// Called when Object is Colliding.
		/// </summary>
		/// <param name="collision">The collision data.</param>
		virtual void OnCollisionEnter(const Collision & collision) {}
		/// <summary>
		/// Called when Object stops Colliding.
		/// </summary>
		/// <param name="collision">The collision data.</param>
		virtual void OnCollisionExit(const Collision & collision) {}
	protected:
		/// <summary>
		/// Determines whether this instance is attached to a GameObject.
		/// </summary>
		/// <returns> true if Attached, else false</returns>
		const bool IsAttached();
		/// <summary>
		/// Attaches this behaviour to the specified GameObject(owner).
		/// </summary>
		/// <param name="owner">The GameObject.</param>
		void SetOwner(const std::weak_ptr<GameObject> owner);
		/// <summary> The GameObject(owner) </summary>
		std::weak_ptr<GameObject> gameObject;
	};
}
#endif // !B00289996_BEHAVIOUR_H