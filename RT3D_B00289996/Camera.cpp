#include "Camera.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace B00289996 {
	std::shared_ptr<Camera> Camera::mainCamera;

	Camera::Camera(const glm::vec3 & position, const glm::quat & rotation) : Moveable(position, rotation, glm::vec3(1, 1, 1)), enable_shared_from_this<Camera>() {
		static bool first = true;
		static int cID = 0;
		// set the default shader
		shaderID = rt3d::initShaders(("RT3D\\Shaders\\" + vert + ".vert").c_str(), ("RT3D\\Shaders\\" + frag + ".frag").c_str());
		// set up the projection matrix
		SetProjection(width, height, near, far, fov);
		camId = ++cID; // generate a unique id
	}

	Camera::~Camera() {
	}

	void Camera::SetProjection(const float & width, const float & height, const float & near, const float & far, const float & fov) {
		this->width = width; this->height = height; this->near = near; this->far = far; this->fov = fov;
		projection = glm::perspective(glm::radians(fov), width / height, near, far);
	}

	void Camera::UpdateProjectionUniform() {
		glUseProgram(shaderID);
		viewProjection = projection * glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
		rt3d::setUniformMatrix4fv(shaderID, "projection", glm::value_ptr(viewProjection));
	}

	void Camera::UpdateProjectionUniform(const GLuint shaderProgram, const bool & useCamTransform, const char * uniformName) {
		glUseProgram(shaderProgram);
		viewProjection = projection * glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
		if(useCamTransform) rt3d::setUniformMatrix4fv(shaderProgram, uniformName, glm::value_ptr(viewProjection));
		else rt3d::setUniformMatrix4fv(shaderProgram, uniformName, glm::value_ptr(projection));
	}

	void Camera::SetAsMain() {
		Camera::mainCamera = shared_from_this();
	}

	std::weak_ptr<Camera> Camera::GetMainCamera() {
		return mainCamera;
	}

	bool Camera::operator==(const Camera & other) {
		return camId == other.camId;
	}

	bool Camera::operator==(const std::shared_ptr<Camera>& other) {
		return camId == other->camId;
	}

	glm::mat4 Camera::GetView() {
		return glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
	}

	glm::mat4 Camera::GetProjection() {
		return projection;
	}
	glm::mat4 Camera::GetViewProjection() {
		return projection * GetView();
	}
}