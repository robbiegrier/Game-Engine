#ifndef _EngineStateEditor
#define _EngineStateEditor

#include "EngineState.h"

namespace Azul
{
	class EngineStateEditor : public EngineState
	{
	public:
		EngineStateEditor() = default;
		EngineStateEditor(const EngineStateEditor&) = delete;
		EngineStateEditor& operator=(const EngineStateEditor&) = delete;
		virtual ~EngineStateEditor() = default;

		virtual void Update(float deltaTime) override;
		virtual void Draw() override;

		virtual void Enter();
		virtual void Exit();

		virtual void TransitionToPlay();
	};
}

#endif