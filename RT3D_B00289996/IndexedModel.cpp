#include "IndexedModel.h"
#include "Transform.h"
#include "RT3D\rt3dObjLoader.h"
#include <map>
namespace B00289996 {
	IndexedModel::IndexedModel(const std::string & name) : Model(Indexed) {
		static std::map<std::string, OBJData> map; // used to prevent OBJ file being loaded multiple times
		if(map.count(name) == 0) { // if OBJ file has not been loaded previously
			std::vector<GLfloat> verts;
			std::vector<GLfloat> norms;
			std::vector<GLfloat> tex_coords;
			std::vector<GLuint> indices;
			rt3d::loadObj(("OBJModels\\" + name + ".obj").c_str(), verts, norms, tex_coords, indices);
			GLuint size = indices.size();
			indexCount = size;
			vertexCount = verts.size();
			// set the IndexedModelType of the object. will be either Cube, Sphere, Cylinder, Capsule, Quad or NonPrimitive ()
			indexedType = (name.compare("cube") == 0 ? Cube : (name.compare("sphere") == 0 ? Sphere : (name.compare("cylinder") == 0 ? Cylinder : (name.compare("capsule") == 0 ? Capsule : (name.compare("quad") == 0 ? Quad : NonPrimitive)))));
			modelID = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
			// cache data for reuse
			OBJData data;
			data.indexCount = indexCount;
			data.vertCount = vertexCount;
			data.type = indexedType;
			data.modelId = modelID;
			map[name] = data;
		}
		else { // Used Cached Model ID
			OBJData data = map[name];
			indexCount = data.indexCount;
			vertexCount = data.vertCount;
			indexedType = data.type;
			modelID = data.modelId;
		}
	}

	IndexedModel::~IndexedModel() {
	}

	void IndexedModel::Render(const glm::mat4 & transform, const GLuint & shaderID) {
		const bool TRANSPARENT = material.diffuse[3] < 0.9999f;
		if(TRANSPARENT) glDepthMask(GL_FALSE); // disable the depth mask if the model if transparent
		rt3d::setMaterial(shaderID, material);
		rt3d::setActiveTexture(textureID);
		rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(transform));
		rt3d::drawIndexedMesh(modelID, indexCount, GL_TRIANGLES);
		if(TRANSPARENT) glDepthMask(GL_TRUE);
	}

	const IndexedModelType IndexedModel::GetIndexedModelType() const {
		return indexedType;
	}
}