#ifndef _FontData
#define _FontData

#include "fontData.pb.h"
#include "glyphData.h"

class fontData
{
public:
	static constexpr unsigned int MaxGlyphs = 128u;

	fontData();
	fontData(const fontData&) = delete;
	fontData& operator = (const fontData&) = delete;
	~fontData();

	void Serialize(fontData_proto& out) const;
	void Deserialize(const fontData_proto& in);

	void Print(const char* const pName) const;

	glyphData* glyphs[MaxGlyphs];
};

#endif
