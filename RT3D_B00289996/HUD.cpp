#include "HUD.h"
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
namespace B00289996 {
	HUD::HUD() {
		// ensure TTF is only initialised once, (irrelevant due to use of the singleton pattern)
		static bool ttfInitialized = false;
		if(!ttfInitialized) { 
			if(TTF_Init() == -1) std::cout << "TTF failed to initialise." << std::endl;
			else ttfInitialized = true;
		}
		LoadFont("arial.ttf");
		model = std::make_shared<IndexedModel>("quad");  // make a new IndexedModel with a Quad mesh representation
		shaderID = rt3d::initShaders("RT3D\\Shaders\\textured.vert", "RT3D\\Shaders\\textured.frag");
	}

	HUD & HUD::GetInstance() {
		static HUD instance;
		return instance;
	}

	HUD::~HUD() {

	}

	void HUD::LoadFont(const char * fontName) {
		font = TTF_OpenFont(fontName, 24);
		if(font == NULL) std::cout << "Failed to open font." << std::endl;
	}

	void HUD::DrawText(const std::string & toDraw, const float & x, const float & y, const float & width, const float & height, const Alignment & alignment) {
		glm::mat4 transform(1.0f);
		// set the text position relative to the specified alignment
		transform = glm::translate(transform, alignment == Alignment::Centered ? glm::vec3(x, y, 0.0f) : alignment == Alignment::Left ? glm::vec3(x + width, y, 0.0f) : glm::vec3(x - width, y, 0.0f));
		// scale the text to the specified width and height
		transform = glm::scale(transform, glm::vec3(width, height, 0.0f));

		if(currentTextIndex >= textObjects.size()) {// if there is no more room in the array
			// create the new text
			HUDItem text;
			text.id = textToTexture(toDraw.c_str(), text.id);
			text.transform = transform;
			textObjects.push_back(text);
		}
		else {
			// update the old text
			textObjects[currentTextIndex].id = textToTexture(toDraw.c_str(), textObjects[currentTextIndex].id);
			textObjects[currentTextIndex].transform = transform;
		}
		currentTextIndex++;
	}

	void HUD::DrawTexture(const GLuint & toDraw, const float & x, const float & y, const float & width, const float & height, const Alignment & alignment) {
		glm::mat4 transform(1.0f);
		// set the textures position relative to the specified alignment
		transform = glm::translate(transform, alignment == Alignment::Centered ? glm::vec3(x, y, 0.0f) : alignment == Alignment::Left ? glm::vec3(x + width * 0.5f, y, 0.0f) : glm::vec3(x - width * 0.5f, y, 0.0f));
		// scale the texture to the specified width and height
		transform = glm::scale(transform, glm::vec3(-width, height, 0.0f));

		if(currentTextureIndex >= textureObjects.size()) { // if there is no more room in the array
			// create the new texture
			HUDItem texture;
			texture.id = toDraw;
			texture.transform = transform;
			textureObjects.push_back(texture);
		}
		else {
			// update the old texture
			textureObjects[currentTextureIndex].id = toDraw;
			textureObjects[currentTextureIndex].transform = transform;
		}
		currentTextureIndex++;
	}

	void HUD::Render() {
		glUseProgram(shaderID);
		glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		rt3d::setUniformMatrix4fv(shaderID, "projection", glm::value_ptr(glm::mat4(1.0f)));
		for(unsigned int i = 0; i < currentTextIndex; i++) {
			model->SetTexture(textObjects[i].id);
			model->Render(textObjects[i].transform, shaderID);
		}
		currentTextIndex = 0;
		for(unsigned int i = 0; i < currentTextureIndex; i++) {
			model->SetTexture(textureObjects[i].id);
			model->Render(textureObjects[i].transform, shaderID);
		}
		currentTextureIndex = 0;
		glEnable(GL_DEPTH_TEST);
	}

	GLuint HUD::textToTexture(const char * str, GLuint textID) {
		GLuint texture = textID;
		TTF_Font * font = this->font;

		SDL_Surface * stringImage = TTF_RenderText_Blended(font, str, {255, 255, 255});

		if(stringImage == NULL) {
			std::cout << "String surface not created." << std::endl;
		}

		if(texture == 0) {
			glGenTextures(1, &texture);//This avoids memory leakage, only initialise //first time
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, stringImage->w, stringImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, stringImage->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);

		SDL_FreeSurface(stringImage);
		return texture;
	}
}
