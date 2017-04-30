#include "Light.h"
#include "Transform.h"
namespace B00289996 {
	Light::Light(const glm::vec3 & position) : Moveable(position), std::enable_shared_from_this<Light>() {
	}

	Light::~Light() {
	}

	void Light::AttatchToShader(const GLuint & shaderID) {
		glm::vec3 position = transform->GetPosition();
		l.position[0] = position.x; l.position[1] = position.y; l.position[2] = position.z;
		rt3d::setLight(shaderID, l);
		rt3d::setLightPos(shaderID, l.position);
	}

	glm::vec4 Light::GetAmbient() {
		return glm::vec4(l.ambient[0], l.ambient[1], l.ambient[2], l.ambient[3]);
	}

	void Light::SetAmbient(const glm::vec4 & colour) {
		SetAmbient(colour.x, colour.y, colour.z, colour.w);
	}

	void Light::SetAmbient(const float & r, const float & g, const float & b, const float & a) {
		l.ambient[0] = r; l.ambient[1] = g; l.ambient[2] = b; l.ambient[3] = a;
	}

	glm::vec4 Light::GetDiffuse() {
		return glm::vec4(l.diffuse[0], l.diffuse[1], l.diffuse[2], l.diffuse[3]);
	}

	void Light::SetDiffuse(const glm::vec4 & colour) {
		SetDiffuse(colour.x, colour.y, colour.z, colour.w);
	}

	void Light::SetDiffuse(const float & r, const float & g, const float & b, const float & a) {
		l.diffuse[0] = r; l.diffuse[1] = g; l.diffuse[2] = b; l.diffuse[3] = a;
	}

	void Light::SetSpecular(const glm::vec4 & colour) {
		SetSpecular(colour.x, colour.y, colour.z, colour.w);
	}

	void Light::SetSpecular(const float & r, const float & g, const float & b, const float & a) {
		l.specular[0] = r; l.specular[1] = g; l.specular[2] = b; l.specular[3] = a;
	}

	glm::vec4 Light::GetSpecular() {
		return glm::vec4(l.specular[0], l.specular[1], l.specular[2], l.specular[3]);
	}
}
