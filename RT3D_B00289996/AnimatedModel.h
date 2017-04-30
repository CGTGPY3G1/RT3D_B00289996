#pragma once
#ifndef B00289996_ANIMATED_MODEL_H
#define B00289996_ANIMATED_MODEL_H
#include "Model.h"
#include "RT3D\md2model.h"
namespace B00289996 {
	/// <summary>
	/// Types of available models
	/// </summary>
	enum AnimatedModelType {
		Human = 0,
		Goblin = 1,
		Skeleton = 2,
		Alien = 3,
		Yoshi = 4,
		NumberOfModels
	};

	/// <summary>
	/// Wrapper for md2model class
	/// </summary>
	/// <seealso cref="Model" />
	/// <seealso cref="std::enable_shared_from_this{AnimatedModel}" />
	class AnimatedModel : public Model, public std::enable_shared_from_this<AnimatedModel> {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="AnimatedModel"/> class. Can Create Human, Goblin, Skeleton, Alien and Yoshi Model Types
		/// </summary>
		/// <param name="type">The type.</param>
		AnimatedModel(const AnimatedModelType & type);
		/// <summary> Finalizes an instance of the <see cref="AnimatedModel"/> class. </summary>
		~AnimatedModel();
		/// <summary>
		/// Updates the Animation.
		/// </summary>
		/// <param name="delta">The frame time step.</param>
		void Update(const float & delta) override;
		void Render(const glm::mat4 & transform, const GLuint & shaderID) override;
		/// <summary>
		/// Loads the specified model and texture (must both be placed in RT3D\md2 models).
		/// </summary>
		/// <param name="modelName">Name of the model.</param>
		/// <param name="textureName">Name of the texture.</param>
		void Load(const std::string & modelName, const std::string & textureName);
		/// <summary>
		/// Gets the animation speed.
		/// </summary>
		/// <returns>the animation speed</returns>
		const float GetAnimationSpeed() const;
		/// <summary>
		/// Sets the animation speed.
		/// </summary>
		/// <param name="speed">The speed.</param>
		void SetAnimationSpeed(const float & speed);
		/// <summary>
		/// Gets the current animation frame ID.
		/// </summary>
		/// <returns></returns>
		const int GetCurrentAnimation();
		/// <summary>
		/// Sets the current animation frame ID.
		/// </summary>
		/// <param name="id">The frame ID.</param>
		void SetCurrentAnimation(const int & id);
	private:
		/// <summary> The md2model </summary>
		md2model model;
		/// <summary> The animation speed </summary>
		float animationSpeed = 1.0f;
		/// <summary> The filenames of the models </summary>
		static const std::string modelNames[AnimatedModelType::NumberOfModels];
	};
}
#endif // !B00289996_ANIMATED_MODEL_H
