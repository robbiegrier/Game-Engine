#include "animClipData.h"

animClipData::~animClipData()
{
	animFrameData* pCurr = pFramesHead;

	while (pCurr)
	{
		animFrameData* pDelete = pCurr;
		pCurr = pCurr->pNext;
		delete pDelete;
	}
}

void animClipData::Serialize(animClipData_proto& out) const
{
	animFrameData* pCurr = pFramesHead;

	while (pCurr)
	{
		animFrameData_proto* afdProto = out.add_frames();
		pCurr->Serialize(*afdProto);
		pCurr = pCurr->pNext;
	}

	out.set_numbones(numBones);
}

void animClipData::Deserialize(const animClipData_proto& in)
{
	animFrameData* pPrev = nullptr;

	for (const animFrameData_proto& afdProto : in.frames())
	{
		animFrameData* pFrame = new animFrameData();
		pFrame->Deserialize(afdProto);

		if (pPrev)
		{
			pPrev->pNext = pFrame;
		}
		else
		{
			pFramesHead = pFrame;
		}

		pPrev = pFrame;
	}

	numBones = in.numbones();
}

void animClipData::Print(const char* const pName) const
{
	Trace::out("%s: \n", pName);

	Trace::out("num bones: %d\n", numBones);

	animFrameData* pCurr = pFramesHead;

	while (pCurr)
	{
		pCurr->Print("frame");
		pCurr = pCurr->pNext;
	}
}