#ifndef _EngineStatePlay
#define _EngineStatePlay

#include "EngineState.h"

namespace Azul
{
	class EngineStatePlay : public EngineState
	{
	public:
		EngineStatePlay() = default;
		EngineStatePlay(const EngineStatePlay&) = delete;
		EngineStatePlay& operator=(const EngineStatePlay&) = delete;
		virtual ~EngineStatePlay() = default;

		virtual void Update(float deltaTime) override;
		virtual void Draw() override;

		virtual void Enter();
		virtual void Exit();

		virtual void TransitionToEditor() override;
	};
}

#endif