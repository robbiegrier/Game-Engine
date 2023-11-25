#ifndef _AnimController
#define _AnimController

#include "TimerController.h"
#include "Clip.h"

namespace Azul
{
	class Anim;
	class GameObjectBasic;

	class AnimController
	{
	public:
		AnimController(AnimTime delta, Clip::Name pInClipName);
		AnimController(const AnimController&) = delete;
		AnimController& operator = (const AnimController&) = delete;
		~AnimController();

		void Update();
		GameObjectBasic* GetObjectPivot() const;

		Anim* GetAnimation() const { return pAnim; }
		TimerController* GetTimerController() const { return pTimerControl; }

		void SetClip(Clip::Name inClipName);

	private:
		TimerController* pTimerControl;
		Anim* pAnim;
	};
}

#endif
