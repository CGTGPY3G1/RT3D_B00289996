#include "PlayerController.h"
#include "GameObject.h"
#include "AnimatedModel.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Input.h"
#include "PhysicsSystem.h"
#include <string>
#include "Collision.h"
#include "ScoreBoard.h"
#include "AudioManager.h"
namespace B00289996 {
	PlayerController::PlayerController() {

	}

	PlayerController::~PlayerController() {
	}

	void PlayerController::FixedUpdate(const float & fixedDeltaTime) {
		if(IsAttached()) {
			glm::vec3 rayVec = glm::vec3(0.0f, 0.775f, 0.0f);
			std::shared_ptr<Transform> t = gameObject.lock()->GetTransform().lock();
			const glm::vec3 position = t->GetPosition();
			RayCastHit hit = PhysicsSystem::GetInstance().Raycast(position, position - rayVec, btBroadphaseProxy::StaticFilter);
			if(hit.hit) {
				SetGrounded(true);
				glm::vec3 newPosition = position;
				newPosition.y = (hit.point + rayVec).y - 0.05f;
				std::shared_ptr<RigidBody> rb = gameObject.lock()->GetRigidBody().lock();
				glm::vec3 velocity = rb->GetLinearVelocity();
				if(velocity.y < 0.0f) velocity.y = 0.0f;
				rb->SetLinearVelocity(velocity);
				rb->SetPosition(newPosition);
			}
			else SetGrounded(false);

			float displacement = 30.0f * fixedDeltaTime, rotation = 90.0f * fixedDeltaTime;
			if(Input::GetKey(SDL_SCANCODE_LSHIFT)) displacement = displacement * 2.0f;
			bool moved = false, rotated = false, jumped = false;
			glm::vec3 toRotate = glm::vec3(0.0f, 0.0f, 0.0f);
			int selectedAnim = 0;
			if(grounded) {
				if(Input::GetKey(SDL_SCANCODE_W)) {
					std::shared_ptr<Transform> t = gameObject.lock()->GetTransform().lock();
					std::shared_ptr<RigidBody> rb = gameObject.lock()->GetRigidBody().lock();
					rb->AddForce(t->GetForward() * rb->GetMass() * displacement, true);
					moved = true;
					selectedAnim = 1;
				}
				if(Input::GetKey(SDL_SCANCODE_S)) {
					std::shared_ptr<Transform> t = gameObject.lock()->GetTransform().lock();
					std::shared_ptr<RigidBody> rb = gameObject.lock()->GetRigidBody().lock();
					rb->AddForce(t->GetForward() * rb->GetMass() * -displacement * 0.5f, true);
					moved = true;
					selectedAnim = 1;
				}
				if(Input::GetKey(SDL_SCANCODE_A)) {
					std::shared_ptr<Transform> t = gameObject.lock()->GetTransform().lock();
					std::shared_ptr<RigidBody> rb = gameObject.lock()->GetRigidBody().lock();
					toRotate += glm::vec3(0.0f, 1.0f, 0.0f) * rotation;
					moved = true;
					rotated = true;
					selectedAnim = 1;
				}
				if(Input::GetKey(SDL_SCANCODE_D)) {
					std::shared_ptr<Transform> t = gameObject.lock()->GetTransform().lock();
					std::shared_ptr<RigidBody> rb = gameObject.lock()->GetRigidBody().lock();
					toRotate += glm::vec3(0.0f, 1.0f, 0.0f) * -rotation;
					moved = true;
					rotated = true;
					selectedAnim = 1;
				}
				if(Input::GetKey(SDL_SCANCODE_SPACE)) {
					std::shared_ptr<RigidBody> rb = gameObject.lock()->GetRigidBody().lock();
					rb->AddForce(glm::vec3(0.0f, 150.0f, 0.0f), true);
					moved = true;
					jumped = true;
					selectedAnim = 6;
				}
			}
			if(moved) {
				if(selectedAnim != currentAnim) {
					currentAnim = selectedAnim;
					std::shared_ptr<AnimatedModel> m = model.lock();
					if(m) m->SetCurrentAnimation(currentAnim);
					if(currentAnim == 6) m->SetAnimationSpeed(2.0f);
					else m->SetAnimationSpeed(10.0f);
				}
				if(rotated) {
					std::shared_ptr<RigidBody> rb = gameObject.lock()->GetRigidBody().lock();
					rb->SetAngularVelocity(toRotate);
				}
			}
			else if(grounded && currentAnim != 0) {
				currentAnim = 0;
				std::shared_ptr<AnimatedModel> m = model.lock();
				if(m) m->SetCurrentAnimation(currentAnim);
				m->SetAnimationSpeed(10.0f);
			}
		}

	}

	void PlayerController::SetAnimatedModel(const std::shared_ptr<AnimatedModel>& model) {
		this->model = model;
	}

	void PlayerController::OnCollisionEnter(const Collision & collision) {
		if(collision.gameObject.use_count() > 0) {
			std::shared_ptr<GameObject> g = collision.gameObject.lock();
			if(g && g->GetTag().compare("Coin") == 0) {
				AudioManager::GetInstance().PlayClip("coin.wav");
				ScoreBoard::GetInstance().AddCoin();
				g->Destroy();
			}
		}
	}

	void PlayerController::OnCollisionExit(const Collision & collision) {

	}


	void PlayerController::SetGrounded(const bool & setGrounded) {
		if(grounded != setGrounded) {
			grounded = setGrounded;
			std::shared_ptr<RigidBody> rb = gameObject.lock()->GetRigidBody().lock();
			rb->SetLocalGravity(grounded ? glm::vec3(0.0f, 0.0f, 0.0f) : PhysicsSystem::GetInstance().GetGravity()); // if grounded disable gravity, else set to world gravity
		}
	}
}