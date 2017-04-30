#pragma once
#ifndef B00289996_LIGHT_H
#define B00289996_LIGHT_H
#include <memory>
#include <string>
#include "Moveable.h"
#include "RT3D\rt3d.h"
namespace B00289996 {
	class Transform;
	/// <summary>
	/// Light Source Class
	/// </summary>
	/// <seealso cref="Moveable" />
	/// <seealso cref="std::enable_shared_from_this{Light}" />
	class Light : public Moveable, public std::enable_shared_from_this<Light> {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Light"/> class.
		/// </summary>
		/// <param name="position">The position.</param>
		Light(const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f));
		/// <summary>
		/// Finalizes an instance of the <see cref="Light"/> class.
		/// </summary>
		~Light();
		/// <summary>
		/// Attatches to the specified shader program.
		/// </summary>
		/// <param name="shaderID">The shader program.</param>
		void AttatchToShader(const GLuint & shaderID);
		/// <summary>
		/// Gets the ambient colour.
		/// </summary>
		/// <returns>the ambient colour</returns>
		glm::vec4 GetAmbient();
		/// <summary>
		/// Sets the ambient colour.
		/// </summary>
		/// <param name="colour">The colour.</param>
		void SetAmbient(const glm::vec4 & colour);
		/// <summary>
		/// Sets the ambient colour.
		/// </summary>
		/// <param name="r">The colours r component.</param>
		/// <param name="g">The colours g component.</param>
		/// <param name="b">The colours b component.</param>
		/// <param name="a">The colours a component.</param>
		void SetAmbient(const float & r, const float & g, const float & b, const float & a = 1.0f);
		/// <summary>
		/// Gets the diffuse colour.
		/// </summary>
		/// <returns>the diffuse colour</returns>
		glm::vec4 GetDiffuse();
		void SetDiffuse(const glm::vec4 & colour);
		/// <summary>
		/// Sets the diffuse colour.
		/// </summary>
		/// <param name="r">The colours r component.</param>
		/// <param name="g">The colours g component.</param>
		/// <param name="b">The colours b component.</param>
		/// <param name="a">The colours a component.</param>
		void SetDiffuse(const float & r, const float & g, const float & b, const float & a = 1.0f);
		/// <summary>
		/// Sets the specular colour.
		/// </summary>
		/// <param name="colour">The colour.</param>
		void SetSpecular(const glm::vec4 & colour);
		/// <summary>
		/// Sets the specular colour.
		/// </summary>
		/// <param name="r">The colours r component.</param>
		/// <param name="g">The colours g component.</param>
		/// <param name="b">The colours b component.</param>
		/// <param name="a">The colours a component.</param>
		void SetSpecular(const float & r, const float & g, const float & b, const float & a = 1.0f);
		/// <summary>
		/// Gets the specular colour.
		/// </summary>
		/// <returns>the specular colour</returns>
		glm::vec4 GetSpecular();
	private:
		rt3d::lightStruct l = {
			{0.4f, 0.4f, 0.4f, 1.0f}, // ambient
			{1.0f, 1.0f, 1.0f, 0.8f}, // diffuse
			{1.0f, 1.0f, 1.0f, 1.0f}, // specular
			{0.0f, 10.0f, 0.0f, 1.0f}  // position
		};
		GLuint shaderID;
	};
}
#endif // !B00289996_LIGHT_H
