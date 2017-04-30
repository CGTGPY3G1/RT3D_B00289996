#include "Moveable.h"
#include "Transform.h"
namespace B00289996 {
	Moveable::Moveable(const glm::vec3 & position, const glm::quat & rotation, const glm::vec3 & scale) {
		transform = std::make_shared<Transform>(position, rotation, scale); // create the new transform node
	}

	Moveable::~Moveable() {
	}

	std::weak_ptr<Transform> Moveable::GetTransform() {
		return transform;
	}
}