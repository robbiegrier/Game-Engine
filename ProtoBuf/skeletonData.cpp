#include "skeletonData.h"
#include <stack>

skeletonData::skeletonData(int inNumBones)
	: numBones(inNumBones)
{
	jointIndicies = std::vector<int>(numBones, 0);
	jointParentIndicies = std::vector<int>(numBones, 0);
	jointNames = std::vector<char*>(numBones, nullptr);

	for (int i = 0; i < numBones; i++)
	{
		jointIndicies[i] = 0u;
		jointParentIndicies[i] = 0u;
		jointNames[i] = new char[NAME_SIZE];
		memset(jointNames[i], 0, NAME_SIZE);
	}
}

skeletonData::~skeletonData()
{
	for (int i = 0; i < numBones; i++)
	{
		delete[] jointNames[i];
	}
}

void skeletonData::SetJointName(int index, const char* name)
{
	memcpy(jointNames[index], name, NAME_SIZE);
}

void skeletonData::SortByParentTree()
{
	//Trace::out("SORTING SKEL:\n");
	struct FSkel
	{
		int index;
		int parent;
		std::string name;
	};

	std::vector<FSkel> entries;

	for (int i = 0; i < (int)jointIndicies.size(); i++)
	{
		FSkel data;
		data.index = jointIndicies[i];
		data.parent = jointParentIndicies[i];
		data.name = jointNames[i];
		entries.push_back(data);
	}

	//Trace::out("BEFORE:\n");
	//for (auto& data : entries)
	//{
	//	Trace::out("\t{ %d, %d, \"%s\" },\n", data.index, data.parent, data.name.c_str());
	//}

	std::vector<FSkel> sorted;
	std::stack<int> frontier;

	sorted.push_back(entries[0]);
	frontier.push(0);

	while (!frontier.empty())
	{
		int searchParent = frontier.top();
		frontier.pop();

		for (auto& data : entries)
		{
			if (data.parent == searchParent)
			{
				sorted.push_back(data);
				frontier.push(data.index);
			}
		}
	}

	//Trace::out("AFTER:\n");
	//for (auto& data : sorted)
	//{
	//	Trace::out("\t{ %d, %d, \"%s\" },\n", data.index, data.parent, data.name.c_str());
	//}

	for (int i = 0; i < numBones; i++)
	{
		jointIndicies[i] = sorted[i].index;
		jointParentIndicies[i] = sorted[i].parent;
		SetJointName(i, sorted[i].name.c_str());
	}
}

void skeletonData::Serialize(skeletonData_proto& out) const
{
	for (int i = 0; i < numBones; i++)
	{
		out.add_jointindicies(jointIndicies[i]);
		out.add_jointparentindicies(jointParentIndicies[i]);
		out.add_jointnames(std::string(jointNames[i]));
	}
}

void skeletonData::Deserialize(const skeletonData_proto& in)
{
	for (int i = 0; i < numBones; i++)
	{
		if (jointNames[i])
		{
			delete[] jointNames[i];
		}
	}

	numBones = in.jointindicies().size();

	jointIndicies = std::vector<int>(numBones, 0);
	jointParentIndicies = std::vector<int>(numBones, 0);
	jointNames = std::vector<char*>(numBones, nullptr);

	for (int i = 0; i < numBones; i++)
	{
		jointIndicies[i] = 0u;
		jointParentIndicies[i] = 0u;
		jointNames[i] = new char[NAME_SIZE];
		memset(jointNames[i], 0, NAME_SIZE);
	}

	for (int i = 0; i < numBones; i++)
	{
		jointIndicies[i] = in.jointindicies().at(i);
		jointParentIndicies[i] = in.jointparentindicies().at(i);
		SetJointName(i, in.jointnames().at(i).c_str());
	}
}

void skeletonData::Print(const char* const pName) const
{
	Trace::out("skeleton %s: \n", pName);

	for (int i = 0; i < numBones; i++)
	{
		Trace::out("ind: %d | parent: %d | name: %s \n", jointIndicies[i], jointParentIndicies[i], jointNames[i]);
	}
}