#include "Transform.h"
#include <algorithm>
#include <glm\gtx\matrix_decompose.hpp>
#include <glm/ext.hpp>
#include "TypeConverter.h"
namespace B00289996 {
	const glm::mat4 Transform::IDENTITY = glm::mat4(1.0f);

	Transform::Transform(const glm::vec3 & position, const glm::quat & rotation, const glm::vec3 & scale) : btMotionState() {
		static unsigned int idGen = 0;
		id = ++idGen;
		localPosition = position; localRotation = rotation, localScale = scale;
		SetDirty();
	}

	Transform::~Transform() {
	}

	glm::vec3 Transform::GetPosition() {
		UpdateTransform();
		return worldPosition;
	}

	void Transform::SetPosition(const glm::vec3 newPosition) {
		localPosition.x = newPosition.x;
		localPosition.y = newPosition.y;
		localPosition.z = newPosition.z;
		SetDirty();
	}

	void Transform::SetPosition(const float & x, const float & y, const float & z) {
		localPosition.x = x; localPosition.y = y; localPosition.z = z;
		SetDirty();
	}

	void Transform::SetRotation(const glm::quat & newRotation) {
		localRotation = newRotation;
		SetDirty();
	}

	void Transform::Translate(const float & x, const float & y, const float & z, const bool & worldSpace) {
		UpdateTransform();
		if(!worldSpace) {
			localPosition.x += x;
			localPosition.y += y;
			localPosition.z += z;
		}
		else localPosition += glm::vec3(-glm::rotate(localRotation, (glm::vec4(x, y, z, 0.0f))));
		SetDirty();
	}

	void Transform::Translate(const glm::vec3 & displacement, const bool & worldSpace) {
		Translate(displacement.x, displacement.y, displacement.z, worldSpace);
	}

	void Transform::Rotate(const glm::vec3 & axis, const float & angle) {
		localRotation = glm::rotate(localRotation, glm::radians(angle), axis);
		SetDirty();
	}

	glm::quat Transform::GetRotation() {
		return localRotation;
	}

	void Transform::Scale(const float & scale) {
		this->localScale *= scale;
		SetDirty();
	}

	void Transform::SetScale(const float & x, const float & y, const float & z) {
		localScale.x = x; localScale.y = y; localScale.z = z;
		SetDirty();
	}

	void Transform::SetScale(const glm::vec3 & scale) {
		this->localScale = scale;
		SetDirty();
	}

	glm::vec3 Transform::GetScale() {
		return localScale;
	}

	glm::vec3 Transform::GetForward() {
		UpdateTransform();
		return forward;
	}

	void Transform::SetForward(const glm::vec3 & newForward, const glm::vec3 & worldUp) {
		glm::mat4 toDecompose = glm::lookAt(GetPosition(), GetPosition() - newForward, worldUp);
		glm::decompose(toDecompose, glm::vec3(), localRotation, glm::vec3(), glm::vec3(), glm::vec4());
		SetDirty();
	}

	glm::vec3 Transform::GetUp() {
		UpdateTransform();
		return up;
	}

	glm::vec3 Transform::GetRight() {
		UpdateTransform();
		return right;
	}

	glm::vec3 Transform::TransformToWorldPoint(glm::vec3 point) {
		UpdateTransform();
		return glm::vec3(worldTransform * glm::vec4(point, 0.0f));
	}

	void Transform::SetParent(std::weak_ptr<Transform> newParent) {
		parent = newParent;
		bool added = false;
		if(!parent.expired()) {
			std::shared_ptr<Transform> p = parent.lock();
			if(p) {
				p->AddChild(shared_from_this());
				added = true;
			}
		}
		SetDirty();
	}

	std::weak_ptr<Transform> Transform::GetParent() {
		return parent;
	}

	void Transform::SlerpRotation(const glm::quat & slerpTo, const float & alpha) {
		localRotation = glm::slerp(localRotation, slerpTo, alpha);
		SetDirty();
	}

	glm::quat Transform::LookAt(const glm::vec3 & eye, const glm::vec3 & centre, const glm::vec3 & up) {
		glm::quat toReturn;
		glm::mat4 toDecompose = glm::lookAt(eye, centre, up);
		glm::decompose(toDecompose, glm::vec3(), toReturn, glm::vec3(), glm::vec3(), glm::vec4());
		return toReturn;
	}

