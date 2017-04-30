#pragma once
#ifndef B00289996_SKYBOX_H
#define B00289996_SKYBOX_H
#include <memory>
#include <GL/glew.h>
class IndexedModel;
namespace B00289996 {
	/// <summary>
	/// A Skybox
	/// </summary>
	class SkyBox {
	public:
		/// <summary>
		/// Gets the singleton instance.
		/// </summary>
		/// <returns>the singleton instance</returns>
		static SkyBox & GetInstance();
		/// <summary>
		/// Finalizes an instance of the <see cref="SkyBox"/> class.
		/// </summary>
		~SkyBox();
		/// <summary>
		/// Renders the Skybox.
		/// </summary>
		void Render();
	private:
		/// <summary>
		/// Prevents a default instance of the <see cref="SkyBox"/> class from being created.
		/// </summary>
		SkyBox();
		GLuint modelID, indexCount, vertexCount, shaderID;
		GLuint textures[6];
	};
}
#endif // !B00289996_SKYBOX_H