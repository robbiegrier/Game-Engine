#ifndef _PlayerScriptFps
#define _PlayerScriptFps

#include "PlayerScript.h"
#include "MathEngine.h"

namespace Azul
{
	class Camera;

	class PlayerScriptFps : public PlayerScript
	{
	public:
		PlayerScriptFps() = default;
		PlayerScriptFps(const PlayerScriptFps&) = delete;
		PlayerScriptFps& operator=(const PlayerScriptFps&) = delete;
		virtual ~PlayerScriptFps() = default;

		virtual void Start();
		virtual void Update(float deltaTime);

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