	glm::quat Transform::SlerpQuaternion(const glm::quat & slerpFrom, const glm::quat & slerpTo, const float & alpha) {
		return glm::slerp(slerpFrom, slerpTo, alpha);;
	}

	void Transform::LerpPosition(const glm::vec3 & lerpTo, const float & alpha) {
		localPosition = glm::lerp(localPosition, lerpTo, alpha);
		SetDirty();
	}

	glm::vec3 Transform::SlerpVec3(const glm::vec3 & slerpFrom, const glm::vec3 & slerpTo, const float & alpha) {
		return  glm::slerp(slerpFrom, slerpTo, alpha);
	}

	void Transform::setWorldTransform(const btTransform & worldTrans) {
		UpdateTransform();
		float data[16];
		worldTrans.getOpenGLMatrix(data);
		glm::mat4 m = glm::make_mat4(data);
		if(parent.use_count() > 0) {
			glm::mat4 m2 = glm::inverse(parent.lock()->GetWorldTransform());
			glm::decompose(m2 * m, glm::vec3(), localRotation, localPosition, glm::vec3(), glm::vec4());
		}
		else {
			SetPosition(TypeConverter::ConvertToGLMVector3(worldTrans.getOrigin()));
			SetRotation(TypeConverter::ConvertToGLMQuat(worldTrans.getRotation()));
		}
		SetDirty();
	}

	void Transform::AddChild(std::weak_ptr<Transform> child) {
		if(std::find_if(children.begin(), children.end(), [&](const std::weak_ptr<Transform>& other) { return !other.expired() && other.lock()->id == child.lock()->id; }) == children.end()) {
			children.push_back(child);
		};
		SetDirty();
	}

	void Transform::RemoveChild(std::weak_ptr<Transform> child) {
		std::vector<std::weak_ptr<Transform>>::iterator it = std::remove_if(children.begin(), children.end(), [&](const std::weak_ptr<Transform>& other) { return other.expired() && other.lock()->id == child.lock()->id; });
		if(it != children.end()) {
			(*it).lock()->SetParent(std::shared_ptr<Transform>());
			children.erase(it);
		};
	}

	void Transform::RemoveChild(const unsigned int index) {
		if(index < children.size()) children.erase(children.begin() + index);
	}

	size_t Transform::NumberOfChildren() {
		return children.size();
	}

	std::weak_ptr<Transform> Transform::GetChild(const unsigned int index) {
		if(index < children.size()) return children[index];
		return std::weak_ptr<Transform>();
	}

	std::vector<std::weak_ptr<Transform>> Transform::GetChildren() {
		return children;
	}


	const glm::mat4 & Transform::GetWorldTransform() {
		UpdateTransform();
		return worldTransform;
	}

	void Transform::getWorldTransform(btTransform & worldTrans) const {
		worldTrans.setOrigin(TypeConverter::ConvertToBulletVector3(localPosition));
		worldTrans.setRotation(TypeConverter::ConvertToBulletQuat(localRotation));
	}

	void Transform::UpdateTransform() {
		if(dirty) {
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), localPosition);
			glm::mat4 rotationMatrix = glm::mat4_cast(localRotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), localScale);
			localTransform = scaleMatrix * translationMatrix * rotationMatrix;
			if(parent.use_count() > 0) {
				std::shared_ptr<Transform> p = parent.lock();
				if(p) {
					p->UpdateTransform();
					worldTransform = p->worldTransform * localTransform;
					worldRotation = p->worldRotation * localRotation;
					worldScale.x = p->worldScale.x * localScale.x;
					worldScale.y = p->worldScale.y * localScale.y;
					worldScale.z = p->worldScale.z * localScale.z;
				}
				else {
					worldTransform = localTransform;
					worldRotation = localRotation;
					worldScale = localScale;
				}
			}
			else {
				worldTransform = localTransform;
				worldRotation = localRotation;
				worldScale = localScale;
			}
			worldPosition = glm::vec3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
			forward = glm::normalize(glm::vec3(worldTransform[2])), up = glm::normalize(glm::vec3(worldTransform[1])), right = glm::normalize(glm::vec3(worldTransform[0]));
			world2Local = glm::inverse(worldTransform);
			dirty = false;
		}
	}

	void Transform::SetDirty() {
		if(!dirty) {
			dirty = true;
			for(size_t i = 0; i < children.size(); i++) {
				std::shared_ptr<Transform> c = children[i].lock();
				if(c) c->SetDirty();
			}
		}
	}
}