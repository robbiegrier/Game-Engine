#ifndef _boneData
#define _boneData

#include "boneData.pb.h"

class boneData
{
public:
	boneData();
	boneData(const boneData&) = delete;
	boneData& operator = (const boneData&) = delete;
	~boneData();

	void Serialize(boneData_proto& out) const;
	void Deserialize(const boneData_proto& in);

	void Print(const char* const pName) const;

	float translation[3];
	float rotation[4];
	float scale[3];
};

#endif

// --- End of File ---
