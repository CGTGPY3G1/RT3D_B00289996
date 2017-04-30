#include "AnimatedModel.h"
#include "Time.h"
#include "Transform.h"
#include "GL\glut.h"
namespace B00289996 {
	/// <summary>
	/// The model names{CC2D43FA-BBC4-448A-9D0B-7B57ADF2655C}
	/// </summary>
	const std::string AnimatedModel::modelNames[AnimatedModelType::NumberOfModels] = {"hayden", "hobgoblin", "hueteotl", "pac3D", "yoshi"};

	AnimatedModel::AnimatedModel(const AnimatedModelType & type) : Model(Animated) {
		//load the model texture (models and textures should always have same name or a manual call to Load will be required)
		Load(AnimatedModel::modelNames[type], AnimatedModel::modelNames[type]);
	}

	AnimatedModel::~AnimatedModel() {
	}

	void AnimatedModel::Update(const float & delta) {
		model.Animate(animationSpeed * delta); // update the animation
		rt3d::updateMesh(modelID, RT3D_VERTEX, model.getAnimVerts(), model.getVertDataSize()); // update the mesh
	}

	void AnimatedModel::Render(const glm::mat4 & transform, const GLuint & shaderID) {
		glDepthMask(GL_TRUE); // make sure depth test is on
		glCullFace(GL_FRONT); // md2 faces are defined clockwise, so cull front face
		rt3d::setActiveTexture(textureID); // set the texture
		rt3d::setMaterial(shaderID, material); // and the material
		rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(transform)); // and the model view transform
		rt3d::drawMesh(modelID, vertexCount, GL_TRIANGLES);
		glCullFace(GL_BACK);// switch to culling back face
	}

	void AnimatedModel::Load(const std::string & modelName, const std::string & textureName) {
		modelID = model.ReadMD2Model(("RT3D\\md2 models\\" + modelName + ".MD2").c_str()); // load the model
		textureID = rt3d::loadBitmap(("RT3D\\md2 models\\" + textureName + ".bmp").c_str()); // load the texture
		vertexCount = model.getVertDataCount(); // store the models vertex count
	}

	const float AnimatedModel::GetAnimationSpeed() const {
		return animationSpeed;
	}

	void AnimatedModel::SetAnimationSpeed(const float & speed) {
		animationSpeed = speed;
	}

	const int AnimatedModel::GetCurrentAnimation() {
		return model.getCurrentAnim();
	}

	void AnimatedModel::SetCurrentAnimation(const int & id) {
		model.setCurrentAnim(id);
	}
}