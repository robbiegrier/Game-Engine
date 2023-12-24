#ifndef _Clip
#define _Clip

#include "DLink.h"
#include "AnimTime.h"
#include "Bone.h"

namespace Azul
{
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
			Bone* poBone;
			char pad[4];
		};

	public:
		Clip(int inNumBones, Name inName);
		Clip();
		Clip(const Clip&) = delete;
		Clip& operator = (const Clip&) = delete;
		virtual ~Clip();

		AnimTime GetTotalTime();
		void AnimateBones(AnimTime tCurr, Bone* pResult);

		// As a DLink node
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		void SetName(Clip::Name inName);
		Clip::Name GetName() const;
		const char* NameToString();

	private:
		void privSetAnimationData();
		void privSetAnimationDataWalk();
		void privSetAnimationDataSidestepRight();
		void privSetAnimationDataHitFront();
		void privSetAnimationDataRun();
		void privSetAnimationDataShotUp();

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
