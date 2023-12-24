#include "boneData.h"

boneData::boneData()
{
	memset(translation, 0, sizeof(translation));
	memset(rotation, 0, sizeof(rotation));
	memset(scale, 0, sizeof(scale));
}

boneData::~boneData()
{
}

void boneData::Serialize(boneData_proto& out) const
{
	for (int i = 0; i < 3; i++)
	{
		out.add_translation(translation[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		out.add_rotation(rotation[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		out.add_scale(scale[i]);
	}
}

void boneData::Deserialize(const boneData_proto& in)
{
	for (int i = 0; i < 3; i++)
	{
		translation[i] = in.translation().at(i);
	}

	for (int i = 0; i < 4; i++)
	{
		rotation[i] = in.rotation().at(i);
	}

	for (int i = 0; i < 3; i++)
	{
		scale[i] = in.scale().at(i);
	}
}

void boneData::Print(const char* const pName) const
{
	Trace::out("%s (%d):", pName, boneNumber);

	Trace::out("T: ", pName);
	for (int i = 0; i < 3; i++)
	{
		Trace::out("%.4f ", translation[i]);
	}
	Trace::out("| ");

	Trace::out("Q: ", pName);
	for (int i = 0; i < 4; i++)
	{
		Trace::out("%.4f ", rotation[i]);
	}
	Trace::out("| ");

	Trace::out("S: ", pName);
	for (int i = 0; i < 3; i++)
	{
		Trace::out("%.1f ", scale[i]);
	}
	Trace::out("\n");
}