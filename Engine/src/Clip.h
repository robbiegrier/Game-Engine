#ifndef _Clip
#define _Clip

#include "DLink.h"
#include "AnimTime.h"
#include "BoneTransform.h"

#define BONE_COUNT_MAX 128
#define HIERARCHY_DEPTH_MAX 20

namespace Azul
{
	struct BlendInput
	{
		BoneTransform* pA;
		BoneTransform* pB;
		float tS;
	};

	class Clip : public DLink
	{
	public:
		enum class Name
		{
			Walk,
			Run,
			SidestepRight,
			HitFront,
			ShotUp,
			JoyfulJump,
			HipHopDancing,
			GangnamStyle,
			HumanoidRun,
			GettingHitBackwards,
			RunJump,
			PaladinWalk,
			KnightWalk,
			Null,
			None
		};

		struct FrameBucket
		{
			~FrameBucket()
			{
				delete[] poBone;
			}

			FrameBucket* nextBucket;
			FrameBucket* prevBucket;
			AnimTime KeyTime;
			BoneTransform* poBone;
			char pad[4];
		};

	public:
		Clip();
		Clip(const Clip&) = delete;
		Clip& operator = (const Clip&) = delete;
		virtual ~Clip();

		AnimTime GetTotalTime();

		BlendInput GetBlendInput(AnimTime tCurr) const;

		// As a DLink node
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		void SetName(Clip::Name inName);
		Clip::Name GetName() const;
		const char* NameToString();

	protected:
		AnimTime privFindMaxTime();
		int  privFindNumFrames();

		int numBones;
		int numFrames;
		AnimTime totalTime;
		FrameBucket* pHead;
		Name name;
	};
}

#endif
