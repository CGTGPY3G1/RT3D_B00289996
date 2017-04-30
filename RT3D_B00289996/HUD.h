#pragma once

#ifndef B00289996_HUD_H
#define B00289996_HUD_H
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <vector>
#include "IndexedModel.h"
namespace B00289996 {
	/// <summary>
	/// Horizontal allignment
	/// </summary>
	enum Alignment {
		Centered,
		Left,
		Right
	};

	/// <summary>
	/// texture id and transform of item to be drawn
	/// </summary>
	struct HUDItem {
		GLuint id = 0;
		glm::mat4 transform = glm::mat4(1.0f);
	};

	/// <summary>
	/// the Heads Up Display
	/// </summary>
	class HUD {
	public:
		/// <summary>
		/// Gets the Singleton instance.
		/// </summary>
		/// <returns>the Singleton instance</returns>
		static HUD & GetInstance();
		/// <summary>
		/// Finalizes an instance of the <see cref="HUD"/> class.
		/// </summary>
		~HUD();
		/// <summary>
		/// Loads the font.
		/// </summary>
		/// <param name="fontName">Name of the font.</param>
		void LoadFont(const char * fontName);
		/// <summary>
		/// Draws text to the screen.
		/// </summary>
		/// <param name="toDraw">To text to draw.</param>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position.</param>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="alignment">The alignment.</param>
		void DrawText(const std::string & toDraw, const float & x, const float & y, const float & width, const float & height, const Alignment & alignment = Alignment::Centered);
		/// <summary>
		/// Draws the texture to the screen.
		/// </summary>
		/// <param name="toDraw">To texture to draw.</param>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position.</param>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="alignment">The alignment.</param>
		void DrawTexture(const GLuint & toDraw, const float & x, const float & y, const float & width, const float & height, const Alignment & alignment = Alignment::Centered);
		/// <summary>
		/// Renders the HUD.
		/// </summary>
		void Render();

	private:
		/// <summary>
		/// Prevents a default instance of the <see cref="HUD"/> class from being created.
		/// </summary>
		HUD();
		/// <summary>
		/// Converts text to a texture.
		/// </summary>
		/// <param name="str">The string.</param>
		/// <param name="textID">The texture identifier.</param>
		/// <returns></returns>
		GLuint textToTexture(const char * str, GLuint textID);
		TTF_Font* font; // the font
		std::vector<HUDItem> textObjects, textureObjects; 
		unsigned int currentTextIndex, currentTextureIndex; // used to track how many items are to be drawn
		std::shared_ptr<IndexedModel> model;
		GLuint shaderID = 0;
	};
}
#endif // !B00289996_HUD_H



