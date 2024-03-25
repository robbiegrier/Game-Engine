#include "Clip.h"
#include "AnimTime.h"
#include "BoneTransform.h"
#include "Mixer.h"
#include "AnimationSystem.h"

namespace Azul
{
	Clip::Clip()
	{
	}

	Clip::~Clip()
	{
		Wash();
	}

	BlendInput Clip::GetBlendInput(AnimTime tCurr) const
	{
		FrameBucket* pNextBucket = pHead;

		while (tCurr >= pNextBucket->KeyTime && pNextBucket->nextBucket != nullptr)
		{
			pNextBucket = pNextBucket->nextBucket;
		}

		BlendInput bi;
		bi.pA = pNextBucket->prevBucket->poBone;
		bi.pB = pNextBucket->poBone;
		bi.tS = (tCurr - pNextBucket->prevBucket->KeyTime) / (pNextBucket->KeyTime - pNextBucket->prevBucket->KeyTime);

		return bi;
	}

	void Clip::Wash()
	{
		FrameBucket* pTmp = pHead;

		while (pTmp != nullptr)
		{
			FrameBucket* pDeleteMe = pTmp;
			pTmp = pTmp->nextBucket;
			delete pDeleteMe;
		}

		pHead = nullptr;
		numFrames = 0;
		numBones = 0;
		totalTime = AnimTime();
		name = Name::None;
	}

	bool Clip::Compare(DLink* pTargetNode)
	{
		return name == ((Clip*)pTargetNode)->name;
	}

	void Clip::Dump()
	{
		Trace::out("Animation Clip: %s\n", NameToString());
	}

	void Clip::SetName(Clip::Name inName)
	{
		name = inName;
	}

	Clip::Name Clip::GetName() const
	{
		return name;
	}

	const char* Clip::NameToString()
	{
		switch (name)
		{
		case Azul::Clip::Name::Walk:
			return "Walk";
		case Azul::Clip::Name::Run:
			return "Run";
		case Azul::Clip::Name::SidestepRight:
			return "SidestepRight";
		case Azul::Clip::Name::HitFront:
			return "HitFront";
		case Azul::Clip::Name::ShotUp:
			return "ShotUp";
		case Azul::Clip::Name::Null:
			return "Null";
		case Azul::Clip::Name::None:
			return "None";
		default:
			break;
		}

		return "None";
	}

	int Clip::privFindNumFrames()
	{
		int count = 0;
		FrameBucket* pTmp = pHead;

		while (pTmp->nextBucket != nullptr)
		{
			count++;
			pTmp = pTmp->nextBucket;
		}
		return count;
	}

	AnimTime Clip::privFindMaxTime()
	{
		AnimTime tMax;
		FrameBucket* pTmp = pHead;

		while (pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;

		return tMax;
	}

	AnimTime Clip::GetTotalTime()
	{
		return totalTime;
	}
}