#include "animFrameData.h"

animFrameData::~animFrameData()
{
	while (!bones.empty())
	{
		boneData* pDelete = bones.back();
		bones.pop_back();
		delete pDelete;
	}
}

void animFrameData::Serialize(animFrameData_proto& out) const
{
	for (boneData* pBone : bones)
	{
		boneData_proto* bdProto = out.add_bones();
		pBone->Serialize(*bdProto);
	}
}

void animFrameData::Deserialize(const animFrameData_proto& in)
{
	for (const boneData_proto& bdProto : in.bones())
	{
		boneData* pBone = new boneData();
		pBone->Deserialize(bdProto);
		bones.push_back(pBone);
	}
}

void animFrameData::Print(const char* const pName) const
{
	Trace::out("%s: \n", pName);

	for (boneData* pBone : bones)
	{
		pBone->Print("bone");
	}
}