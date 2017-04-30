#pragma once
#ifndef B00289996_CAMERA_H
#define B00289996_CAMERA_H
#include <memory>
#include <string>
#include "Moveable.h"
#include "RT3D\rt3d.h"

namespace B00289996 {
	/// <summary>
	/// Camera
	/// </summary>
	/// <seealso cref="Moveable" />
	/// <seealso cref="std::enable_shared_from_this{Camera}" />
	class Camera : public Moveable, public std::enable_shared_from_this<Camera> {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Camera"/> class.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="rotation">The rotation.</param>
		Camera(const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat & rotation = glm::quat());
		~Camera();
		/// <summary>
		/// Sets the projection matrix.
		/// </summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="near">The near clipping plane.</param>
		/// <param name="">The far clipping plane.</param>
		/// <param name="fov">The field of view.</param>
		void SetProjection(const float & width, const float & height, const float & near, const float & far, const float & fov);
		/// <summary> Updates the default shaders projection uniform.  </summary>
		void UpdateProjectionUniform();
		/// <summary>
		/// Updates the provided shaders projection uniform.
		/// </summary>
		/// <param name="shaderProgram">The shader program.</param>
		/// <param name="useCamTransform">combine the cameras view matrix.</param>
		/// <param name="uniformName">Name of the uniform.</param>
		void UpdateProjectionUniform(const GLuint shaderProgram, const bool & useCamTransform = false, const char * uniformName = "projection");
		/// <summary> Sets this instance of camera as Camera::main. </summary>
		void SetAsMain();
		/// <summary>
		/// Gets the camera set as Camera::main.
		/// </summary>
		/// <returns>The Main Camera</returns>
		static std::weak_ptr<Camera> GetMainCamera();
		bool operator == (const Camera & other);
		bool operator == (const std::shared_ptr<Camera> & other);
		/// <summary>
		/// Gets the view matrix.
		/// </summary>
		/// <returns>the view matrix</returns>
		glm::mat4 GetView();
		/// <summary>
		/// Gets the projection matrix.
		/// </summary>
		/// <returns>the projection matrix</returns>
		glm::mat4 GetProjection();
		/// <summary>
		/// Gets the view projection matrix.
		/// </summary>
		/// <returns>the view projection matrix</returns>
		glm::mat4 GetViewProjection();
	private:
		// the default shader names
		const std::string frag = "phong-tex", vert = "phong-tex";
		GLuint shaderID;
		glm::mat4 projection, viewProjection;
		unsigned int camId; // unique id for camera
		static std::shared_ptr<Camera> mainCamera; // static reference to the main camera
		// projection values
		float width = 1280, height = 720, near = 0.1f, far = 300.0f, fov = 45.0f;
	};
}
#endif // !B00289996_CAMERA_H
