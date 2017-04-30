#pragma once
#ifndef B00289996_MODEL_H
#define B00289996_MODEL_H
#include "RT3D\rt3d.h"
#include <memory>
#include <glm\mat4x4.hpp>
namespace B00289996 {
	/// <summary>
	/// Types of Model
	/// </summary>
	enum ModelType {
		Null,
		Indexed,
		Animated
	};
	class Transform;
	/// <summary>
	/// Model Abstraction (Used to attach Renderable Component to GameObjects)
	/// </summary>
	/// <seealso cref="std::enable_shared_from_this{Model}" />
	class Model : public std::enable_shared_from_this<Model> {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Model"/> class.
		/// </summary>
		/// <param name="modelType">Type of the model.</param>
		Model(const ModelType & modelType);
		/// <summary>
		/// Finalizes an instance of the <see cref="Model"/> class.
		/// </summary>
		~Model();
		/// <summary>
		/// Renders the model using the specified transform and shader program.
		/// </summary>
		/// <param name="transform">The transform.</param>
		/// <param name="shaderID">The shader program.</param>
		virtual void Render(const glm::mat4 & transform, const GLuint & shaderID) = 0;
		/// <summary>
		/// Interface for updating a model.
		/// </summary>
		/// <param name="delta">The delta time.</param>
		virtual void Update(const float & delta) {};
		/// <summary>
		/// Gets the type.
		/// </summary>
		/// <returns>the Model Type</returns>
		ModelType GetType();
		/// <summary>
		/// Loads the texture.
		/// </summary>
		/// <param name="filePath">The file path.</param>
		void LoadTexture(const std::string & filePath);
		/// <summary>
		/// Sets the models texture.
		/// </summary>
		/// <param name="id">The texture id.</param>
		void SetTexture(const GLuint & id);
		/// <summary>
		/// Gets the material.
		/// </summary>
		/// <returns>the material</returns>
		rt3d::materialStruct GetMaterial();
		/// <summary>
		/// Sets the material.
		/// </summary>
		/// <param name="material">The material.</param>
		void SetMaterial(const rt3d::materialStruct & material);
		/// <summary>
		/// Sets the models opacity.
		/// </summary>
		/// <param name="alpha">The opacity.</param>
		void SetOpacity(const GLfloat & alpha);
	protected:
		rt3d::materialStruct material = {
			{1.0f, 1.0f, 1.0f, 1.0f}, // ambient
			{1.0f, 1.0f, 1.0f, 1.0f}, // diffuse
			{0.2f, 0.2f, 0.2f, 1.0f}, // specular
			1.0f  // shininess
		};
		GLuint modelID, textureID, vertexCount;
		ModelType type = Null;
	};
}
#endif // !B00289996_MODEL_H
