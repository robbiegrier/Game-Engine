#ifndef _PlayerScriptAdventure
#define _PlayerScriptAdventure

#include "PlayerScript.h"
#include "MathEngine.h"

namespace Azul
{
	class Camera;

	class PlayerScriptAdventure : public PlayerScript
	{
	public:
		PlayerScriptAdventure();
		PlayerScriptAdventure(const PlayerScriptAdventure&) = delete;
		PlayerScriptAdventure& operator=(const PlayerScriptAdventure&) = delete;
		virtual ~PlayerScriptAdventure();

		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void Draw();

	private:
		void CheckMovement(float deltaTime);
		void CheckRotate(float deltaTime);

		POINT prevPoint;
		bool jumping = false;
		bool sprinting = false;
		float jumpImpulse = 0.f;
		Camera* pPlayerCamera;

		Vec3 startViewOffset;

		float scrollImpulse = 0.f;

		Vec3 moveImpulse;

		bool rotOngoing = false;
		Vec3 pivot;

		GraphicsObject* pGoMoveIndicator = nullptr;
		GraphicsObject* pGoCharacterIndicator = nullptr;
		bool drawMoveIndicator = false;

		GameObject* pCharacter;
	};
}

#endif