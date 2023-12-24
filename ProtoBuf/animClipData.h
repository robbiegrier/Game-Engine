#ifndef _animClipData
#define _animClipData

#include "animFrameData.h"
#include "animClipData.pb.h"

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

	animFrameData* pFramesHead = nullptr;
};

#endif

// --- End of File ---
