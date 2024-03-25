#ifndef _GlyphData
#define _GlyphData

#include "glyphData.pb.h"

class glyphData
{
public:
	glyphData();
	glyphData(const glyphData&) = delete;
	glyphData& operator = (const glyphData&) = delete;
	~glyphData();

	void Serialize(glyphData_proto& out) const;
	void Deserialize(const glyphData_proto& in);

	void Print(const char* const pName) const;

	unsigned int key;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
};

#endif
