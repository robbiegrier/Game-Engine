#include "fontData.h"

fontData::fontData()
{
	for (unsigned int i = 0; i < MaxGlyphs; i++)
	{
		glyphs[i] = nullptr;
	}
}

fontData::~fontData()
{
	for (unsigned int i = 0; i < MaxGlyphs; i++)
	{
		if (glyphs[i])
		{
			delete glyphs[i];
		}
	}
}

void fontData::Serialize(fontData_proto& out) const
{
	out.clear_fontmetrics();

	for (unsigned int i = 0; i < MaxGlyphs; i++)
	{
		if (glyphs[i])
		{
			glyphData_proto* gdp = out.add_fontmetrics();
			glyphs[i]->Serialize(*gdp);
		}
	}
}

void fontData::Deserialize(const fontData_proto& in)
{
	for (unsigned int i = 0; i < MaxGlyphs; i++)
	{
		if (glyphs[i])
		{
			delete glyphs[i];
			glyphs[i] = nullptr;
		}
	}

	for (int i = 0; i < in.fontmetrics().size(); i++)
	{
		const glyphData_proto& gdp = in.fontmetrics().at(i);

		if (!glyphs[gdp.key()])
		{
			glyphs[gdp.key()] = new glyphData();
			glyphs[gdp.key()]->Deserialize(gdp);
		}
		else
		{
			Trace::out("Warning: duplicate key: %d\n", gdp.key());
		}
	}
}

void fontData::Print(const char* const pName) const
{
	Trace::out("%s: \n", pName);

	for (unsigned int i = 0; i < MaxGlyphs; i++)
	{
		if (glyphs[i])
		{
			glyphs[i]->Print("glyph");
		}
	}
}