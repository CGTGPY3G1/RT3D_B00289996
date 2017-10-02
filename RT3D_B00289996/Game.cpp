#include "Game.h"

#include "Input.h"
#include "RT3D\rt3d.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\string_cast.hpp>
#include <glm\vec2.hpp>
#include "GameObject.h"
#include "Transform.h"
#include "Time.h"
#include "AnimatedModel.h"
#include "IndexedModel.h"
#include "PlayerController.h"
#include "AutoRotate.h"
#include "PhysicsSystem.h"
#include "RigidBody.h"
#include <algorithm>
#include "SkyBox.h"
#include "GameObjectFactory.h"
#include "HUD.h"
#include "Random.h"
#include "ScoreBoard.h"
#include "Light.h"
namespace B00289996 {
	Game::Game() {
		SetupRC();
		// Required on Windows *only* init GLEW to access OpenGL beyond 1.1
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if(GLEW_OK != err) { // glewInit failed, something is seriously wrong
			std::cout << "glewInit failed, aborting." << std::endl;
			exit(1);
		}
		std::cout << glGetString(GL_VERSION) << std::endl;
		Init();
	}

	Game::~Game() {
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void Game::Update() {
		SDL_PumpEvents();
		Input::Update(); // esure input values are up to date
		Timer::Update(); // Update the frame time

		while(SDL_PollEvent(&events)) {
			if(events.type == SDL_QUIT) running = false;
		}
		if(!paused) { // if the Game Isn't paused
			const float deltaTime = Timer::GetDeltaTime();
			const float fixedDeltaTime = Timer::GetFixedDeltaTime();
			accumulator += deltaTime; // increment the physics time step accumulator by the frame time
			frameTimer += deltaTime; // increment the frame timer (for Frames Per Second calculation)
			// rotate the lights parent object
			std::shared_ptr<GameObject> lightStand = GameObjectFactory::GetInstance().GetGameObject("Light").lock();
			if(lightStand) lightStand->GetTransform().lock()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f * deltaTime);
			if(Input::GetKey(SDL_SCANCODE_R)) { // change the lights R diffuse value
				glm::vec4 diffuse = light->GetDiffuse();
				if(Input::GetKey(SDL_SCANCODE_LSHIFT)) { // decrease (if greater than 0)
					diffuse.x -= deltaTime;
					if(diffuse.x < 0.0f) diffuse.x = 0.0f;
				}
				else { // increase (if less than 1)
					diffuse.x += deltaTime;
					if(diffuse.x > 1.0f) diffuse.x = 1.0f;
				}
				light->SetDiffuse(diffuse);
			}
			if(Input::GetKey(SDL_SCANCODE_G)) { // change the lights G diffuse value
				glm::vec4 diffuse = light->GetDiffuse();
				if(Input::GetKey(SDL_SCANCODE_LSHIFT)) { // decrease (if greater than 0)
					diffuse.y -= deltaTime;
					if(diffuse.y < 0.0f) diffuse.y = 0.0f;
				}
				else { // increase (if less than 1)
					diffuse.y += deltaTime;
					if(diffuse.y > 1.0f) diffuse.y = 1.0f;
				}
				light->SetDiffuse(diffuse);
			}
			if(Input::GetKey(SDL_SCANCODE_B)) { // change the lights B diffuse value
				glm::vec4 diffuse = light->GetDiffuse();
				if(Input::GetKey(SDL_SCANCODE_LSHIFT)) { // decrease (if greater than 0)
					diffuse.z -= deltaTime;
					if(diffuse.z < 0.0f) diffuse.z = 0.0f;
				}
				else { // increase (if less than 1)
					diffuse.z += deltaTime;
					if(diffuse.z > 1.0f) diffuse.z = 1.0f;
				}
				light->SetDiffuse(diffuse);
			}

			// calculate average frame time
			while(frameTimer > 1.0f) { 
				FPS += fpsCount;
				FPS /= 2;
				frameTimer -= 1.0f;
				fpsCount = 0;
			}

			// while enough time has accumulated to process physics step
			while(accumulator >= fixedDeltaTime) {
				PhysicsSystem::GetInstance().Update(fixedDeltaTime); // step the physics simulation
				GameObjectFactory::GetInstance().FixedUpdate(fixedDeltaTime); // Update the GameObjects physics simulation
				if(thirdPersonCam) { // if camera following player
					// Get the cameras target from the GameObjectFactory
					std::shared_ptr<Transform> target = GameObjectFactory::GetInstance().GetGameObject("CamTarget").lock()->GetTransform().lock();
					// spherically interpolate the cameras orientation to face the target
					camera->GetTransform().lock()->SlerpRotation(camera->GetTransform().lock()->LookAt(target->GetPosition(), camera->GetTransform().lock()->GetPosition()), 5.0f * fixedDeltaTime);
					// spherically interpolate the cameras position to the target point
					glm::vec3 lerpTo = target->GetPosition() + (target->GetUp() * 4.0f) + (target->GetForward() * -15.0f);
					camera->GetTransform().lock()->LerpPosition(lerpTo, 2.0f * fixedDeltaTime);
				}
				accumulator -= fixedDeltaTime; // decrement the accumulator
			}
			if(!thirdPersonCam) { // if camera not following player
				// sets up uniform didplacement and rotation values
				const float displacement = 10.0f * deltaTime, rotation = 90.0f * deltaTime;
				// move and / or rotate the camera
				if(Input::GetKey(SDL_SCANCODE_W)) camera->GetTransform().lock()->Translate(camera->GetTransform().lock()->GetForward() * displacement);
				if(Input::GetKey(SDL_SCANCODE_S)) camera->GetTransform().lock()->Translate(camera->GetTransform().lock()->GetForward() * -displacement);
				if(Input::GetKey(SDL_SCANCODE_A)) camera->GetTransform().lock()->Translate(camera->GetTransform().lock()->GetRight() * displacement);
				if(Input::GetKey(SDL_SCANCODE_D)) camera->GetTransform().lock()->Translate(camera->GetTransform().lock()->GetRight() * -displacement);
				if(Input::GetKey(SDL_SCANCODE_Q)) camera->GetTransform().lock()->Translate(camera->GetTransform().lock()->GetUp() * -displacement);
				if(Input::GetKey(SDL_SCANCODE_E)) camera->GetTransform().lock()->Translate(camera->GetTransform().lock()->GetUp() * displacement);
				glm::vec2 mouseMovement = Input::GetMouseMovement();
				if(Input::GetMouseButton(1)) {
					if(mouseMovement.y != 0.0f) camera->GetTransform().lock()->Rotate(glm::vec3(mouseMovement.y > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f), rotation);
					if(mouseMovement.x != 0.0f) camera->GetTransform().lock()->Rotate(glm::vec3(0.0f, mouseMovement.x > 0.0f ? 1.0f : -1.0f, 0.0f), -rotation);
					camera->GetTransform().lock()->SetForward(camera->GetTransform().lock()->GetForward());
				}
				if(Input::GetMouseButton(3)) {
					if(mouseMovement.x != 0.0f) camera->GetTransform().lock()->Rotate(glm::vec3(0.0f, 0.0f, mouseMovement.x > 0.0f ? 1.0f : -1.0f), -rotation * 0.5f);
				}
				if(Input::GetKey(SDL_SCANCODE_KP_0)) camera->GetTransform().lock()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -rotation);
				if(Input::GetKey(SDL_SCANCODE_RCTRL)) camera->GetTransform().lock()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), rotation);
				if(Input::GetKey(SDL_SCANCODE_UP)) camera->GetTransform().lock()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), rotation);
				if(Input::GetKey(SDL_SCANCODE_DOWN)) camera->GetTransform().lock()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), -rotation);
				if(Input::GetKey(SDL_SCANCODE_RIGHT)) camera->GetTransform().lock()->Rotate(glm::vec3(0.0f, 0.0f, 1.0f), rotation);
				if(Input::GetKey(SDL_SCANCODE_LEFT)) camera->GetTransform().lock()->Rotate(glm::vec3(0.0f, 0.0f, 1.0f), -rotation);
				if(Input::GetKeyDown(SDL_SCANCODE_F)) { // fire globe
					std::shared_ptr<GameObject> globe = GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Globe, "Globe", camera->GetTransform().lock()->GetPosition() + camera->GetTransform().lock()->GetForward() * 2.0f).lock();
					globe->GetRigidBody().lock()->AddForce(camera->GetTransform().lock()->GetForward() * 800.0f, true);
				}
			}
			else { // add coin counter to the HUD
				unsigned int numberOfCoins = ScoreBoard::GetInstance().GetCoins();
				std::string coinsAsString = "Coins Collected = " + std::to_string(numberOfCoins);
				HUD::GetInstance().DrawText(coinsAsString, -0.95f, -0.7f, 0.02f * coinsAsString.size(), 0.1f, Alignment::Left);
			}
			GameObjectFactory::GetInstance().Update(deltaTime); // Update GameObjects using frame time step
			if(Input::GetKeyDown(SDL_SCANCODE_RETURN)) thirdPersonCam = !thirdPersonCam; // switch camera mode
		}
		if(Input::GetKeyDown(SDL_SCANCODE_P)) paused = !paused; // pause / unpause game
		if(Input::GetKey(SDL_SCANCODE_ESCAPE)) running = false; // quit
		if(Input::GetKeyDown(SDL_SCANCODE_I)) PhysicsSystem::GetInstance().SetDrawDebug(!PhysicsSystem::GetInstance().GetDrawDebug());

		// add FPS to HUD
		/*std::string fpsString = "FPS = " + std::to_string(FPS);
		HUD::GetInstance().DrawText(fpsString, -0.95f, -0.7f, 0.02f * fpsString.size(), 0.1f, Alignment::Left);*/
	}

	void Game::Render() {
		fpsCount++; // increment Frames per Second count
		if(thirdPersonCam) { // if camera following player
			// store the cameras position
			std::shared_ptr<Transform> camTransform = camera->GetTransform().lock();
			glm::vec3 camPos = camTransform->GetPosition();
			glm::quat camRotation = camTransform->GetRotation();
			std::shared_ptr<GameObject> player = GameObjectFactory::GetInstance().GetGameObject("Player").lock();
			if(player) { // if the player exists
				// place camera above player at height based on velocity and downward facing orientation
				std::shared_ptr<Transform> playerTransform = player->GetTransform().lock();
				glm::vec3 velocity = player->GetRigidBody().lock()->GetLinearVelocity();
				velocity.y = 0;
				miniMapScale = miniMapScale + ((glm::length(velocity) - miniMapScale) * Timer::GetDeltaTime());
				camTransform->SetPosition(playerTransform->GetPosition() + glm::vec3(0.0f, 20.f + 1.15f * miniMapScale, 0.0f));
				camTransform->SetForward(glm::vec3(0.0f, -1.0f, 0.0f), -playerTransform->GetForward());
			}
			renderTexture->Bind();  // bind the Frame Buffer Object
			// Render the Scene to the Frame Buffer
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			camera->UpdateProjectionUniform();
			light->AttatchToShader(shaderID);
			GameObjectFactory::GetInstance().Render(shaderID);
			renderTexture->Unbind();  // Unbind the Frame Buffer Object
			glDisable(GL_CULL_FACE);
			// reset the camera Position
			camTransform->SetPosition(camPos);
			camTransform->SetRotation(camRotation);
		}
		// Render the Scene (again)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SkyBox::GetInstance().Render();
		glEnable(GL_CULL_FACE);
		camera->UpdateProjectionUniform();
		light->AttatchToShader(shaderID);
		GameObjectFactory::GetInstance().Render(shaderID);
		PhysicsSystem::GetInstance().Draw();
		if(thirdPersonCam) {
			HUD::GetInstance().DrawTexture(renderTexture->GetTextureID(), 0.7f, 0.7f, 0.25f, 0.25f);
			HUD::GetInstance().Render();
		}
		glDisable(GL_CULL_FACE);
		rt3d::setActiveTexture(0);
		SDL_GL_SwapWindow(window); // swap buffers
	}

	void Game::PlaceBarrels() {
		glm::quat rotation; // used to randomize Y rotation
		const int levels = 9; // number of levels in pyramid
		// position start and offset values
		float offsetX = 0.5f, offsetZ = 0.5f, offsetY = 1.4f, startX, startZ, startY = 0.7f;
		for(int y = 0; y < levels; y++) { // foe each level
			offsetZ = y * 0.5f; 
			for(int z = y; z < levels; z++, offsetZ += 1.1f) { // foe each row
				offsetX = y * 0.5f;
				for(int x = y; x < levels; x++, offsetX += 1.1f) { // foe each column
					// rotate by random value around Y axis
					rotation = glm::rotate(rotation, glm::radians(Random::RandomFloat(360.0f)), glm::vec3(0.0f, 1.0f, 0.0f)); 
					// create a barrel at the calculated position
					GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Barrel, "Barrel", glm::vec3(offsetX, startY + offsetY * y, offsetZ), rotation); 
				}
			}
		}
	}

	void Game::PlaceCoins() {
		// place coins in groups
		float startLocation = -90.0f;
		float xLocation = startLocation, zLocation = startLocation;
		float offset = 5.0f;
		for(size_t i = 0; i < 3; i++, xLocation += offset) {
			zLocation = startLocation;
			for(size_t j = 0; j < 3; j++, zLocation += offset) {
				GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Coin, "Coin", glm::vec3(xLocation, 1.0f, zLocation));
			}
		}

		xLocation = -startLocation, zLocation = startLocation;
		for(size_t i = 0; i < 3; i++, xLocation -= offset) {
			zLocation = startLocation;
			for(size_t j = 0; j < 3; j++, zLocation += offset) {
				GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Coin, "Coin", glm::vec3(xLocation, 1.0f, zLocation));
			}
		}

		startLocation = -40.0f;
		xLocation = -startLocation, zLocation = -startLocation;
		offset = 5.0f;
		for(size_t i = 0; i < 5; i++, xLocation -= offset) {
			zLocation = startLocation;
			for(size_t j = 0; j < 5; j++, zLocation += offset) {
				GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Coin, "Coin", glm::vec3(xLocation, 1.0f, zLocation));
			}
		}
	}

	void Game::PlaceBuildings() {
		// place buildings around the game world
		GameObjectFactory::GetInstance().CreateBuilding(1, glm::vec3(60.0f, 0.3f, 60.0f));
		GameObjectFactory::GetInstance().CreateBuilding(2, glm::vec3(10.0f, 0.3f, 60.0f));
		GameObjectFactory::GetInstance().CreateBuilding(3, glm::vec3(-30.0f, 0.3f, 60.0f));
		GameObjectFactory::GetInstance().CreateBuilding(4, glm::vec3(-60.0f, 0.3f, 60.0f));

		GameObjectFactory::GetInstance().CreateBuilding(5, glm::vec3(-60.0f, 0.3f, -60.0f));
		GameObjectFactory::GetInstance().CreateBuilding(5, glm::vec3(-37.3f, 0.3f, -60.0f));
		GameObjectFactory::GetInstance().CreateBuilding(5, glm::vec3(-14.6f, 0.3f, -60.0f));

		GameObjectFactory::GetInstance().CreateBuilding(1, glm::vec3(30.0f, 0.3f, -60.0f));

		GameObjectFactory::GetInstance().CreateBuilding(3, glm::vec3(70.0f, 0.3f, 20.0f));
		GameObjectFactory::GetInstance().CreateBuilding(3, glm::vec3(70.0f, 0.3f, 0.0f));
		GameObjectFactory::GetInstance().CreateBuilding(3, glm::vec3(70.0f, 0.3f, -20.0f));

		GameObjectFactory::GetInstance().CreateBuilding(4, glm::vec3(-70.0f, 0.3f, 19.8f));
		GameObjectFactory::GetInstance().CreateBuilding(4, glm::vec3(-70.0f, 0.3f, 0.0f));
		GameObjectFactory::GetInstance().CreateBuilding(4, glm::vec3(-70.0f, 0.3f, -19.8f));
	}

	const bool Game::IsRunning() const {
		return running;
	}

	void Game::Init() {
		glEnable(GL_DEPTH_TEST); 
		glEnable(GL_BLEND);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(10);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glDepthMask(GL_TRUE);
		renderTexture = std::make_shared<RenderTexture>(SCREEN_WIDTH, SCREEN_HEIGHT);
		// create the camera and define it as the main camera
		camera = std::make_shared<Camera>(glm::vec3(-10.0f, 20.0f, 10.0f));
		camera->SetAsMain();
		// initialize the default shader program
		shaderID = rt3d::initShaders("RT3D\\Shaders\\phong-tex.vert", "RT3D\\Shaders\\phong-tex.frag");
		// create the light
		std::shared_ptr<GameObject> lightStand = GameObjectFactory::GetInstance().CreateGameObject("Light", glm::vec3(0.0f, 60.0f, 0.0f)).lock();
		light = std::make_shared<Light>(glm::vec3(-100.0f, 15.0f, 0.0f));
		light->GetTransform().lock()->SetParent(lightStand->GetTransform().lock());

		// create the player from 3 unique connected GameObejects
		// create the empty Gameobject that will act as a container for the Dynamic Character Controller
		std::shared_ptr<GameObject> modelStand = GameObjectFactory::GetInstance().CreateGameObject("Player", glm::vec3(3.0f, 3.0f, -10.0f)).lock(); 
		// create the scaled and reoriented Gameobject that will be rendered
		std::shared_ptr<GameObject> model3D = GameObjectFactory::GetInstance().CreateGameObject("Model").lock();
		model3D->GetTransform().lock()->Rotate(modelStand->GetTransform().lock()->GetRight(), -90.0f);
		model3D->GetTransform().lock()->Rotate(modelStand->GetTransform().lock()->GetForward(), -90.0f);
		model3D->GetTransform().lock()->Scale(0.05f);
		model3D->GetTransform().lock()->Translate(0.0f, 0.6f, 0.0f);
		model3D->GetTransform().lock()->SetParent(modelStand->GetTransform());
		// set up the objects model
		std::shared_ptr<AnimatedModel> model = std::make_shared<AnimatedModel>(AnimatedModelType::Human);
		model->SetAnimationSpeed(10.0f);
		model3D->SetModel(std::static_pointer_cast<Model>(model));
		// set up the Dynamic Character Controllers PlayerController component
		std::shared_ptr<PlayerController> p = std::make_shared<PlayerController>();
		p->SetAnimatedModel(model);
		modelStand->AddBehaviour(p);
		// and then its Character Controller component
		PhysicsSystem::GetInstance().AddCharacterController(modelStand, glm::vec3(0.0f, 0.75f, 0.0f), 1.5f, 0.7f, 50.0f);
		// add the camera target object in front of the player
		std::shared_ptr<GameObject> camTarget = GameObjectFactory::GetInstance().CreateGameObject("CamTarget", glm::vec3(0.0f, 1.0f, 6.0f)).lock();
		camTarget->GetTransform().lock()->SetParent(modelStand->GetTransform());

		// Set Up Static Scenery
		std::shared_ptr<GameObject> ground = GameObjectFactory::GetInstance().CreateGameObject("Ground", glm::vec3(0.0f, 0.25f, 0.0f), glm::quat()).lock();
		std::shared_ptr<IndexedModel> groundModel = std::make_shared<IndexedModel>("ground");
		groundModel->LoadTexture("pavement");
		ground->SetModel(std::static_pointer_cast<Model>(groundModel));
		PhysicsSystem::GetInstance().AddBox(ground, glm::vec3(0.0f, -0.25f, 0.0f), glm::vec3(200.0f, 0.5f, 200.0f), 0.0f);
		ground->SetTag("Ground");
		ground->GetRigidBody().lock()->SetCollisionGroup(btBroadphaseProxy::StaticFilter);

		GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Wall, "Wall", glm::vec3(0.0f, 0.0f, 100.0f)).lock();
		GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Wall, "Wall", glm::vec3(0.0f, 0.0f, -100.0f)).lock();
		GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Wall, "Wall", glm::vec3(100.0f, 0.0f, 0.0f), glm::rotate(glm::quat(), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))).lock();
		GameObjectFactory::GetInstance().CreatePrefab(PrefabObject::Wall, "Wall", glm::vec3(-100.0f, 0.0f, 0.0f), glm::rotate(glm::quat(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f))).lock();
		PlaceBuildings();

		// add dynamic and kinematic objects
		PlaceBarrels();
		PlaceCoins();
		running = true;
	}

	void Game::SetupRC() {

		if(SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
			rt3d::exitFatalError("Unable to initialize SDL");
		// Request an OpenGL 3.0 context.
		// Not able to use SDL to choose profile (yet), should default to core profile on 3.2 or later
		// If you request a context not supported by your drivers, no OpenGL context will be created

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		// Create 800x600 window
		window = SDL_CreateWindow(gameName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if(!window) // Check window was created OK
			rt3d::exitFatalError("Unable to create window");

		glContext = SDL_GL_CreateContext(window); // Create opengl context and attach to window
		SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	}
}