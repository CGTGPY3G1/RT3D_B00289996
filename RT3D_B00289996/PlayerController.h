#pragma once
#ifndef B00289996_PLAYER_CONTROLLER_H
#define B00289996_PLAYER_CONTROLLER_H
#include "Behaviour.h"


namespace B00289996 {
	struct Collision;
	class AnimatedModel;
	/// <summary>
	/// Controls Player Movement and Animation
	/// </summary>
	/// <seealso cref="Behaviour" />
	class PlayerController : public Behaviour {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PlayerController"/> class.
		/// </summary>
		PlayerController();
		/// <summary>
		/// Finalizes an instance of the <see cref="PlayerController"/> class.
		/// </summary>
		~PlayerController();
		/// <summary>
		/// For fixed time updates.
		/// </summary>
		/// <param name="fixedDeltaTime">The fixed time step.</param>
		/// <seealso cref="Behaviour" />
		void FixedUpdate(const float & fixedDeltaTime) override;
		/// <summary>
		/// Sets the animated model.
		/// </summary>
		/// <param name="model">The model.</param>
		void SetAnimatedModel(const std::shared_ptr<AnimatedModel> & model);
		/// <summary>
		/// Called when Object is Colliding.
		/// <param name="collision">The collision data.</param>
		/// </summary>
		void OnCollisionEnter(const Collision & collision) override;
		/// <summary>
		/// Called when Object stops Colliding.
		/// </summary>
		/// <param name="collision">The collision data.</param>
		void OnCollisionExit(const Collision & collision) override;
	private:
		/// <summary>
		/// Changes local gravity and flags object as either on ground or not.
		/// </summary>
		/// <param name="setGrounded">the value to set, true if on ground else false.</param>
		void SetGrounded(const bool & setGrounded);
		/// <summary> true if player is on ground </summary>
		bool grounded = false;
		/// <summary> The Animated Model Representing the Player </summary>
		std::weak_ptr<AnimatedModel> model;
		/// <summary> The current animation </summary>
		int currentAnim = 0;
	};
}
#endif // !B00289996_PLAYER_CONTROLLER_H