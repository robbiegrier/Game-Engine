#ifndef _EngineState
#define _EngineState

namespace Azul
{
	class EngineState
	{
	public:
		EngineState() = default;
		EngineState(const EngineState&) = delete;
		EngineState& operator=(const EngineState&) = delete;
		virtual ~EngineState() = default;

		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;

		virtual void Enter() {}
		virtual void Exit() {}

		virtual void TransitionToEditor() {}
		virtual void TransitionToPlay() {}
	};
}

#endif
