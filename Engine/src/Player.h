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
		virtual ~Player();

		virtual void Start() override;

		virtual void Tick(float deltaTime) override;

	private:
		void CheckMovement(float deltaTime);
		void CheckLookAt(float deltaTime);
		void ProcessGravity(float deltaTime);

		POINT prevPoint;
		bool jumping = false;
		bool sprinting = false;
		float jumpImpulse = 0.f;

		Camera* pPlayerCamera;
	};
}

#endif