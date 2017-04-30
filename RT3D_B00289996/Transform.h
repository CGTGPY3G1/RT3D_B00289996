#pragma once
#ifndef B002899996_TRANSFORM_H
#define B002899996_TRANSFORM_H

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\string_cast.hpp>
#include <memory>
#include <vector>
#include <glm\gtx\quaternion.hpp>
#include <btBulletDynamicsCommon.h>
namespace B00289996 {
	class Transform : public btMotionState, public std::enable_shared_from_this <Transform> {
		friend class TypeConverter;
	public:
		Transform(const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat & rotation = glm::quat(), const glm::vec3 & scale = glm::vec3(1.0f, 1.0f, 1.0f));
		~Transform();
		glm::vec3 GetPosition();
		void SetPosition(const glm::vec3 newPosition);
		void SetPosition(const float & x, const float & y, const float & z);
		glm::quat GetRotation();
		void SetRotation(const glm::quat & newRotation);
		void Translate(const float & x, const float & y, const float & z, const bool & worldSpace = false);
		void Translate(const glm::vec3 & displacement, const bool & worldSpace = false);
		void Rotate(const glm::vec3 & axis, const float & angle);
		void Scale(const float & scale);
		void SetScale(const float & x, const float & y, const float & z);
		void SetScale(const glm::vec3 & scale);
		glm::vec3 GetScale();
		glm::vec3 GetForward();
		void SetForward(const glm::vec3 & newForward, const glm::vec3 & worldUp = glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 GetUp();
		glm::vec3 GetRight();
		glm::vec3 TransformToWorldPoint(glm::vec3 point);
		void SetParent(std::weak_ptr<Transform> newParent);
		std::weak_ptr<Transform> GetParent();

		void SlerpRotation(const glm::quat & slerpTo, const float & alpha);
		static glm::quat LookAt(const glm::vec3 & eye, const glm::vec3 & centre, const glm::vec3 & up = glm::vec3(0.0f, 1.0f, 0.0f));
		static glm::quat SlerpQuaternion(const glm::quat & slerpFrom, const glm::quat & slerpTo, const float & alpha);
		void LerpPosition(const glm::vec3 & lerpTo, const float & alpha);
		static glm::vec3 SlerpVec3(const glm::vec3 & slerpFrom, const glm::vec3 & slerpTo, const float & alpha);
		size_t NumberOfChildren();
		std::weak_ptr<Transform> GetChild(const unsigned int index);
		std::vector<std::weak_ptr<Transform>> GetChildren();
		const glm::mat4 & GetWorldTransform();
		void getWorldTransform(btTransform& worldTrans) const override;
		void UpdateTransform();
		//Bullet only calls the update of worldtransform for active objects
		void setWorldTransform(const btTransform& worldTrans) override;
		static const glm::mat4 IDENTITY;
	protected:
		void AddChild(std::weak_ptr<Transform> child);
		void RemoveChild(std::weak_ptr<Transform> child);
		void RemoveChild(const unsigned int index);
		void SetDirty();
		glm::vec3 localPosition = glm::vec3(0.0f), worldPosition, localScale = glm::vec3(1.0f), worldScale = glm::vec3(1.0f), forward, up, right;
		glm::quat localRotation, worldRotation;
		glm::mat4 worldTransform, localTransform, world2Local;

		bool dirty = true;
		unsigned int id;
		std::weak_ptr<Transform> parent;
		std::vector<std::weak_ptr<Transform>> children;
	private:
		glm::vec3 NULL_VEC3;
		glm::vec4 NULL_VEC4;
	};
}
#endif // !B002899996_TRANSFORM_H
