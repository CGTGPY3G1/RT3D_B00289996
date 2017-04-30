#pragma once
#ifndef B00289996_INDEXED_MODEL_H
#define B00289996_INDEXED_MODEL_H
#include "Model.h"
#include "RT3D\md2model.h"
#include "glm\mat4x4.hpp"
namespace B00289996 {
	/// <summary>
	/// Types of Indexed Model
	/// </summary>
	enum IndexedModelType {
		Cube = 0,
		Sphere = 1,
		Cylinder = 2,
		Capsule = 3,
		Quad = 4,
		NonPrimitive = 5
	};

	/// <summary>
	/// OBJ Model Data
	/// </summary>
	struct OBJData {
		GLuint indexCount, vertCount, modelId;
		IndexedModelType type;
	};

	/// <summary>
	/// An Indexed Model
	/// </summary>
	/// <seealso cref="Model" />
	/// <seealso cref="std::enable_shared_from_this{IndexedModel}" />
	class IndexedModel : public Model, public std::enable_shared_from_this<IndexedModel> {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="IndexedModel"/> class.
		/// </summary>
		/// <param name="name">The file name (excluding format).</param>
		IndexedModel(const std::string & name);
		/// <summary>
		/// Finalizes an instance of the <see cref="IndexedModel"/> class.
		/// </summary>
		~IndexedModel();
		/// <summary>
		/// Renders the model using the specified transform and shader program.
		/// </summary>
		/// <param name="transform">The transform.</param>
		/// <param name="shaderID">The shader program.</param>
		void Render(const glm::mat4 & transform, const GLuint & shaderID) override;
		/// <summary>
		/// Gets the type of the indexed model.
		/// </summary>
		/// <returns>the type of the indexed model</returns>
		const IndexedModelType GetIndexedModelType() const;
	private:
		GLuint indexCount; // number of ondices
		IndexedModelType indexedType;
	};
}
#endif // !B00289996_INDEXED_MODEL_H
