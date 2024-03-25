#ifndef _skeletonData
#define _skeletonData

#include "skeletonData.pb.h"

class skeletonData
{
public:
	skeletonData(int inNumBones);
	skeletonData(const skeletonData&) = delete;
	skeletonData& operator = (const skeletonData&) = delete;
	~skeletonData();

	void SetJointName(int index, const char* name);

	void SortByParentTree();

	void Serialize(skeletonData_proto& out) const;
	void Deserialize(const skeletonData_proto& in);

	void Print(const char* const pName) const;

	//static const int NUM_BONES = 8;
	static const int NAME_SIZE = 32;

	int numBones;

	std::vector<int> jointIndicies;
	std::vector<int> jointParentIndicies;
	std::vector<char*> jointNames;

	//unsigned int jointIndicies[NUM_BONES];
	//unsigned int jointParentIndicies[NUM_BONES];
	//char* jointNames[NUM_BONES];
};

#endif

// --- End of File ---
