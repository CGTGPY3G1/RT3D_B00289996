#pragma once
#ifndef B00289996_RENDER_TEXTURE_H
#define B00289996_RENDER_TEXTURE_H
#include <GL/glew.h>
namespace B00289996 {
	class RenderTexture {
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RenderTexture"/> class with the specified width and height.
		/// </summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		RenderTexture(const unsigned int & width, const unsigned int & height);
		~RenderTexture();
		/// <summary>
		/// Binds the FBO.
		/// </summary>
		void Bind();
		/// <summary>
		/// Unbinds the FBO.
		/// </summary>
		void Unbind();
		/// <summary>
		/// Gets the FBO.
		/// </summary>
		/// <returns>the FBO</returns>
		const GLuint GetFBO() const;
		/// <summary>
		/// Gets the TextureID.
		/// </summary>
		/// <returns>the TextureID</returns>
		const GLuint GetTextureID() const;
		/// <summary>
		/// Gets the depth buffer ID.
		/// </summary>
		/// <returns>the depth buffer ID</returns>
		const GLuint GetDepthBuffer() const;
	private:
		/// <summary>
		/// Generates the Frame Buffer and attachments.
		/// </summary>
		void Generate();
		GLuint FBO = 0, textureID = 0, depthBuffer = 0;
		unsigned int w, h;
	};
}

#endif // !B00289996_RENDER_TEXTURE_H
