#include "skeletonData.h"

skeletonData::skeletonData()
{
	for (int i = 0; i < (int)NUM_BONES; i++)
	{
		jointIndicies[i] = 0u;
		jointParentIndicies[i] = 0u;
		jointNames[i] = new char[NAME_SIZE];
		memset(jointNames[i], 0, NAME_SIZE);
	}
}

skeletonData::~skeletonData()
{
	for (int i = 0; i < (int)NUM_BONES; i++)
	{
		delete[] jointNames[i];
	}
}

void skeletonData::SetJointName(int index, const char* name)
{
	memcpy(jointNames[index], name, NAME_SIZE);
}

void skeletonData::Serialize(skeletonData_proto& out) const
{
	for (int i = 0; i < (int)NUM_BONES; i++)
	{
		out.add_jointindicies(jointIndicies[i]);
		out.add_jointparentindicies(jointParentIndicies[i]);
		out.add_jointnames(std::string(jointNames[i]));
	}
}

void skeletonData::Deserialize(const skeletonData_proto& in)
{
	for (int i = 0; i < (int)NUM_BONES; i++)
	{
		jointIndicies[i] = in.jointindicies().at(i);
		jointParentIndicies[i] = in.jointparentindicies().at(i);
		SetJointName(i, in.jointnames().at(i).c_str());
	}
}

void skeletonData::Print(const char* const pName) const
{
	Trace::out("skeleton %s: \n", pName);

	for (int i = 0; i < (int)NUM_BONES; i++)
	{
		Trace::out("ind: %d | parent: %d | name: %s \n", jointIndicies[i], jointParentIndicies[i], jointNames[i]);
	}
}