#pragma once
#ifndef B00289996_MOVE_SEQUENCE_BEHAVIOUR_H
#define B00289996_MOVE_SEQUENCE_BEHAVIOUR_H
#include "Behaviour.h"
#include <glm\gtc\matrix_transform.hpp>
#include <vector>
namespace B00289996 {
	class AutoRotate : public Behaviour {
	public:
		AutoRotate(const glm::vec3 & rotationAxis, const float & rotationAlpha);
		~AutoRotate();
		void Update(const float & deltaTime) override;
		void SetValues(const glm::vec3 & axis, const float & alpha);
	private:
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
		float alpha = 90.0f;
	};
}
#endif // !B00289996_MOVE_SEQUENCE_BEHAVIOUR_H