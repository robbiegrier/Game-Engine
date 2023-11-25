#ifndef _TimerController
#define _TimerController

#include "AnimTimer.h"

namespace Azul
{
	class TimerController
	{
	public:
		TimerController(AnimTime Delta, AnimTime Min, AnimTime Max);
		TimerController() = delete;
		TimerController(const TimerController&) = delete;
		TimerController& operator = (const TimerController&) = delete;
		~TimerController() = default;

		void Update();

		void SetCurrTime(const AnimTime time);
		void SetDeltaTime(const AnimTime time);
		void SetMaxTime(const AnimTime time);
		void SetMinTime(const AnimTime time);

		AnimTime GetCurrTime() const;
		AnimTime GetDeltaTime() const;
		AnimTime GetMaxTime() const;
		AnimTime GetMinTime() const;

	private:
		AnimTime tCurr;
		AnimTime tDelta;
		AnimTime tMax;
		AnimTime tMin;
	};
}

#endif
