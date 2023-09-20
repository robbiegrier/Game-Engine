#ifndef _Game
#define _Game

#include "Engine.h"
#include "MathEngine.h"

namespace Azul
{
	// The Game implements the core setup, runtime, and teardown methods
	// which are invoked in the correct order by the Engine base class.
	class Game : public Engine
	{
	public:
		// Singleton start and stop
		static int WINAPI Launch(HINSTANCE pInstance, int cmdShow);
		static void Teardown();

	protected:
		// Big four
		Game();
		Game(const Game&) = delete;
		Game& operator = (const Game&) = delete;
		virtual ~Game();

		// Game/Engine contract
		virtual bool LoadContent() override;
		virtual void UnloadContent() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual	void ClearDepthStencilBuffer() override;

	private:
		// Demo methods
		void LoadFloor(int size);
		void LoadClock(float size, const Vec3& pos);
		void LoadPylon(float size, const Vec3& pos);
		void LoadColorObjects();
		void LoadLightObjects();
		void LoadMovingObjects();
		void LoadInstancedObjects();
		void UpdateDemo(float deltaTime);

		// Singleton
		static Game& GetInstance();
		static Game* pInstance;
	};
}

#endif