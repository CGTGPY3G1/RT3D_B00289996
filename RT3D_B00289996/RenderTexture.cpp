#include "RenderTexture.h"

namespace B00289996 {
	RenderTexture::RenderTexture(const unsigned int & width, const unsigned int & height) : w(width), h(height) {
		Generate();
	}

	RenderTexture::~RenderTexture() {
	}

	void RenderTexture::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}

	void RenderTexture::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	inline void RenderTexture::Generate() {
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// create an empty texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// create a depth buffer
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		// attach the texture with a colour attachment
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);
		GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, buffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind the buffer
	}

	const GLuint RenderTexture::GetFBO() const {
		return FBO;
	}

	const GLuint RenderTexture::GetTextureID() const {
		return textureID;
	}

	const GLuint RenderTexture::GetDepthBuffer() const {
		return depthBuffer;
	}
}