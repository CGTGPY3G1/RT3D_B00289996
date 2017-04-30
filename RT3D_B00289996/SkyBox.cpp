#include "SkyBox.h"
#include "IndexedModel.h"
#include "RT3D\rt3dObjLoader.h"
#include "Camera.h"
#include "Transform.h"
#include <stack>

namespace B00289996 {
	SkyBox::SkyBox() {

		vertexCount = 4;
		GLfloat vertices [] = {-2.0f, -2.0f, 0.0f,
			2.0f, -2.0f, 0.0f,
			2.0f, 2.0f, 0.0f,
			-2.0f, 2.0f, 0.0f, };
		GLfloat colours [] = {1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f};
		indexCount = 6;
		GLuint indices [] = {0, 1, 2, 0, 2, 3};

		GLfloat texCoords [] = {0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f, };
		modelID = rt3d::createMesh(vertexCount, vertices, colours, nullptr, texCoords, indexCount, indices);
		for(int i = 0; i < 6; i++) {
			textures[i] = rt3d::loadBitmap(("Textures\\skybox" + std::to_string(i) + ".bmp").c_str());
		}
		shaderID = rt3d::initShaders("RT3D\\Shaders\\textured.vert", "RT3D\\Shaders\\textured.frag");
	}

	SkyBox & SkyBox::GetInstance() {
		static SkyBox instance;
		return instance;
	}

	SkyBox::~SkyBox() {
	}

	void SkyBox::Render() {
		std::stack<glm::mat4> mvStack;
		std::shared_ptr<Transform> transform = Camera::GetMainCamera().lock()->GetTransform().lock();
		mvStack.push(Camera::GetMainCamera().lock()->GetView());
		glUseProgram(shaderID);
		rt3d::setUniformMatrix4fv(shaderID, "projection", glm::value_ptr(Camera::GetMainCamera().lock()->GetProjection()));
		glDepthMask(GL_FALSE); // make sure depth test is off

		glm::mat3 mvRotOnlyMat3 = glm::mat3(mvStack.top());
		mvStack.push(glm::mat4(mvRotOnlyMat3));

		// front
		mvStack.push(mvStack.top());
		rt3d::setActiveTexture(textures[0]);
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, -2.0f));
		mvStack.top() = glm::rotate(mvStack.top(), -glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::drawIndexedMesh(modelID, indexCount, GL_TRIANGLES);
		mvStack.pop();

		// back
		mvStack.push(mvStack.top());
		rt3d::setActiveTexture(textures[1]);
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, 2.0f));

		rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::drawIndexedMesh(modelID, indexCount, GL_TRIANGLES);
		mvStack.pop();
		// left
		mvStack.push(mvStack.top());
		rt3d::setActiveTexture(textures[2]);
		//mvStack.top() = glm::scale(mvStack.top(), scale);
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-2.0f, 0.0f, 0.0f));
		mvStack.top() = glm::rotate(mvStack.top(), -glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::drawIndexedMesh(modelID, indexCount, GL_TRIANGLES);
		mvStack.pop();

		// right
		mvStack.push(mvStack.top());
		rt3d::setActiveTexture(textures[3]);
		//mvStack.top() = glm::scale(mvStack.top(), scale);

		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(2.0f, 0.0f, 0.0f));
		mvStack.top() = glm::rotate(mvStack.top(), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::drawIndexedMesh(modelID, indexCount, GL_TRIANGLES);
		mvStack.pop();
		// top
		mvStack.push(mvStack.top());
		rt3d::setActiveTexture(textures[4]);
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 2.0f, 0.0f));
		mvStack.top() = glm::rotate(mvStack.top(), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mvStack.top() = glm::rotate(mvStack.top(), -glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::drawIndexedMesh(modelID, indexCount, GL_TRIANGLES);
		mvStack.pop();

		mvStack.push(mvStack.top());
		rt3d::setActiveTexture(textures[5]);
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, -2.0f, 0.0f));

		mvStack.top() = glm::rotate(mvStack.top(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mvStack.top() = glm::rotate(mvStack.top(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::drawIndexedMesh(modelID, indexCount, GL_TRIANGLES);
		mvStack.pop();

		// back to remainder of rendering
		glDepthMask(GL_TRUE); // make sure depth test is on
	}
}