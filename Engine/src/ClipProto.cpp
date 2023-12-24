#include "ClipProto.h"
#include "EngineUtils.h"

namespace Azul
{
	ClipProto::ClipProto(const char* pAnimFilename, Clip::Name inName)
		: Clip(12, inName)
	{
		animClipData_proto acdProto;
		acdProto.ParseFromString(EngineUtils::FileToString(pAnimFilename));

		animClipData clipData;
		clipData.Deserialize(acdProto);

		SetFromClipData(clipData);

		Trace::out("Clip %s loaded from converter\n", pAnimFilename);
	}

	void ClipProto::SetFromClipData(const animClipData& clipData)
	{
		FrameBucket* pTmp = nullptr;
		FrameBucket* pTmpX = nullptr;

		for (animFrameData* pFrame : clipData.frames)
		{
			pTmpX = new FrameBucket();
			pTmpX->prevBucket = pTmp;
			pTmpX->nextBucket = nullptr;
			pTmpX->KeyTime = 1 * AnimTime(AnimTime::Duration::FILM_24_FRAME);
			pTmpX->poBone = new Bone[(unsigned int)this->numBones];

			if (pTmp)
			{
				pTmp->nextBucket = pTmpX;
			}

			pTmp = pTmpX;

			int i = 0;
			for (boneData* pBone : pFrame->bones)
			{
				pTmp->poBone[i].T = Vec3(pBone->translation[0], pBone->translation[1], pBone->translation[2]);
				pTmp->poBone[i].Q = Quat(pBone->rotation[0], pBone->rotation[1], pBone->rotation[2], pBone->rotation[3]);
				pTmp->poBone[i].S = Vec3(pBone->scale[0], pBone->scale[1], pBone->scale[2]);
				i++;
			}
		}
	}
}