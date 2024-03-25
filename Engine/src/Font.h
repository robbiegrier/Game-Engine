#ifndef _Font
#define _Font

#include "DLink.h"

namespace Azul
{
	class TextureObject;
	class Glyph;

	class Font : public DLink
	{
	public:
		enum class Name
		{
			Century,
			Null
		};

		static constexpr unsigned int MaxGlyphs = 128;

		Font() = delete;
		Font(const char* pFilename, TextureObject* pInTexture);
		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;
		~Font();

		Glyph* GetGlyph(char character) const;
		TextureObject* GetTexture() const;

		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;
		void SetName(Font::Name inName);
		Font::Name GetName() const;
		const char* NameToString();

	private:
		TextureObject* pTexture;
		Glyph* pGlyphArray[MaxGlyphs];
		Name name;
	};
}

#endif