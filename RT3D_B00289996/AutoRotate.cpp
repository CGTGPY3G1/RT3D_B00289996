#include "AutoRotate.h"
#include "GameObject.h"
#include "Transform.h"
namespace B00289996 {
	AutoRotate::AutoRotate(const glm::vec3 & rotationAxis, const float & rotationAlpha) : axis(rotationAxis), alpha(rotationAlpha) {
	}

	AutoRotate::~AutoRotate() {
	}

	void AutoRotate::Update(const float & deltaTime) {
		if(IsAttached()) {
			gameObject.lock()->GetTransform().lock()->Rotate(axis, alpha * deltaTime); // rotate the object
		}
	}


	void AutoRotate::SetValues(const glm::vec3 & axis, const float & alpha) {
		this->axis = axis; this->alpha = alpha;
	}
}