#ifndef _Player
#define _Player

#include "GameObject.h"

namespace Azul
{
	class PlayerScript;

	// Temporary player class to encapsulate demo logic
	class Player : public GameObject
	{
	public:
		Player();
		virtual ~Player();

		virtual void Start() override;
		virtual void Tick(float deltaTime) override;
		virtual void Draw() override;

	private:
		PlayerScript* pScript = nullptr;
	};
}

#endif