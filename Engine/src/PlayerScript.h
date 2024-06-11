#ifndef _PlayerScript
#define _PlayerScript

#include "Player.h"

namespace Azul
{
	class PlayerScript
	{
	public:
		PlayerScript() = default;
		PlayerScript(const PlayerScript&) = delete;
		PlayerScript& operator=(const PlayerScript&) = delete;
		virtual ~PlayerScript() = default;

		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void Draw();

		void SetPlayer(Player* pInPlayer) { pPlayer = pInPlayer; }
		Player* GetPlayer() const { return pPlayer; }

	private:
		Player* pPlayer;
	};
}

#endif