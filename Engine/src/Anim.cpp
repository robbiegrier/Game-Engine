#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "Clip.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "Skeleton.h"
#include "ClipManager.h"

namespace Azul
{
	Anim::Anim(Clip* pInClip)
		: pClip(nullptr), pBoneResult(nullptr), pSkeleton(nullptr), numBones(NUM_BONES)
	{
		pBoneResult = new Bone[(unsigned int)numBones]();
		pSkeleton = new Skeleton(pBoneResult, numBones, "hit_front.skeleton.proto.azul");
		pClip = pInClip;
	}

	Anim::~Anim()
	{
		delete[] pBoneResult;
		delete pSkeleton;
	}

	AnimTime Anim::FindMaxTime()
	{
		return pClip->GetTotalTime();
	}

	void Anim::Animate(AnimTime tCurr)
	{
		pClip->AnimateBones(tCurr, pBoneResult);
	}

	void Anim::PoseSkeleton(GameObjectAnimSkeleton* node)
	{
		assert(node);

		GameObjectAnimSkeleton* childNode = (GameObjectAnimSkeleton*)node->GetChild();
		GameObjectAnimSkeleton* parentNode = node;

		if (parentNode != nullptr && childNode != nullptr)
		{
			Vec3 start(0.0f, 0.0f, 0.0f);

			Vec3 ptA(Vec4(start, 1) * parentNode->GetWorld());
			Vec3 ptB(Vec4(start, 1) * childNode->GetWorld());

			Vec3 dir = ptB - ptA;

			float mag = dir.len();

			Scale S(.2f, .2f, mag * 2);
			Quat Q(Orient::LocalToWorld, dir.getNorm(), Vec3(0.0f, 1.0f, 0.0f));
			Trans T(ptA);
			Mat4 BoneOrient = S * Q * T;
			parentNode->SetBoneOrientation(BoneOrient);
		}

		if (parentNode != nullptr && childNode == nullptr)
		{
			GameObjectAnimSkin* grandParentNode = (GameObjectAnimSkin*)parentNode->GetParent();
			assert(grandParentNode);
			Vec3 pTmp1(Vec4(0, 0, 0, 1) * grandParentNode->GetWorld());
			Vec3 pTmp2(Vec4(0, 0, 0, 1) * parentNode->GetWorld());
			Vec3 dir = pTmp2 - pTmp1;
			float mag = dir.len();

			Vec3 pTmp3(Vec4(0, 0, 0, 1) * parentNode->GetWorld());
			Vec3 pTmp4(Vec4(1, 0, 0, 1) * parentNode->GetWorld());
			dir = pTmp4 - pTmp3;

			Scale S(.2f, .2f, mag * 2);
			Rot R(Orient::LocalToWorld, dir.getNorm(), Vec3(0.0f, 1.0f, 0.0f));
			Trans T(pTmp2);

			Mat4 mTmp = S * R * T;
			parentNode->SetBoneOrientation(mTmp);
		}
	}

	void Anim::SetClip(Clip::Name inClipName)
	{
		pClip = ClipManager::Find(inClipName);
	}
}