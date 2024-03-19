#include "TimerController.h"
#include "Engine.h"

namespace Azul
{
	TimerController::TimerController(AnimTime Delta, AnimTime Min, AnimTime Max)
		: tCurr(AnimTime::Duration::ZERO), tDelta(Delta), tMax(Max), tMin(Min)
	{
	}

	void TimerController::Update()
	{
		tCurr += tDelta * Engine::GetDeltaTime();

		// protection for time values for looping
		if (tCurr < tMin)
		{
			tCurr = tMax;
		}
		else if (tCurr > tMax)
		{
			tCurr = tMin;
		}
	}

	void TimerController::SetCurrTime(const AnimTime time)
	{
		tCurr = time;
	}

	AnimTime TimerController::GetCurrTime() const
	{
		return tCurr;
	}

	void TimerController::SetDeltaTime(const AnimTime time)
	{
		tDelta = time;
	}

	AnimTime TimerController::GetDeltaTime() const
	{
		return tDelta;
	}

	void TimerController::SetMaxTime(const AnimTime time)
	{
		tMax = time;
	}

	AnimTime TimerController::GetMaxTime() const
	{
		return tMax;
	}

	void TimerController::SetMinTime(const AnimTime time)
	{
		tMin = time;
	}

	AnimTime TimerController::GetMinTime() const
	{
		return tMin;
	}
}