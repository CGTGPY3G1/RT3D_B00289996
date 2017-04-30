#include "Model.h"
namespace B00289996 {
	Model::Model(const ModelType & modelType) : type(modelType) {
	}

	Model::~Model() {
	}

	ModelType Model::GetType() {
		return type;
	}

	void Model::LoadTexture(const std::string & filePath) {
		textureID = rt3d::loadBitmap(("Textures\\" + filePath + ".bmp").c_str());
	}

	void Model::SetTexture(const GLuint & id) {
		textureID = id;
	}

	rt3d::materialStruct Model::GetMaterial() {
		return material;
	}

	void Model::SetMaterial(const rt3d::materialStruct & material) {
		this->material = material;
	}

	void Model::SetOpacity(const GLfloat & alpha) {
		material.ambient[3] = alpha;
		material.diffuse[3] = alpha;
		material.specular[3] = alpha;
	}
}