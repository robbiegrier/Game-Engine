#include "AnimController.h"
#include "Anim.h"
#include "Skeleton.h"
#include "GameObjectBasic.h"
#include "ClipManager.h"
namespace Azul
{
	AnimController::AnimController(AnimTime delta, Clip::Name pInClipName)
	{
		pAnim = new Anim(ClipManager::Find(pInClipName));
		pTimerControl = new TimerController(delta, AnimTime(AnimTime::Duration::ZERO), pAnim->FindMaxTime());
	}

	AnimController::~AnimController()
	{
		delete pAnim;
		delete pTimerControl;
	}

	void AnimController::Update()
	{
		pTimerControl->Update();
		pAnim->Animate(pTimerControl->GetCurrTime());
	}

	GameObjectBasic* AnimController::GetObjectPivot() const
	{
		return pAnim->GetSkeleton()->GetPivot();
	}

	void AnimController::SetClip(Clip::Name inClipName)
	{
		GetAnimation()->SetClip(inClipName);
		pTimerControl->SetMaxTime(pAnim->FindMaxTime());
	}
}