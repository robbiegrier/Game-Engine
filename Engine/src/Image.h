#ifndef _Image
#define _Image

#include "DLink.h"
#include "Rect.h"
#include "TextureObject.h"

namespace Azul
{
	class Image : public DLink
	{
	public:

		enum Name
		{
			WhiteBird,
			YellowBird,
			GreenBird,
			Stitch,
			ALLBirds,
			Text,
			Null,
		};

	public:
		Image();
		Image(Image::Name imageName, TextureObject::Name textName, Rect rect);
		Image(Image::Name imageName, TextureObject* pTexture, Rect rect);
		virtual ~Image() = default;

		void Set(Image::Name imageName, TextureObject::Name textName, Rect rect);
		void Set(Image::Name imageName, TextureObject* pTexture, Rect rect);

		void SetTexture(TextureObject* pTexture);
		void SetRect(Rect rect);

		virtual bool Compare(DLink* pTarget) override;
		virtual void Dump() override;
		virtual void Wash() override;
		const char* NameToString();
		void SetName(Image::Name inName);
		Image::Name GetName() const;

	public:
		Image::Name imageName;
		TextureObject* pText;
		Rect imageRect;
	};
}

#endif
