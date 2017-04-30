#pragma once
#ifndef B00289996_GAME_H
#define B00289996_GAME_H

#include <memory>
#include <vector>
#include <glm\mat4x4.hpp>
#include "SDL.h"
#include "Camera.h"
#include "RenderTexture.h"

namespace B00289996 {
	class GameObject;
	class Light;
	/// <summary>
	/// Game
	/// </summary>
	class Game {
	public:
		/// <summary> Initializes a new instance of the <see cref="Game"/> class. </summary>
		Game();
		/// <summary> Finalizes an instance of the <see cref="Game"/> class. </summary>
		~Game();
		/// <summary> Updates the Game. </summary>
		void Update();
		/// <summary> Renders  the Game. </summary>
		void Render();
		/// <summary> Sets up a pyramid of barrels. </summary>
		void PlaceBarrels();
		/// <summary> Places coins around the world. </summary>
		void PlaceCoins();
		/// <summary> Places the buildings. </summary>
		void PlaceBuildings();
		/// <summary>
		/// Determines whether the Game is running.
		/// </summary>
		/// <returns> true if game is running</returns>
		const bool IsRunning() const;
	private:

		float miniMapScale = 0.0f;
		/// <summary> Initializes this instance. </summary>
		void Init();
		/// <summary> Sets up the render context. </summary>
		void SetupRC();
		/// <summary> The render texture </summary>
		std::shared_ptr<RenderTexture> renderTexture;
		const int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;
		SDL_Window * window;
		SDL_GLContext glContext;
		SDL_Event events;
		std::string gameName = "Unnamed Game";
		bool running, thirdPersonCam = false; // flads to determine if game is runnig & current camera mode
		std::shared_ptr<Camera> camera;	
		GLuint shaderID;
		std::shared_ptr<Light> light;
		bool paused = false;
		float accumulator = 0.0f; // acumulates frame times for use with physics system
		float frameTimer = 0.0f; // time taken to process last fram
		unsigned int FPS = 0, fpsCount = 0; // frames per second and variable to count it
	};
}
#endif // !B00289996_GAME_H
