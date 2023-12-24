#ifndef _animFrameData
#define _animFrameData

#include "boneData.h"
#include "animFrameData.pb.h"

class animFrameData
{
public:
	animFrameData() = default;
	animFrameData(const animFrameData&) = delete;
	animFrameData& operator = (const animFrameData&) = delete;
	~animFrameData();

	void Serialize(animFrameData_proto& out) const;
	void Deserialize(const animFrameData_proto& in);

	void Print(const char* const pName) const;

	boneData* pBonesHead = nullptr;
	int frameNumber = 0;

	animFrameData* pNext = nullptr;
};

#endif

// --- End of File ---
