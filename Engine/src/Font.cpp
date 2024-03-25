#include "Font.h"
#include "Glyph.h"
#include "fontData.h"
#include "EngineUtils.h"

namespace Azul
{
	Font::Font(const char* pFilename, TextureObject* pInTexture)
		: pTexture(pInTexture)
	{
		for (int i = 0; i < (int)MaxGlyphs; i++)
		{
			pGlyphArray[i] = nullptr;
		}

		fontData_proto proto;
		proto.ParseFromString(EngineUtils::FileToString(pFilename));

		fontData fd;
		fd.Deserialize(proto);

		for (int i = 0; i < (int)MaxGlyphs; i++)
		{
			glyphData* pGlyphData = fd.glyphs[i];

			if (pGlyphData)
			{
				//pGlyphData->Print("loaded glyph");

				Glyph* pGlyph = new Glyph(Rect(
					static_cast<float>(pGlyphData->x),
					static_cast<float>(pGlyphData->y),
					static_cast<float>(pGlyphData->width),
					static_cast<float>(pGlyphData->height)
				));

				pGlyphArray[pGlyphData->key] = pGlyph;
			}
		}
	}

	Font::~Font()
	{
		for (int i = 0; i < (int)MaxGlyphs; i++)
		{
			if (pGlyphArray[i])
			{
				delete pGlyphArray[i];
				pGlyphArray[i] = nullptr;
			}
		}
	}

	Glyph* Font::GetGlyph(char character) const
	{
		const unsigned int charAsIndex = static_cast<unsigned int>(character);
		return pGlyphArray[charAsIndex];
	}

	TextureObject* Font::GetTexture() const
	{
		return pTexture;
	}

	void Font::Wash()
	{
		for (int i = 0; i < (int)MaxGlyphs; i++)
		{
			if (pGlyphArray[i])
			{
				delete pGlyphArray[i];
				pGlyphArray[i] = nullptr;
			}
		}

		pTexture = nullptr;
	}

	bool Font::Compare(DLink* pTargetNode)
	{
		Font* pOther = (Font*)pTargetNode;
		return pOther->GetName() == GetName();
	}

	void Font::Dump()
	{
		Trace::out("%s\n", NameToString());
	}

	void Font::SetName(Font::Name inName)
	{
		name = inName;
	}

	Font::Name Font::GetName() const
	{
		return name;
	}

	const char* Font::NameToString()
	{
		switch (name)
		{
		case Name::Century:
			return "Century Gothic";
		case Name::Null:
			return "Null";
		default:
			return "None";
		}
	}
}