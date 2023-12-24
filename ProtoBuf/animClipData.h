#ifndef _animClipData
#define _animClipData

#include "animFrameData.h"
#include "animClipData.pb.h"
#include <vector>

class animClipData
{
public:
	animClipData() = default;
	animClipData(const animClipData&) = delete;
	animClipData& operator = (const animClipData&) = delete;
	~animClipData();

	void Serialize(animClipData_proto& out) const;
	void Deserialize(const animClipData_proto& in);

	void Print(const char* const pName) const;

	std::vector<animFrameData*> frames;
};

#endif

// --- End of File ---
