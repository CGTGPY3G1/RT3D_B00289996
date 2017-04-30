#pragma once
#ifndef B00289996_MOVEABLE_H
#define B00289996_MOVEABLE_H
#include <memory>
#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>
namespace B00289996 {
	class Transform;
	/// <summary>
	/// Abstraction for Transformable objects to inherit from
	/// </summary>
	class Moveable {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Moveable"/> class.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="rotation">The rotation.</param>
		/// <param name="scale">The scale.</param>
		Moveable(const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat & rotation = glm::quat(), const glm::vec3 & scale = glm::vec3(1.0f, 1.0f, 1.0f));
		/// <summary>
		/// Finalizes an instance of the <see cref="Moveable"/> class.
		/// </summary>
		~Moveable();
		/// <summary>
		/// Gets the transform.
		/// </summary>
		/// <returns></returns>
		std::weak_ptr<Transform> GetTransform();
	protected:
		std::shared_ptr<Transform> transform;
	};
}

#endif // !B00289996_MOVEABLE_H
