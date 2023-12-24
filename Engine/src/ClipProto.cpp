#include "ClipProto.h"
#include "EngineUtils.h"

namespace Azul
{
	ClipProto::ClipProto(const char* pAnimFilename, Clip::Name inName)
		: Clip()
	{
		animClipData_proto acdProto;
		acdProto.ParseFromString(EngineUtils::FileToString(pAnimFilename));

		animClipData clipData;
		clipData.Deserialize(acdProto);

		numBones = 12;
		numFrames = 0;
		totalTime = AnimTime(AnimTime::Duration::ZERO);
		pHead = nullptr;

		SetFromClipData(clipData);

		totalTime = privFindMaxTime();
		numFrames = privFindNumFrames();

		Trace::out("Clip %s loaded from converter\n", pAnimFilename);
	}

	void ClipProto::SetFromClipData(const animClipData& clipData)
	{
		FrameBucket* pTmp = nullptr;
		FrameBucket* pTmpX = nullptr;

		for (int i = 0; i < (int)clipData.frames.size(); i++)
		{
			animFrameData* pFrame = clipData.frames[i];
			pTmpX = new FrameBucket();
			pTmpX->prevBucket = pTmp;
			pTmpX->nextBucket = nullptr;
			pTmpX->KeyTime = i * AnimTime(AnimTime::Duration::FILM_24_FRAME);
			pTmpX->poBone = new Bone[(unsigned int)this->numBones];

			if (pTmp)
			{
				pTmp->nextBucket = pTmpX;
			}
			else
			{
				this->pHead = pTmpX;
			}

			pTmp = pTmpX;

			for (int j = 0; j < (int)pFrame->bones.size(); j++)
			{
				boneData* pBone = pFrame->bones[j];
				pTmp->poBone[j].T = Vec3(pBone->translation[0], pBone->translation[1], pBone->translation[2]);
				pTmp->poBone[j].Q = Quat(pBone->rotation[0], pBone->rotation[1], pBone->rotation[2], pBone->rotation[3]);
				pTmp->poBone[j].S = Vec3(pBone->scale[0], pBone->scale[1], pBone->scale[2]);
			}
		}
	}
}