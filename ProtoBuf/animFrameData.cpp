#include "animFrameData.h"

animFrameData::~animFrameData()
{
	boneData* pCurr = pBonesHead;

	while (pCurr)
	{
		boneData* pDelete = pCurr;
		pCurr = pCurr->pNext;
		delete pDelete;
	}
}

void animFrameData::Serialize(animFrameData_proto& out) const
{
	boneData* pCurr = pBonesHead;

	while (pCurr)
	{
		boneData_proto* bdProto = out.add_bones();
		pCurr->Serialize(*bdProto);
		pCurr = pCurr->pNext;
	}
}

void animFrameData::Deserialize(const animFrameData_proto& in)
{
	boneData* pPrev = nullptr;

	for (const boneData_proto& bdProto : in.bones())
	{
		boneData* pBone = new boneData();
		pBone->Deserialize(bdProto);

		if (pPrev)
		{
			pPrev->pNext = pBone;
		}
		else
		{
			pBonesHead = pBone;
		}

		pPrev = pBone;
	}
}

void animFrameData::Print(const char* const pName) const
{
	Trace::out("%s (%d): \n", pName, frameNumber);

	boneData* pCurr = pBonesHead;

	while (pCurr)
	{
		pCurr->Print("bone");
		pCurr = pCurr->pNext;
	}
}