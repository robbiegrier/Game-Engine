#ifndef _Player
#define _Player

#include "GameObject.h"

namespace Azul
{
	// Temporary player class to encapsulate demo logic
	class Player : public GameObject
	{
	public:
		Player();
		~Player() = default;

		virtual void Tick(AnimTime deltaTime) override;

	private:
		void CheckSwitchCameraInput();
		void CheckMovement(float deltaTime);
		void CheckLookAt(float deltaTime);
		void ToggleLookAtMode();
		void ToggleFramerateMode();
		void ProcessGravity(float deltaTime);

		bool useMouseLook = false;
		POINT prevPoint;
		bool jumping = false;
		bool sprinting = false;
		float jumpImpulse = 0.f;

		bool walkMode = false;
	};
}

#endif