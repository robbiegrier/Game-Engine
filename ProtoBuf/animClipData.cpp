#include "animClipData.h"

animClipData::~animClipData()
{
	while (!frames.empty())
	{
		animFrameData* pDelete = frames.back();
		frames.pop_back();
		delete pDelete;
	}
}

void animClipData::Serialize(animClipData_proto& out) const
{
	for (animFrameData* pFrame : frames)
	{
		animFrameData_proto* afdProto = out.add_frames();
		pFrame->Serialize(*afdProto);
	}
}

void animClipData::Deserialize(const animClipData_proto& in)
{
	for (const animFrameData_proto& afdProto : in.frames())
	{
		animFrameData* pFrame = new animFrameData();
		pFrame->Deserialize(afdProto);
		frames.push_back(pFrame);
	}
}

void animClipData::Print(const char* const pName) const
{
	Trace::out("%s: \n", pName);

	for (animFrameData* pFrame : frames)
	{
		pFrame->Print("frame");
	}
}