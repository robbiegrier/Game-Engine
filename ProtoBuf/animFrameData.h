#ifndef _animFrameData
#define _animFrameData

#include "boneData.h"
#include "animFrameData.pb.h"
#include <vector>

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

	std::vector<boneData*> bones;
};

#endif

// --- End of File ---
