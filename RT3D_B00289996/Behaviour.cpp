#include "Behaviour.h"
namespace B00289996 {
	Behaviour::Behaviour() {
	}

	Behaviour::~Behaviour() {
	}

	const bool Behaviour::IsAttached() {
		return gameObject.use_count() > 0; // if the owner exists return true, else return false
	}

	void Behaviour::SetOwner(const std::weak_ptr<GameObject> owner) {
		gameObject = owner;
	}
}